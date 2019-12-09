#include "../stdafx.h"

#include "../Include/Globals.h"
#include "../Include/CAppQuadTree.h"
#include "..\Include\CQuadTree.h"
#include <iostream>

using std::cout;

//---
CAppQuadTree::CAppQuadTree() : CAppQuadTree(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT){

	//CVector3 _m_worldUp, CVector3 _m_eyePos, CVector3 _m_lookAt, int _m_frameBufferWidth, int _m_frameBufferHeight, float _m_fieldOfView, float _m_nearPlane, float _m_farPlane
}

//---
CAppQuadTree::CAppQuadTree(int window_width, int window_height) : CApp(window_width, window_height){
	cout << "Constructor: CAppQuadTree(int window_width, int window_height)" << endl;
	m_camera = new CCamera({ 0, 1, 0 }, { 0, 3, 0 }, { 0, 0, -4 }, window_width, window_height, 75.0f, 0.001f, 1000.0f);

	m_vertexArrayObjectHexaGrid = 0;
}

// Free memory allocated in this class instance here
CAppQuadTree::~CAppQuadTree(){
	cout << "Destructor: ~CAppQuadTree()" << endl;
	if (m_camera!= nullptr) {
		delete m_camera;
	}
}

//Aquí inicia lo mio ^-^

//Función para poder mandar a llamar la función que ejecutará 
//DWORD WINAPI ThreadCreateGrid(LPVOID _variable) {
//	CAppQuadTree *cAppQuadTree = (CAppQuadTree*)_variable;
//	cAppQuadTree->StartThreadGrid();
//	return 0;
//}


void CAppQuadTree::initialize() {
	/*
	//threadGrid = CreateThread(NULL, 0, ThreadCreateGrid, NULL, 0, &threadEndId);	//Creamos el thread 1

	//Obtener número de filas y columnas
	//m_cols = m_objHexWorld->num_cols;
	//m_rows = m_objHexWorld->num_rows;

	//Ciclo para poder obtener los puntos mas alejados
	for (int i = 0; i < m_cols; i++) {
		for (int j = 0; j < m_rows; j++) {
	
			//CHexGridCell cell = m_grid->getCell(i,j);
			for (int k = 0; k < 6; k++) {

				//CVector3 corner = cell->getCorner(k);

				//if (corner.x < m_minX) { m_minX = corner.x; }
				//if (corner.x < m_maxX) { m_maxX = corner.x; }
				//if (corner.z < m_minX) { m_minZ = corner.z; }
				//if (corner.z < m_minX) { m_maxZ = corner.z; }
			}

		}
	}
	*/

	m_ColorModelShaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	if (m_ColorModelShaderID == 0){

		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}

	m_texturedModelShaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (m_texturedModelShaderID == 0){

		cout << "ERROR: Unable to load texture shader" << endl;
		return;
	}

	m_objHexGrid = new Hexa_Grid();

	m_objHexGrid->initialize(3, 3, 1, true, {0.0f, 0.0f, 0.0f});

	bool isLoaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(&m_ColorModelShaderID, &m_vertexArrayObjectHexaGrid, m_objHexGrid->m_vertex_data,
		6,
		m_objHexGrid->m_normal_Data,
		4,
		m_objHexGrid->m_vertex_UVs,
		6,
		m_objHexGrid->m_face_Indices,
		4,
		m_objHexGrid->m_face_Normal_Indices,
		4,
		m_objHexGrid->m_face_Indices,
		4
	);

	if (!isLoaded){

		if (m_vertexArrayObjectHexaGrid > 0){

			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_vertexArrayObjectHexaGrid);
			m_vertexArrayObjectHexaGrid = 0;
		}
	}

	m_minX = 0.0f;
	m_maxX = 0.0f;
	m_minZ = 0.0f;
	m_maxZ = 0.0f;

	//Filas
	for (int i = 0; i < 3; i++) {

		//Columnas
		for (int j = 0; j < 3; j++) {

			//Puntos
			for (int k = 1; k < 7; k++) {

				CVector3 puntos = m_objHexGrid->hexa_points(m_objHexGrid->vec_center_hexa[i][j].centers, k, 6, true);

				if (puntos.getX() < m_minX) {

					m_minX = puntos.getX();
				}
				if (puntos.getX() > m_maxX) {

					m_maxX = puntos.getX();
				}
				if (puntos.getZ() < m_minZ) {

					m_minZ = puntos.getZ();
				}
				if (puntos.getZ() > m_maxZ) {

					m_maxZ = puntos.getZ();
				}
			}
		}
	}

	//Generar el boundering
	CVector3 limits[4];

	limits[0] = { m_minX, 0, m_minZ };
	limits[1] = { m_maxX, 0, m_minZ };
	limits[2] = { m_minX, 0, m_maxZ };
	limits[3] = { m_maxX, 0, m_maxZ };

	m_boundering.setCorners(limits);

	m_objQuadTree.Subdivide(m_boundering);

	if (!m_objQuadTree.LoadGeometry(getOpenGLRenderer(), m_ColorModelShaderID)) {

		std::cout << "\n\nNo cargo la geometria del quadtree\n\n";
	}


}

void CAppQuadTree::render(){

	float color[3] = { 41.0f, 255.0f, 218.0f };
	unsigned int noTexture = 0;

	if (m_vertexArrayObjectHexaGrid > 0 && m_objHexGrid->numFaces > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(0, m_objHexGrid->vec_center_hexa[i][j].centers);

				getOpenGLRenderer()->renderObject
				(
					&m_ColorModelShaderID,
					&m_vertexArrayObjectHexaGrid,
					&noTexture,
					m_objHexGrid->numFaces,
					color,
					&modelMatrix,
					COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
					false
				);
			}
		}
	}

	m_objQuadTree.m_root->render(getOpenGLRenderer(), m_ColorModelShaderID);
}

//Función para iniciar con el Hexa Grid
bool CAppQuadTree::StartThreadGrid(){
	return false;
}




void CAppQuadTree::onArrowUp(int _key){

	m_camera->moveForward(0.025f);
}

void CAppQuadTree::onArrowDown(int _key){

	m_camera->moveForward(-0.025f);
}

void CAppQuadTree::onArrowLeft(int _key){

	m_camera->moveUp(-0.025f);
}

void CAppQuadTree::onArrowRight(int _key) {

	m_camera->moveUp(0.025f);
}

//---
void CAppQuadTree::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_QUADTREE_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}
//---
void CAppQuadTree::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	// ===============================
}
//---
void CAppQuadTree::onMouseMove(float deltaX, float deltaY){
	
	if (deltaX < 100.0f && deltaY < 100.0f)
	{
		float moveX = -deltaX * DEFAULT_CAMERA_MOVE_SPEED;
		float moveZ = -deltaY * DEFAULT_CAMERA_MOVE_SPEED;

		float currPos[3];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m_objHexGrid->vec_center_hexa[i][j].centers.getValues(currPos);
				currPos[0] += moveX;
				currPos[2] += moveZ;
				m_objHexGrid->vec_center_hexa[i][j].centers.setValues(currPos);
			}
		}
	}
}

void CAppQuadTree::moveCamera(float _direction){

	if (getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->simpleCameraZoom(_direction);
	}
}

void CAppQuadTree::onF3(int mods){

	if (mods & KEY_MOD_SHIFT)
	{
		moveCamera(-1.0f);
	}
	else
	{
		moveCamera(1.0f);
	}
}
//---
void CAppQuadTree::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}