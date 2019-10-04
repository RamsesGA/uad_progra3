#include "../stdafx.h"

// include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/COpenGLRenderer.h"
#include "../Include/LoadShaders.h"
#include "../Include/MathHelper.h"
#include "../Include/CWideStringHelper.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * ONLY MODIFY THIS FILE IF YOU KNOW WHAT YOU'RE DOING...
 * AND EVEN THEN... BE CAREFUL...
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
*/
COpenGLRenderer::COpenGLRenderer():
	m_OpenGLError{ false },
	m_cameraDistance{ MIN_CAMERA_DISTANCE },
	m_frameBufferWidth{ 0 },
	m_frameBufferHeight{ 0 },
	m_testCubeVAOID{ 0 },
	m_mCCubeVAOID{ 0 },
	m_testCubeShaderProgramID{ 0 },
	m_mCCubeShaderProgramID{ 0 },
	m_activeShaderProgram{ nullptr },
	m_activeShaderProgramWrapper{ nullptr }
{
	cout << "Constructor: COpenGLRenderer()" << endl;

	m_expectedUniformsInShader.push_back(UNIFORM_MODEL_MATRIX);
	m_expectedUniformsInShader.push_back(UNIFORM_VIEW_MATRIX);
	m_expectedUniformsInShader.push_back(UNIFORM_PROJECTION_MATRIX);
	m_expectedUniformsInShader.push_back(UNIFORM_COLOR);
	m_expectedUniformsInShader.push_back(UNIFORM_TEXTURE_SAMPLER);

	m_expectedAttributesInShader.push_back(ATTRIBUTE_POSITION);
	m_expectedAttributesInShader.push_back(ATTRIBUTE_NORMAL);
	m_expectedAttributesInShader.push_back(ATTRIBUTE_UV);
	m_expectedAttributesInShader.push_back(ATTRIBUTE_COLOR);
	m_expectedAttributesInShader.push_back(ATTRIBUTE_INSTANCING_MVP_MAT4);
	m_expectedAttributesInShader.push_back(ATTRIBUTE_INSTANCING_WORLD_MAT4);

	// Load program names, vertex and fragment shaders in the same pos in each array
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_COLOR_CUBE);
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_COLOR_OBJECT);
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_TEXTURED_OBJECT);
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_COLOR_OBJECT_PHONG);
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_TEXTURED_OBJECT_PHONE);
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_TEXTURED_CUBE);
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_MENU);
	m_knownShaderProgramNames.push_back(SHADER_PROGRAM_WIREFRAME);

	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_3D_OBJECT_COLOR);
	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_3D_OBJECT);
	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_TEXTURED_3D_OBJECT);
	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_3D_OBJECT_PHONG);
	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_TEXTURE_3D_OBJECT_PHONG);
	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_MC_CUBE);
	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_MENU);
	m_knownShaderProgramVertexShaders.push_back(VERTEX_SHADER_WIREFRAME);

	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_3D_OBJECT_COLOR);
	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_3D_OBJECT);
	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_TEXTURED_3D_OBJECT);
	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_3D_OBJECT_PHONG);
	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_TEXTURE_3D_OBJECT_PHONG);
	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_MC_CUBE);
	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_MENU);
	m_knownShaderProgramFragmentShaders.push_back(FRAGMENT_SHADER_WIREFRAME);
}

/*
*/
COpenGLRenderer::~COpenGLRenderer()
{
	cout << "Destructor: ~COpenGLRenderer()" << endl;

	// Delete all shader programs
	for (auto it = m_shaderProgramWrappers.begin(); it != m_shaderProgramWrappers.end(); ++it)
	{
		COpenGLShaderProgram *shaderProgramWrapper = (COpenGLShaderProgram *)it->second;

		if (shaderProgramWrapper != nullptr)
		{
			glDeleteProgram(shaderProgramWrapper->getShaderProgramID());
			delete shaderProgramWrapper;
			shaderProgramWrapper = nullptr;
		}
	}

	m_shaderProgramWrappers.clear();
	m_knownShaders.clear();
	m_expectedUniformsInShader.clear();
	m_expectedAttributesInShader.clear();

	if (m_testCubeVAOID != 0)
	{
		deleteVertexArrayObject(&m_testCubeVAOID);
	}

	// MC Cube
	if (m_mCCubeVAOID != 0)
	{
		deleteVertexArrayObject(&m_mCCubeVAOID);
	}

	m_activeShaderProgram = nullptr;
	m_activeShaderProgramWrapper = nullptr;
}

/*
*/
bool COpenGLRenderer::initialize()
{
	unsigned int result = 0;
	int loadedShaderCount = 0;
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	// Make sure size of all shader filenames vectors is the same
	if (m_knownShaderProgramNames.size() == m_knownShaderProgramVertexShaders.size() &&
		m_knownShaderProgramNames.size() == m_knownShaderProgramFragmentShaders.size())
	{
		// Loop through all known program names vector
		for (int i = 0; i < (int)m_knownShaderProgramNames.size(); ++i)
		{
			resourceFilenameFS = "";
			resourceFilenameVS = "";
			wresourceFilenameFS = L"";
			wresourceFilenameVS = L"";

			// Get the full filename of the vertex and fragment shaders
			if (!CWideStringHelper::GetResourceFullPath(m_knownShaderProgramVertexShaders[i].c_str(), wresourceFilenameVS, resourceFilenameVS) ||
				!CWideStringHelper::GetResourceFullPath(m_knownShaderProgramFragmentShaders[i].c_str(), wresourceFilenameFS, resourceFilenameFS))
			{
				cout << "ERROR: Unable to find one or more shaders: " << endl;
				cout << "  " << m_knownShaderProgramVertexShaders[i].c_str() << endl;
				cout << "  " << m_knownShaderProgramFragmentShaders[i].c_str() << endl;
			}
			else
			{
				// Create the shader
				if (createShaderProgram(
					m_knownShaderProgramNames[i],
					&result,
					resourceFilenameVS.c_str(),
					resourceFilenameFS.c_str()
				))
				{
					++loadedShaderCount;
				}
			}
		}
	}
	else
	{
		std::cout << "ERROR: Missing or invalid known program shaders" << std::endl;
	}

	return (loadedShaderCount == (int)m_knownShaderProgramNames.size());
}

/*
*/
unsigned int COpenGLRenderer::getShaderProgramID(std::string shaderProgramName)
{
	unsigned int result = 0;
	auto it = m_knownShaders.find(shaderProgramName);

	if (it != m_knownShaders.end())
	{
		result = it->second;
	}

	return result;
}

/*
*/
bool COpenGLRenderer::createShaderProgram(std::string shaderProgramName, unsigned int *shaderProgramId, const char *vertexShader, const char *fragmentShader)
{
	if (shaderProgramId != NULL)
	{
		ShaderInfo shaders[3];

		shaders[0].type = GL_VERTEX_SHADER;
		shaders[0].filename = vertexShader;

		shaders[1].type = GL_FRAGMENT_SHADER;
		shaders[1].filename = fragmentShader;

		shaders[2].type = GL_NONE;
		shaders[2].filename = NULL;

		*shaderProgramId = (unsigned int)LoadShaders(shaders);

		if (*shaderProgramId > 0)
		{
			// Create new shader program
			COpenGLShaderProgram *newShaderProgramWrapper = new COpenGLShaderProgram();
			
			// Set shader program Id
			newShaderProgramWrapper->setShaderProgramID(*shaderProgramId);

			// Search for uniforms and attributes
			// NOTE: We're assuming these attrib and uniforms are in the shader

			cout << "SHADER: " << vertexShader << endl;
			cout << "SHADER: " << fragmentShader << endl;

			// Uniforms
			for (size_t idx = 0; idx < m_expectedUniformsInShader.size(); idx++)
			{
				int uniformLocation = glGetUniformLocation(*shaderProgramId, m_expectedUniformsInShader[idx].c_str());

				if (uniformLocation >= 0)
				{
					// Set the uniform location in the shader program
					newShaderProgramWrapper->setUniformLocation(m_expectedUniformsInShader[idx], uniformLocation);
					cout << "INFO: Found uniform location for: " << m_expectedUniformsInShader[idx].c_str() << endl;
				}
				else
				{
					//cout << "WARNING: Unable to get uniform location for: " << m_expectedUniformsInShader[idx].c_str() << endl;
				}
			}

			// Attributes
			for (size_t idx = 0; idx < m_expectedAttributesInShader.size(); idx++)
			{
				int attributeLocation = glGetAttribLocation(*shaderProgramId, m_expectedAttributesInShader[idx].c_str());

				if (attributeLocation >= 0)
				{
					// Set the uniform location in the shader program
					newShaderProgramWrapper->setAttributeLocation(m_expectedAttributesInShader[idx], attributeLocation);
					cout << "INFO: Found attribute location for: " << m_expectedAttributesInShader[idx].c_str() << endl;
				}
				else
				{
					//cout << "WARNING: Unable to get attribute location for: " << m_expectedAttributesInShader[idx].c_str() << endl;
				}
			}	

			// Insert shader program in map
			m_shaderProgramWrappers.insert(std::make_pair(*shaderProgramId, newShaderProgramWrapper));

			// Insert into known shaders as well
			auto it_shader = m_knownShaders.find(shaderProgramName);

			// If no entry was found
			if (it_shader == m_knownShaders.end())
			{
				m_knownShaders.insert(std::pair<std::string, unsigned int>(shaderProgramName, *shaderProgramId));
			}
		}

		return (*shaderProgramId > 0);
	}

	return false;
}

/*
*/
bool COpenGLRenderer::deleteShaderProgram(unsigned int *shaderProgramId)
{
	if (shaderProgramId != NULL && *shaderProgramId > 0)
	{
		auto it = m_shaderProgramWrappers.find(*shaderProgramId);

		if (it != m_shaderProgramWrappers.end())
		{
			delete it->second;
			it->second = nullptr;
		}

		glDeleteProgram(*shaderProgramId);
		*shaderProgramId = 0;
		return true;
	}

	return false;
}

/*
*/
bool COpenGLRenderer::useShaderProgram(const unsigned int * const shaderProgramId) 
{
	if (shaderProgramId != nullptr && *shaderProgramId > 0)
	{
		// Make sure there's a shader with uniforms and attribs
		auto it = m_shaderProgramWrappers.find(*shaderProgramId);

		if (it != m_shaderProgramWrappers.end() && it->second != nullptr)
		{
			glUseProgram((GLuint)*shaderProgramId);
			m_activeShaderProgram = shaderProgramId;
			m_activeShaderProgramWrapper = it->second;

			return true;
		}
		else
		{
			m_activeShaderProgram = nullptr;
			m_activeShaderProgramWrapper = nullptr;
		}
	}
	else
	{
		glUseProgram(0);
		m_activeShaderProgram = nullptr;
		m_activeShaderProgramWrapper = nullptr;
	}

	return false;
}

/*
*/
void COpenGLRenderer::setCurrentVertexArrayObjectID(unsigned int *vertexArrayObjectId)
{
	if (vertexArrayObjectId != nullptr && *vertexArrayObjectId > 0)
	{
		// Bind vertex array object for this 3D object
		glBindVertexArray((GLuint)*vertexArrayObjectId);
	}
	else
	{
		// Unbind vertex array object
		glBindVertexArray(0);
	}
}

/*
*/
void COpenGLRenderer::setCurrentShaderObjectColor(GLfloat *objectColor)
{
	if (m_activeShaderProgram != nullptr &&
		m_activeShaderProgramWrapper != nullptr &&
		m_activeShaderProgramWrapper->getColorUniformLocation() >= 0)
	{
		glUniform3f(m_activeShaderProgramWrapper->getColorUniformLocation(), objectColor[0], objectColor[1], objectColor[2]);
	}
}

/*
 * Set the view matrix every time the camera changes
 */
void COpenGLRenderer::setCurrentShaderViewMatrix(MathHelper::Matrix4 *viewMatrix)
{
	if (m_activeShaderProgram != nullptr &&
		m_activeShaderProgramWrapper != nullptr &&
		m_activeShaderProgramWrapper->getViewMatrixUniformLocation() >= 0)
	{
		if (viewMatrix == nullptr)
		{
			MathHelper::Matrix4 simpleViewMatrix = MathHelper::SimpleViewMatrix(m_cameraDistance);
			glUniformMatrix4fv(m_activeShaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(simpleViewMatrix.m[0][0]));
		}
		else
		{
			glUniformMatrix4fv(m_activeShaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(viewMatrix->m[0][0]));
		}
	}
}

/*
 * Set the projection matrix every time the projection or window size changes
 */
void COpenGLRenderer::setCurrentShaderProjectionMatrix(MathHelper::Matrix4 *projectionMatrix)
{
	if (m_activeShaderProgram != nullptr &&
		m_activeShaderProgramWrapper != nullptr &&
		m_activeShaderProgramWrapper->getProjectionMatrixUniformLocation() >= 0)
	{
		if (projectionMatrix == nullptr)
		{
			if (m_frameBufferWidth == 0 || m_frameBufferHeight == 0)
			{
				m_frameBufferWidth = RENDERER_DEFAULT_FB_WIDTH;
				m_frameBufferHeight = RENDERER_DEFAULT_FB_HEIGHT;
			}

			MathHelper::Matrix4 simpleProjectionMatrix = MathHelper::SimpleProjectionMatrix(float(m_frameBufferWidth) / float(m_frameBufferHeight));
			glUniformMatrix4fv(m_activeShaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(simpleProjectionMatrix.m[0][0]));
		}
		else
		{
			glUniformMatrix4fv(m_activeShaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(projectionMatrix->m[0][0]));
		}
	}
}

/*
*/
void COpenGLRenderer::setCurrentShaderTexture(unsigned int *textureObjectId)
{
	// Set the texture sampler uniform
	if (textureObjectId != nullptr && 
		*textureObjectId > 0 &&
		m_activeShaderProgramWrapper != nullptr &&
		m_activeShaderProgramWrapper->getTextureSamplerUniformLocation() >= 0)
	{
		// DO NOT CALL glEnable(GL_TEXTURE_2D) OR OPENGL WILL RETURN AN "1280" ERROR
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *textureObjectId);
		glUniform1i(m_activeShaderProgramWrapper->getTextureSamplerUniformLocation(), 0);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/*
*/
void COpenGLRenderer::setCurrentShaderAmbientLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity)
{
	// Set the ambient light uniform
	if (m_activeShaderProgramWrapper != nullptr &&
		m_activeShaderProgramWrapper->getAmbientLightColorUniformLocation() >= 0 &&
		m_activeShaderProgramWrapper->getAmbientLightIntensityUniformLocation() >= 0)
	{
		glUniform3f(m_activeShaderProgramWrapper->getAmbientLightColorUniformLocation(), red, green, blue);
		glUniform1f(m_activeShaderProgramWrapper->getAmbientLightIntensityUniformLocation(), ambientIntensity);
	}
}

/*
*/
bool COpenGLRenderer::createTextureObject(unsigned int *textureObjectId, unsigned char *textureData, int width, int height)
{
	if (textureObjectId != NULL && textureData != NULL)
	{
		// Create an OpenGL texture object
		glGenTextures(1, textureObjectId);

		// Activate texture unit #0
		glActiveTexture(GL_TEXTURE0);

		// Bind the new texture objectly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, *textureObjectId);

		// Pass the data to OpenGL
		// *NOTE: We're specifying GL_BGR because we're reading TGA files, which have the RED and BLUE channels switched.
		//        Ideally, we should pass a parameter to specify this ;)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);

		// Set mipmap filtering modes and generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("COpenGLRenderer::createTextureObject");

		return true;
	}

	return false;
}

/*
*/
GLuint COpenGLRenderer::generateVertexArrayObjectID() const
{
	GLuint vAO_ID = 0;
	glGenVertexArrays(1, &vAO_ID);
	glBindVertexArray(vAO_ID);

	return vAO_ID;
}

/*
*/
void COpenGLRenderer::deleteVertexArrayObject(GLuint *id)
{
	glDeleteVertexArrays(1, id);
}

/*
*/
GLuint COpenGLRenderer::generateBufferObjectID(GLfloat * const vertices, int numVertices) const
{
	GLuint vBO_ID;
	glCreateBuffers(1, &vBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, vBO_ID);
	glNamedBufferStorage(vBO_ID, sizeof(GLfloat) * numVertices * 2, vertices, 0);

	return vBO_ID;
}

/*
*/
void COpenGLRenderer::deleteBufferObject(GLuint *id)
{
	glDeleteBuffers(1, id);
}

/*
 * Enables the given vertex shader attribute in the CURRENTLY BOUND buffer
*/
void COpenGLRenderer::setCurrentVertexAttribPointer(int vertexShaderAttribPos, int numComponents, int type) const
{
	glVertexAttribPointer(
		vertexShaderAttribPos,  // shader attribute location index
		numComponents,          // number of components (x, y, z)
		type,                   // 
		GL_FALSE,               // don't normalize
		0,                      // stride = 0
		BUFFER_OFFSET(0));      // pointer to the data, in the currently bound buffer

	glEnableVertexAttribArray(vertexShaderAttribPos);
}

/*
*/
COpenGLShaderProgram* COpenGLRenderer::getShaderProgramWrapper(unsigned int id)
{
	COpenGLShaderProgram *shaderProgramWrapper = nullptr;

	auto it = m_shaderProgramWrappers.find(id);

	if (it != m_shaderProgramWrappers.end())
	{
		shaderProgramWrapper = it->second;
	}

	return shaderProgramWrapper;
}

/*
*/
bool COpenGLRenderer::freeGraphicsMemoryForObject(/*unsigned int *shaderProgramId,*/ unsigned int *vertexArrayObjectID)
{
	bool deletedVertexArrayObjectId = false;

	if (vertexArrayObjectID != NULL && *vertexArrayObjectID > 0)
	{
		deleteVertexArrayObject((GLuint *)vertexArrayObjectID);
		*vertexArrayObjectID = 0;
		deletedVertexArrayObjectId = true;
	}

	return (deletedVertexArrayObjectId);
}

/*
*/
bool COpenGLRenderer::allocateGraphicsMemoryForObject(
	unsigned int *shaderProgramId,
	unsigned int *vertexArrayObjectID,
	GLfloat *vertices, int numVertices,
	GLfloat *normals, int numNormals,
	GLfloat *UVcoords, int numUVCoords,
	unsigned short *indicesVertices, int numIndicesVert,
	unsigned short *indicesNormals, int numIndicesNormals,
	unsigned short *indicesUVCoords, int numIndicesUVCoords)
{
	if (shaderProgramId == NULL || *shaderProgramId == 0)
	{
		cout << "Error: Invalid shader object ID" << endl;
		return false;
	}

	if (vertexArrayObjectID != NULL)
	{
		*vertexArrayObjectID = 0;
	}

	if (vertexArrayObjectID != NULL
		&& *shaderProgramId > 0
		&& vertices != NULL 
		&& normals != NULL
		&& UVcoords != NULL
		&& indicesVertices != NULL
		&& indicesNormals != NULL
		&& indicesUVCoords != NULL)
	{
		GLuint vertexPositionBuffer = 0;
		GLuint normalsPositionBuffer = 0;
		GLuint uvCoordsPositionBuffer = 0;
		GLuint indicesVertexBuffer = 0;

		if (!useShaderProgram(shaderProgramId))
		{
			cout << "ERROR: Cannot use shader program id: " << *shaderProgramId << endl;
			return false;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(*shaderProgramId);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << *shaderProgramId << endl;
			return false;
		}

		// There's no supported way to render multi-indexed buffers in OpenGL, we need to generate a
		// final set of vertices and duplicate vertices for the ones that share more than one normal or UV coord
		int finalNumTriangles = 0;
		int numFaces = numIndicesVert;
		int finalNumVertices = numFaces * 3 * 3; // Number of faces * 3 vertex indices * 3 components (x,y,z)
		int finalNumNormals  = numFaces * 3 * 3; // Number of faces * 3 normal indices * 3 components (x,y,z)
		int finalNumUVCoords = numFaces * 3 * 2; // Number of faces * 3 UV indices * 2 components (x, y)

		GLfloat *finalVertices = new GLfloat[finalNumVertices];
		GLfloat *finalNormals = new GLfloat[finalNumNormals];
		GLfloat *finalUVCoords = new GLfloat[finalNumUVCoords];

		memset(finalVertices, 0, sizeof(GLfloat) * finalNumVertices);
		memset(finalNormals, 0, sizeof(GLfloat) * finalNumNormals);
		memset(finalUVCoords, 0, sizeof(GLfloat) * finalNumUVCoords);

		// Flatten the arrays in the final geometry arrays
		if (!generateRenderGeometry(
			vertices, numVertices,
			normals, numNormals,
			UVcoords, numUVCoords,
			numIndicesVert,
			indicesVertices,
			indicesNormals,
			indicesUVCoords,
			finalVertices, finalNormals, finalUVCoords, &finalNumTriangles))
		{
			if (finalVertices != NULL)
			{
				delete[] finalVertices;
				finalVertices = NULL;
			}
			if (finalNormals != NULL)
			{
				delete[] finalNormals;
				finalNormals = NULL;
			}
			if (finalUVCoords != NULL)
			{
				delete[] finalUVCoords;
				finalUVCoords = NULL;
			}

			return false;
		}

		// Commented out ***, not using indices, we have flattened arrays

		// Create and bind a vertex array object
		*vertexArrayObjectID = (unsigned int)generateVertexArrayObjectID();

		// Generate a buffer for the vertices and set its data
		glGenBuffers(1, &vertexPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
		//*** glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, vertices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * finalNumVertices, finalVertices, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glVertexAttribPointer(shaderProgramWrapper->getPositionAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(shaderProgramWrapper->getPositionAttributeLocation());

		// Generate a buffer for the normals and set its data
		glGenBuffers(1, &normalsPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalsPositionBuffer);
		//*** glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numNormals * 3, normals, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * finalNumNormals, finalNormals, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glVertexAttribPointer(shaderProgramWrapper->getNormalAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(shaderProgramWrapper->getNormalAttributeLocation());
		
		// Generate a buffer for the UV coords and set its data
		glGenBuffers(1, &uvCoordsPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvCoordsPositionBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numUVCoords * 2, UVcoords, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * finalNumUVCoords, finalUVCoords, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glVertexAttribPointer(shaderProgramWrapper->getUVAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(shaderProgramWrapper->getUVAttributeLocation());

		// Generate a buffer for the triangle indices and set its data
		
		//*** glGenBuffers(1, &indicesVertexBuffer);
		//*** glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVertexBuffer);
		//*** glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndicesVert * 3, indicesVertices, GL_STATIC_DRAW);

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//*** glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Delete buffers
		deleteBufferObject(&vertexPositionBuffer);
		deleteBufferObject(&normalsPositionBuffer);
		deleteBufferObject(&uvCoordsPositionBuffer);
		//*** deleteBufferObject(&indicesVertexBuffer);

		glUseProgram(0);

		// Free memory allocated for final render geometry, it's stored in the graphics card now
		if (finalVertices != NULL)
		{
			delete[] finalVertices;
			finalVertices = NULL;
		}
		if (finalNormals != NULL)
		{
			delete[] finalNormals;
			finalNormals = NULL;
		}
		if (finalUVCoords != NULL)
		{
			delete[] finalUVCoords;
			finalUVCoords = NULL;
		}

		return true;
	}

	return false;
}

/*
*/
bool COpenGLRenderer::allocateGraphicsMemoryForObject(
	const unsigned int * const shaderProgramId,
	unsigned int *vertexArrayObjectID,
	GLfloat *vertices, int numVertices,
	unsigned short *indicesVertices, int numIndicesVert)
{
	GLuint vertexPositionBuffer = 0;
	GLuint indicesVertexBuffer = 0;

	if (shaderProgramId == nullptr || *shaderProgramId <= 0)
	{
		cout << "Error: Invalid shader object ID" << endl;
		return false;
	}

	if (vertexArrayObjectID == nullptr
		|| vertices == nullptr 
		|| indicesVertices == nullptr 
		|| !useShaderProgram(shaderProgramId))
	{
		cout << "ERROR: Cannot use shader program id: " << *shaderProgramId << endl;
		return false;
	}
	
	COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(*shaderProgramId);
	if (shaderProgramWrapper == nullptr)
	{
		cout << "ERROR: Could not find shader program wrapper for shader program id: " << *shaderProgramId << endl;
		return false;
	}

	*vertexArrayObjectID = 0;

	// Create and bind a vertex array object
	*vertexArrayObjectID = (unsigned int)generateVertexArrayObjectID();

	// Generate a buffer for the vertices and set its data
	glGenBuffers(1, &vertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, vertices, GL_STATIC_DRAW);
	// Link the vertex position buffer with the shader
	glVertexAttribPointer(shaderProgramWrapper->getPositionAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(shaderProgramWrapper->getPositionAttributeLocation());

	// Generate a buffer for the triangle indices and set its data

	glGenBuffers(1, &indicesVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVertexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndicesVert * 3, indicesVertices, GL_STATIC_DRAW);

	// Unbind vertex array
	glBindVertexArray(0);

	// Unbind buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Delete buffers
	deleteBufferObject(&vertexPositionBuffer);
	deleteBufferObject(&indicesVertexBuffer);

	glUseProgram(0);

	return true;
}

/*
*/
bool COpenGLRenderer::generateRenderGeometry(
	GLfloat *vertices, int numVertices,
	GLfloat *normals, int numNormals,
	GLfloat *UVcoords, int numUVCoords,
	int numFaces,
	unsigned short *indicesVertices, 
	unsigned short *indicesNormals,
	unsigned short *indicesUVCoords,
	GLfloat *finalVertices,
	GLfloat *finalNormals,
	GLfloat *finalUVCoords,
	int *numTriangles)
{
	int currentVertex = 0;
	int currentNormal = 0;
	int currentUV = 0;

	unsigned short vIndices[3];
	unsigned short nIndices[3];
	unsigned short uIndices[3];

	if (numTriangles == NULL
		|| finalVertices == NULL
		|| finalNormals == NULL
		|| finalUVCoords == NULL
		|| indicesVertices == NULL
		|| indicesNormals == NULL
		|| indicesUVCoords == NULL
		|| vertices == NULL
		|| normals == NULL
		|| UVcoords == NULL
		)
	{
		cout << "COpenGLRenderer::generateRenderGeometry() : Invalid pointers" << endl;
		return false;
	}

	*numTriangles = numFaces;

	// Iterate over each face
	for (int i = 0; i < (numFaces * 3); i+=3)
	{
		// Each face has 3 vertex indices
		// Each vertex has 3 components: x, y, z

		// Vertex indices for this face
		vIndices[0] = indicesVertices[i];   // Vertex index 1 
		vIndices[1] = indicesVertices[i+1]; // Vertex index 2
		vIndices[2] = indicesVertices[i+2]; // Vertex index 3

		// Normal indices for this face
		nIndices[0] = indicesNormals[i];    // Normal index 1
		nIndices[1] = indicesNormals[i+1];  // Normal index 2
		nIndices[2] = indicesNormals[i+2];  // Normal index 3

		// UVCoord indices for this face
		uIndices[0] = indicesUVCoords[i];   // UV coord index 1
		uIndices[1] = indicesUVCoords[i+1]; // UV coord index 2
 		uIndices[2] = indicesUVCoords[i+2]; // UV coord index 3

		if (((vIndices[0] * 3) + 2) >= (numVertices * 3)
			|| ((vIndices[1] * 3) + 2) >= (numVertices * 3)
			|| ((vIndices[2] * 3) + 2) >= (numVertices * 3))
		{
			cout << "COpenGLRenderer::generateRenderGeometry() : Invalid vertex indices" << endl;
			return false;
		}

		if (((nIndices[0] * 3) + 2) >= (numNormals * 3)
			|| ((nIndices[1] * 3) + 2) >= (numNormals * 3)
			|| ((nIndices[2] * 3) + 2) >= (numNormals * 3))
		{
			cout << "COpenGLRenderer::generateRenderGeometry() : Invalid normal indices" << endl;
			return false;
		}

		if (((uIndices[0] * 2) + 1) >= (numUVCoords * 2)
			|| ((uIndices[1] * 2) + 1) >= (numUVCoords * 2)
			|| ((uIndices[2] * 2) + 1) >= (numUVCoords * 2))
		{
			cout << "COpenGLRenderer::generateRenderGeometry() : Invalid UV coord indices" << endl;
			return false;
		}

		// For each vertex, copy its 3 components, finalVertices is an array of floats

		finalVertices[currentVertex + 0] = vertices[(vIndices[0] * 3) + 0]; // Vertex1 X
		finalVertices[currentVertex + 1] = vertices[(vIndices[0] * 3) + 1]; // Vertex1 Y
		finalVertices[currentVertex + 2] = vertices[(vIndices[0] * 3) + 2]; // Vertex1 Z
		currentVertex += 3;
		finalVertices[currentVertex + 0] = vertices[(vIndices[1] * 3) + 0]; // Vertex2 X
		finalVertices[currentVertex + 1] = vertices[(vIndices[1] * 3) + 1]; // Vertex2 Y
		finalVertices[currentVertex + 2] = vertices[(vIndices[1] * 3) + 2]; // Vertex2 Z
		currentVertex += 3;
		finalVertices[currentVertex + 0] = vertices[(vIndices[2] * 3) + 0]; // Vertex3 X
		finalVertices[currentVertex + 1] = vertices[(vIndices[2] * 3) + 1]; // Vertex3 Y
		finalVertices[currentVertex + 2] = vertices[(vIndices[2] * 3) + 2]; // Vertex3 Z
		currentVertex += 3;

		finalNormals[currentNormal + 0] = normals[(nIndices[0] * 3) + 0]; // Normal 1 X
		finalNormals[currentNormal + 1] = normals[(nIndices[0] * 3) + 1]; // Normal 1 Y
		finalNormals[currentNormal + 2] = normals[(nIndices[0] * 3) + 2]; // Normal 1 Z
		currentNormal += 3;
		finalNormals[currentNormal + 0] = normals[(nIndices[1] * 3) + 0]; // Normal 2 X
		finalNormals[currentNormal + 1] = normals[(nIndices[1] * 3) + 1]; // Normal 2 Y
		finalNormals[currentNormal + 2] = normals[(nIndices[1] * 3) + 2]; // Normal 2 Z
		currentNormal += 3;
		finalNormals[currentNormal + 0] = normals[(nIndices[2] * 3) + 0]; // Normal 3 X
		finalNormals[currentNormal + 1] = normals[(nIndices[2] * 3) + 1]; // Normal 3 Y
		finalNormals[currentNormal + 2] = normals[(nIndices[2] * 3) + 2]; // Normal 3 Z
		currentNormal += 3;

		finalUVCoords[currentUV + 0] = UVcoords[(uIndices[0] * 2) + 0]; // UV coord 1 X
		finalUVCoords[currentUV + 1] = UVcoords[(uIndices[0] * 2) + 1]; // UV coord 1 Y
		currentUV += 2;
		finalUVCoords[currentUV + 0] = UVcoords[(uIndices[1] * 2) + 0]; // UV coord 2 X
		finalUVCoords[currentUV + 1] = UVcoords[(uIndices[1] * 2) + 1]; // UV coord 2 Y
		currentUV += 2;
		finalUVCoords[currentUV + 0] = UVcoords[(uIndices[2] * 2) + 0]; // UV coord 3 X
		finalUVCoords[currentUV + 1] = UVcoords[(uIndices[2] * 2) + 1]; // UV coord 3 Y
		currentUV += 2;
	}

	return true;
}

/*
*/
GLenum COpenGLRenderer::primitiveModeToGLEnum(EPRIMITIVE_MODE mode) const
{
	GLenum result = GL_TRIANGLES;

	switch (mode)
	{
	case POINTS:
		result = GL_POINTS;
		break;
	case LINES:
		result = GL_LINES;
		break;
	case LINE_STRIP:
		result = GL_LINE_STRIP;
		break;
	case LINE_LOOP:
		result = GL_LINE_LOOP;
		break;
	case TRIANGLES:
		result = GL_TRIANGLES;
		break;
	case TRIANGLE_STRIP:
		result = GL_TRIANGLE_STRIP;
		break;
	case TRIANGLE_FAN:
		result = GL_TRIANGLE_FAN;
		break;
	}

	return result;
}

/*
*/
bool COpenGLRenderer::renderWireframeObject(
	unsigned int *shaderProgramId,
	unsigned int *vertexArrayObjectId,
	int numFaces,
	GLfloat *objectColor,
	MathHelper::Matrix4 *objectTransformation)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int noTexture = 0;

	return renderObject(
		shaderProgramId,
		vertexArrayObjectId,
		&noTexture, // no texture
		numFaces,
		objectColor,
		objectTransformation,
		TRIANGLES,
		true);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/*
*/
bool COpenGLRenderer::renderObject(
	unsigned int *shaderProgramId,
	unsigned int *vertexArrayObjectId,
	unsigned int *textureObjectId,
	int numFaces,
	GLfloat *objectColor,
	MathHelper::Matrix4 *objectTransformation,
	EPRIMITIVE_MODE mode,
	bool drawIndexedPrimitives)
{
	/*
	if (m_frameBufferWidth > 0
		&& m_frameBufferHeight > 0
		&& vertexArrayObjectId != NULL
		&& *vertexArrayObjectId > 0
		&& numFaces > 0
		&& objectColor != NULL
		&& !m_OpenGLError)
	{
		if (!useShaderProgram(shaderProgramId))
		{
			cout << "ERROR: Cannot use shader program id: " << *shaderProgramId << endl;
			m_OpenGLError = true;
			glUseProgram(0);
			return false;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(*shaderProgramId);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << *shaderProgramId << endl;
			return false;
		}

		GLenum drawingPrimitiveMode = primitiveModeToGLEnum(mode);

		// Bind vertex array object for this 3D object
		glBindVertexArray((GLuint)*vertexArrayObjectId);

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time
		if (shaderProgramWrapper->getModelMatrixUniformLocation() >= 0)
		{
			if (objectTransformation == NULL)
			{
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationY(0.0f);
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(modelMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(objectTransformation->m[0][0]));
			}
		}

		if (shaderProgramWrapper->getViewMatrixUniformLocation() >= 0)
		{
			MathHelper::Matrix4 viewMatrix = MathHelper::SimpleViewMatrix(m_cameraDistance);
			glUniformMatrix4fv(shaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(viewMatrix.m[0][0]));
		}

		if (shaderProgramWrapper->getProjectionMatrixUniformLocation() >= 0)
		{
			MathHelper::Matrix4 projectionMatrix = MathHelper::SimpleProjectionMatrix(float(m_frameBufferWidth) / float(m_frameBufferHeight));
			glUniformMatrix4fv(shaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(projectionMatrix.m[0][0]));
		}

		if (shaderProgramWrapper->getColorUniformLocation() >= 0)
		{
			glUniform3f(shaderProgramWrapper->getColorUniformLocation(), objectColor[0], objectColor[1], objectColor[2]);
		}

		// Set the texture sampler uniform
		if (textureObjectId != nullptr && shaderProgramWrapper->getTextureSamplerUniformLocation() >= 0 && *textureObjectId > 0)
		{
			// DO NOT CALL glEnable(GL_TEXTURE_2D) OR OPENGL WILL RETURN AN "1280" ERROR
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *textureObjectId);
			glUniform1i(shaderProgramWrapper->getTextureSamplerUniformLocation(), 0);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (drawIndexedPrimitives)
		{
			glDrawElements(
				drawingPrimitiveMode,
				numFaces * 3,			// Number of indices
				GL_UNSIGNED_SHORT,		// Data type
				0);

			// Check for OpenGL errors
			m_OpenGLError = checkOpenGLError("glDrawElements()");
		}
		else
		{
			// Draw
			glDrawArrays(
				drawingPrimitiveMode,
				0,
				numFaces * 3 // 3 indices per face
			);

			// Check for OpenGL errors
			m_OpenGLError = checkOpenGLError("glDrawArrays()");
		}

		// Unbind vertex array object
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);

		if (!m_OpenGLError)
			return true;
	}

	return false; */

	return renderObject(
		shaderProgramId,
		vertexArrayObjectId,
		textureObjectId,
		numFaces,
		objectColor,
		objectTransformation,
		nullptr,
		nullptr,
		mode,
		drawIndexedPrimitives);
}

/*
*/
bool COpenGLRenderer::renderObject(
	unsigned int *shaderProgramId,
	unsigned int *vertexArrayObjectId,
	unsigned int *textureObjectId,
	int numFaces,
	GLfloat *objectColor,
	MathHelper::Matrix4 *modelMatrix,
	MathHelper::Matrix4 *viewMatrix,
	MathHelper::Matrix4 *projectionMatrix,
	EPRIMITIVE_MODE mode,
	bool drawIndexedPrimitives)
{
	if (m_frameBufferWidth > 0
		&& m_frameBufferHeight > 0
		&& vertexArrayObjectId != nullptr
		&& *vertexArrayObjectId > 0
		&& numFaces > 0
		&& objectColor != nullptr 
		&& m_activeShaderProgram != nullptr
		&& m_activeShaderProgramWrapper != nullptr
		&& !m_OpenGLError)
	{
		if (!useShaderProgram(shaderProgramId))
		{
			cout << "ERROR: Cannot use shader program id: " << *shaderProgramId << endl;
			m_OpenGLError = true;
			glUseProgram(0);
			return false;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(*shaderProgramId);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << *shaderProgramId << endl;
			return false;
		}

		GLenum drawingPrimitiveMode = primitiveModeToGLEnum(mode);

		// Bind vertex array object for this 3D object
		glBindVertexArray((GLuint)*vertexArrayObjectId);

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time
		if (shaderProgramWrapper->getModelMatrixUniformLocation() >= 0)
		{
			if (modelMatrix == nullptr)
			{
				MathHelper::Matrix4 simpleModelMatrix = MathHelper::SimpleModelMatrixRotationY(0.0f);
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(simpleModelMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(modelMatrix->m[0][0]));
			}
		}

		if (shaderProgramWrapper->getViewMatrixUniformLocation() >= 0)
		{
			if (viewMatrix == nullptr)
			{
				MathHelper::Matrix4 simpleViewMatrix = MathHelper::SimpleViewMatrix(m_cameraDistance);
				glUniformMatrix4fv(shaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(simpleViewMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(shaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(viewMatrix->m[0][0]));
			}
		}

		if (shaderProgramWrapper->getProjectionMatrixUniformLocation() >= 0)
		{
			if (projectionMatrix == nullptr)
			{
				MathHelper::Matrix4 simpleProjectionMatrix = MathHelper::SimpleProjectionMatrix(float(m_frameBufferWidth) / float(m_frameBufferHeight));
				glUniformMatrix4fv(shaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(simpleProjectionMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(shaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(projectionMatrix->m[0][0]));
			}
		}

		if (shaderProgramWrapper->getColorUniformLocation() >= 0)
		{
			glUniform3f(shaderProgramWrapper->getColorUniformLocation(), objectColor[0], objectColor[1], objectColor[2]);
		}

		// Set the texture sampler uniform
		if (textureObjectId != nullptr && shaderProgramWrapper->getTextureSamplerUniformLocation() >= 0 && *textureObjectId > 0)
		{
			// DO NOT CALL glEnable(GL_TEXTURE_2D) OR OPENGL WILL RETURN AN "1280" ERROR
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *textureObjectId);
			glUniform1i(shaderProgramWrapper->getTextureSamplerUniformLocation(), 0);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (drawIndexedPrimitives)
		{
			glDrawElements(
				drawingPrimitiveMode,
				numFaces * 3,			// Number of indices
				GL_UNSIGNED_SHORT,		// Data type
				0);

			// Check for OpenGL errors
			m_OpenGLError = checkOpenGLError("glDrawElements()");
		}
		else
		{
			// Draw
			glDrawArrays(
				drawingPrimitiveMode,
				0,
				numFaces * 3 // 3 indices per face
			);

			// Check for OpenGL errors
			m_OpenGLError = checkOpenGLError("glDrawArrays()");
		}

		// Unbind vertex array object
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);

		if (!m_OpenGLError)
			return true;
	}

	return false;
}

/*
 * Calling this method assumes the following methods have been called:
 * ------------------------------------------------------------------
 *
 * 1 - useShaderProgram( ) 
 * 2 - setCurrentVertexArrayObjectID( ) 
 * 3 - setCurrentShaderObjectColor( )
 * 4 - setCurrentShaderViewMatrix( )
 * 5 - setCurrentShaderProjectionMatrix( )
 * 6 - setCurrentShaderTexture( ) (if applicable)
 * 7 - setCurrentShaderAmbientLight( )
 *
 */
bool COpenGLRenderer::renderObjectNew(
	int numFaces,
	MathHelper::Matrix4 *modelMatrix,
	EPRIMITIVE_MODE mode,
	bool drawIndexedPrimitives)
{
	if (numFaces > 0
		&& m_activeShaderProgram != nullptr
		&& m_activeShaderProgramWrapper != nullptr
		&& !m_OpenGLError)
	{
		GLenum drawingPrimitiveMode = primitiveModeToGLEnum(mode);
		int modelMatrixUniformLocation = m_activeShaderProgramWrapper->getModelMatrixUniformLocation();

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time
		if (modelMatrixUniformLocation >= 0)
		{
			if (modelMatrix == nullptr)
			{
				MathHelper::Matrix4 simpleModelMatrix = MathHelper::SimpleModelMatrixRotationY(0.0f);
				glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &(simpleModelMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &(modelMatrix->m[0][0]));
			}
		}

		if (drawIndexedPrimitives)
		{
			glDrawElements(
				drawingPrimitiveMode,
				numFaces * 3,			// Number of indices
				GL_UNSIGNED_SHORT,		// Data type
				0);

			// Check for OpenGL errors
			m_OpenGLError = checkOpenGLError("glDrawElements()");
		}
		else
		{
			// Draw
			glDrawArrays(
				drawingPrimitiveMode,
				0,
				numFaces * 3 // 3 indices per face
			);

			// Check for OpenGL errors
			m_OpenGLError = checkOpenGLError("glDrawArrays()");
		}

		if (!m_OpenGLError)
		{
			return true;
		}
	}

	return false;
}

/*
*/
bool COpenGLRenderer::renderMenuItem(
	unsigned int *shaderProgramId,
	unsigned int *textureObjectId,
	unsigned int *vertexArrayObjectId, 
	GLfloat *menuItemColor
)
{
	if (shaderProgramId != NULL
		&& textureObjectId != NULL
		&& vertexArrayObjectId != NULL
		&& *vertexArrayObjectId > 0
		&& menuItemColor != NULL
		&& m_activeShaderProgram != nullptr
		&& m_activeShaderProgramWrapper != nullptr
		&& !m_OpenGLError)
	{
		if (!useShaderProgram(shaderProgramId))
		{
			cout << "ERROR: Cannot use shader program id: " << *shaderProgramId << endl;
			m_OpenGLError = true;
			glUseProgram(0);
			return false;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(*shaderProgramId);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << *shaderProgramId << endl;
			return false;
		}

		// Bind vertex array object
		glBindVertexArray(*vertexArrayObjectId);

		// Pass the color value to the uniform
		if (shaderProgramWrapper->getColorUniformLocation() >= 0)
		{
			glUniform3f(shaderProgramWrapper->getColorUniformLocation(), menuItemColor[0], menuItemColor[1], menuItemColor[2]);
		}
		// Set the texture sampler uniform
		if (shaderProgramWrapper->getTextureSamplerUniformLocation() >= 0 && *textureObjectId >= 0)
		{
			// DO NOT CALL glEnable(GL_TEXTURE_2D) OR OPENGL WILL RETURN AN "1282" ERROR
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *textureObjectId);
			glUniform1i(shaderProgramWrapper->getTextureSamplerUniformLocation(), 0);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Draw 
		glDrawElements(
			GL_TRIANGLES,      // Triangles
			6,                 // Number of indices: 1 face, 2 two triangles per face, 3 indices per triangle)
			GL_UNSIGNED_SHORT, // Data type
			0);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");
		if (m_OpenGLError)
			return false;

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);

		return true;
	}

	return false;
}

/*
*/
void COpenGLRenderer::initializeColorCube()
{
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_3D_OBJECT_COLOR,   wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_3D_OBJECT_COLOR, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_3D_OBJECT_COLOR << endl;
		cout << "  " << FRAGMENT_SHADER_3D_OBJECT_COLOR << endl;
		return;
	}

	if (createShaderProgram(
		SHADER_PROGRAM_COLOR_CUBE,
		&m_testCubeShaderProgramID,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()
	))
	{
		GLuint vertexPositionBuffer = 0;
		GLuint vertexColorBuffer = 0;
		GLuint indicesVertexBuffer = 0;

		if (!useShaderProgram(&m_testCubeShaderProgramID))
		{
			cout << "ERROR: Cannot use shader program id: " << m_testCubeShaderProgramID << endl;
			return;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(m_testCubeShaderProgramID);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << m_testCubeShaderProgramID << endl;
			return;
		}

		// Create and bind a vertex array object
		m_testCubeVAOID = generateVertexArrayObjectID();

		// Test cube geometry.
		GLfloat vertexPositions[] =
		{
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f
		};

		// Generate a buffer for the vertices and set its data
		glGenBuffers(1, &vertexPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glEnableVertexAttribArray(shaderProgramWrapper->getPositionAttributeLocation());
		glVertexAttribPointer(shaderProgramWrapper->getPositionAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLfloat vertexColors[] =
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f
		};

		// Generate a buffer for the colors and set its data
		glGenBuffers(1, &vertexColorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		// Link the color buffer with the shader
		glVertexAttribPointer(shaderProgramWrapper->getColorAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(shaderProgramWrapper->getColorAttributeLocation());

		short indices[] =
		{
			0, 1, 2, // -x
			1, 3, 2,

			4, 6, 5, // +x
			5, 6, 7,

			0, 5, 1, // -y
			0, 4, 5,

			2, 7, 6, // +y
			2, 3, 7,

			0, 6, 4, // -z
			0, 2, 6,

			1, 7, 3, // +z
			1, 5, 7
		};

		// Generate a buffer for the triangle indices and set its data
		glGenBuffers(1, &indicesVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVertexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Unbind vertex array
		glBindVertexArray(0);

		// Delete buffers
		deleteBufferObject(&vertexPositionBuffer);
		deleteBufferObject(&vertexColorBuffer);
		deleteBufferObject(&indicesVertexBuffer);

		glUseProgram(0);
	}
}

/*
*/
void COpenGLRenderer::initializeTexturedCube()
{
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_MC_CUBE, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_MC_CUBE, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_MC_CUBE << endl;
		cout << "  " << FRAGMENT_SHADER_MC_CUBE << endl;
		return;
	}

	if (createShaderProgram(
		SHADER_PROGRAM_TEXTURED_CUBE,
		&m_mCCubeShaderProgramID,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()
	))
	{
		GLuint mcCubeVertexPositionBuffer = 0;
		GLuint mcCubeVertexColorBuffer = 0;
		GLuint mcCubeVertexUVBuffer = 0;
		GLuint mcCubeIndexBuffer = 0;

		if (!useShaderProgram(&m_mCCubeShaderProgramID))
		{
			cout << "ERROR: Cannot use shader program id: " << m_mCCubeShaderProgramID << endl;
			return;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(m_mCCubeShaderProgramID);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << m_mCCubeShaderProgramID << endl;
			return;
		}

		// Create and bind a vertex array object
		m_mCCubeVAOID = generateVertexArrayObjectID();

		// Test cube geometry.
		GLfloat vertexPositions[] =
		{
			-1.0f,  1.0f, -1.0f,  // -x, +y, -z TOP LEFT, BACK      #0
			-1.0f,  1.0f,  1.0f,  // -x, +y, +z TOP LEFT, FRONT     #1

			 1.0f,  1.0f, -1.0f,  // +x, +y, -z TOP RIGHT, BACK     #2
			 1.0f,  1.0f,  1.0f,  // +x, +y, +z TOP RIGHT, FRONT    #3

			-1.0f, -1.0f, -1.0f,  // -x, -y, -z BOTTOM LEFT, BACK   #4
			-1.0f, -1.0f,  1.0f,  // -x, -y, +z BOTTOM LEFT, FRONT  #5

			 1.0f, -1.0f, -1.0f,  // +x, -y, -z BOTTOM RIGHT, BACK  #6
			 1.0f, -1.0f,  1.0f,  // +x, -y, +z BOTTOM RIGHT, FRONT #7

			// DUPLICATE VERTICES
			// -------------------
			-1.0f, -1.0f, -1.0f,  // -x, -y, -z BOTTOM LEFT, BACK   #8
			 1.0f, -1.0f, -1.0f,  // +x, -y, -z BOTTOM RIGHT, BACK  #9

		    -1.0f, -1.0f,  1.0f,  // -x, -y, +z BOTTOM LEFT, FRONT  #10
			 1.0f, -1.0f,  1.0f,  // +x, -y, +z BOTTOM RIGHT, FRONT #11

            -1.0f, -1.0f, -1.0f,  // -x, -y, -z BOTTOM LEFT, BACK   #12
			-1.0f, -1.0f,  1.0f   // -x, -y, +z BOTTOM LEFT, FRONT  #13
		};

		// Generate a buffer for the vertices and set its data
		glGenBuffers(1, &mcCubeVertexPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mcCubeVertexPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glEnableVertexAttribArray(shaderProgramWrapper->getPositionAttributeLocation());
		glVertexAttribPointer(shaderProgramWrapper->getPositionAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLfloat vertexColors[] =
		{
			1.0f, 1.0f, 1.0f, // -x, +y, -z TOP LEFT, BACK      #0
			1.0f, 1.0f, 1.0f, // -x, +y, +z TOP LEFT, FRONT     #1

			1.0f, 1.0f, 1.0f, // +x, +y, -z TOP RIGHT, BACK     #2
			1.0f, 1.0f, 1.0f, // +x, +y, +z TOP RIGHT, FRONT    #3

			1.0f, 1.0f, 1.0f, // -x, -y, -z BOTTOM LEFT, BACK   #4
			1.0f, 1.0f, 1.0f, // -x, -y, +z BOTTOM LEFT, FRONT  #5
			
			1.0f, 1.0f, 1.0f, // +x, -y, -z BOTTOM RIGHT, BACK  #6
			1.0f, 1.0f, 1.0f, // +x, -y, +z BOTTOM RIGHT, FRONT #7

            // DUPLICATE VERTICES
			// -------------------
			1.0f, 1.0f, 1.0f, // -x, -y, -z BOTTOM LEFT, BACK   #8
			1.0f, 1.0f, 1.0f, // +x, -y, -z BOTTOM RIGHT, BACK  #9

			1.0f, 1.0f, 1.0f, // -x, -y, +z BOTTOM LEFT, FRONT  #10
			1.0f, 1.0f, 1.0f, // +x, -y, +z BOTTOM RIGHT, FRONT #11

			1.0f, 1.0f, 1.0f, // -x, -y, -z BOTTOM LEFT, BACK   #12
			1.0f, 1.0f, 1.0f  // +x, -y, +z BOTTOM RIGHT, FRONT #13
		};

		// Generate a buffer for the colors and set its data
		glGenBuffers(1, &mcCubeVertexColorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mcCubeVertexColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		// Link the color buffer with the shader
		glVertexAttribPointer(shaderProgramWrapper->getColorAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(shaderProgramWrapper->getColorAttributeLocation());

		GLfloat vertexUVs[] =
		{
			0.25f, 0.33f, // TOP LEFT, BACK
			0.25f, 0.66f, // TOP LEFT, FRONT
			0.50f, 0.33f, // TOP RIGHT, BACK
			0.50f, 0.66f, // TOP RIGHT, FRONT
			0.00f, 0.33f, // BOTTOM LEFT, BACK 
			0.00f, 0.66f, // BOTTOM LEFT, FRONT
			0.75f, 0.33f, // BOTTOM RIGHT, BACK
			0.75f, 0.66f, // BOTTOM RIGHT, FRONT

			// DUPLICATES
			// ----------
			0.25f, 0.0f,  // BOTTOM LEFT, BACK
			0.50f, 0.0f,  // BOTTOM RIGHT, BACK
			0.25f, 1.0f,  // BOTTOM LEFT, FRONT
			0.50f, 1.0f,  // BOTTOM RIGHT, FRONT
			1.0f,  0.33f, // BOTTOM LEFT, BACK
			1.0f,  0.66f  // BOTTOM LEFT, FRONT
		};

		// Generate a buffer for the UVs and set its data
		glGenBuffers(1, &mcCubeVertexUVBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mcCubeVertexUVBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexUVs), vertexUVs, GL_STATIC_DRAW);
		// Link the UV buffer with the shader
		glVertexAttribPointer(shaderProgramWrapper->getUVAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(shaderProgramWrapper->getUVAttributeLocation());

		short indices[] =
		{
			0, 1, 2, // TOP #1
			2, 1, 3, // TOP #2

			0, 4, 1, // LEFT #1
			1, 4, 5, // LEFT #2

			3, 7, 2, // RIGHT #1
			2, 7, 6, // RIGHT #2

			2, 9, 0, // BACK #1
			0, 9, 8, // BACK #2

			1, 10, 3,// FRONT #1
			3, 10, 11,//FRONT #2

			13, 12, 7,//BOTTOM #1
			7, 12, 6  //BOTTOM #2
		};

		// Generate a buffer for the triangle indices and set its data
		glGenBuffers(1, &mcCubeIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mcCubeIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Delete buffers
		deleteBufferObject(&mcCubeVertexPositionBuffer);
		deleteBufferObject(&mcCubeVertexColorBuffer);
		deleteBufferObject(&mcCubeVertexUVBuffer);
		deleteBufferObject(&mcCubeIndexBuffer);

		glUseProgram(0);
	}
}

/*
*/
bool COpenGLRenderer::allocateGraphicsMemoryForMenuItem(
	float topX, 
	float topY,
	float menuItemHeight,
	float *uvCoords,
	unsigned int *shaderProgramId,
	unsigned int *vertexArrayObjectID)
{
	// Use shader program
	if (shaderProgramId != NULL
		&& vertexArrayObjectID != NULL)
	{
		if (!useShaderProgram(shaderProgramId))
		{
			cout << "ERROR: Cannot use shader program id: " << *shaderProgramId << endl;
			return false;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(*shaderProgramId);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << *shaderProgramId << endl;
			return false;
		}

		// Create and bind a vertex array object
		*vertexArrayObjectID = generateVertexArrayObjectID();

		// Single quad representing a menu item
		GLfloat vertexPositions[] =
		{
			-1.0f + topX,  topY - menuItemHeight,  0.0f,  // 0: -x, -y
			-1.0f + topX,  topY,                   0.0f,  // 1: -x, +y
			 1.0f - topX,  topY - menuItemHeight,  0.0f,  // 2: +x, -y
			 1.0f - topX,  topY,                   0.0f   // 3: +x, +y
		};

		// Generate a buffer for the vertices and set its data
		GLuint vertexPosBuffer = 0;
		glGenBuffers(1, &vertexPosBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPosBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glEnableVertexAttribArray(shaderProgramWrapper->getPositionAttributeLocation());
		glVertexAttribPointer(shaderProgramWrapper->getPositionAttributeLocation(), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("COpenGLRenderer::allocateGraphicsMemoryForMenuItem() pos buffer initialization");

		GLfloat uvData[] =
		{
			uvCoords[4], uvCoords[5], // -x, -y
			uvCoords[0], uvCoords[1], // -x, +y
			uvCoords[6], uvCoords[7], // +x, -y
			uvCoords[2], uvCoords[3]  // +x, +y
		};

		// Generate a buffer for the UV data and fill it
		GLuint uvBuffer = 0;
		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvData), uvData, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glEnableVertexAttribArray(shaderProgramWrapper->getUVAttributeLocation());
		glVertexAttribPointer(shaderProgramWrapper->getUVAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		m_OpenGLError = checkOpenGLError("COpenGLRenderer::allocateGraphicsMemoryForMenuItem() uv buffer initialization");

		short indices[] =
		{
			0, 3, 1, // +z
			0, 2, 3
		};

		// Generate a buffer for the triangle indices and set its data
		GLuint indexBuffer = 0;
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		m_OpenGLError = checkOpenGLError("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);");

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Delete buffers
		deleteBufferObject(&vertexPosBuffer);
		deleteBufferObject(&uvBuffer);
		deleteBufferObject(&indexBuffer);

		glUseProgram(0);

		return true;
	}

	return false;
}

/*
*/
void COpenGLRenderer::renderColorCube(MathHelper::Matrix4 *objectTransformation)
{
	if (m_frameBufferWidth > 0
		&& m_frameBufferHeight > 0
		&& m_activeShaderProgram != nullptr
		&& m_activeShaderProgramWrapper != nullptr
		&& !m_OpenGLError)
	{
		if (!useShaderProgram(&m_testCubeShaderProgramID))
		{
			cout << "ERROR: Cannot use shader program id: " << m_testCubeShaderProgramID << endl;
			m_OpenGLError = true;
			glUseProgram(0);
			return;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(m_testCubeShaderProgramID);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << m_testCubeShaderProgramID << endl;
			return;
		}

		// BIND VERTEX ARRAY OBJECT !
		// ============================================================================================================
		glBindVertexArray(m_testCubeVAOID);

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time

		if (shaderProgramWrapper->getModelMatrixUniformLocation() >= 0)
		{
			if (objectTransformation == NULL)
			{
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationY(0.0f);
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(modelMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(objectTransformation->m[0][0]));
			}
		}

		if (shaderProgramWrapper->getViewMatrixUniformLocation() >= 0)
		{
			MathHelper::Matrix4 viewMatrix = MathHelper::SimpleViewMatrix(m_cameraDistance);
			glUniformMatrix4fv(shaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(viewMatrix.m[0][0]));
		}

		if (shaderProgramWrapper->getProjectionMatrixUniformLocation() >= 0)
		{
			MathHelper::Matrix4 projectionMatrix = MathHelper::SimpleProjectionMatrix(float(m_frameBufferWidth) / float(m_frameBufferHeight));
			glUniformMatrix4fv(shaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(projectionMatrix.m[0][0]));
		}

		// ====== DRAW ================================================================================================
		
		// Draw 
		glDrawElements(
			GL_TRIANGLES,      // Triangles
			(6 * 2) * 3,       // Number of indices: 36 indices (six faces, two triangles per face, 3 indices per triangle)
			GL_UNSIGNED_SHORT, // Data type
			0);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");
		if (m_OpenGLError)
			return;		

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);
	}
}

/*
*/
void COpenGLRenderer::renderColorCube(
	MathHelper::Matrix4 *modelMatrix,
	MathHelper::Matrix4 *viewMatrix,
	MathHelper::Matrix4 *projectionMatrix
)
{
	if (!m_OpenGLError
		&& modelMatrix != nullptr
		&& viewMatrix != nullptr 
		&& projectionMatrix != nullptr
		&& m_activeShaderProgram != nullptr
		&& m_activeShaderProgramWrapper != nullptr)
	{
		if (!useShaderProgram(&m_testCubeShaderProgramID))
		{
			cout << "ERROR: Cannot use shader program id: " << m_testCubeShaderProgramID << endl;
			m_OpenGLError = true;
			glUseProgram(0);
			return;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(m_testCubeShaderProgramID);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << m_testCubeShaderProgramID << endl;
			return;
		}

		// BIND VERTEX ARRAY OBJECT !
		// ============================================================================================================
		glBindVertexArray(m_testCubeVAOID);

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time

		if (shaderProgramWrapper->getModelMatrixUniformLocation() >= 0)
		{
			glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(modelMatrix->m[0][0]));
		}

		if (shaderProgramWrapper->getViewMatrixUniformLocation() >= 0)
		{
			glUniformMatrix4fv(shaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(viewMatrix->m[0][0]));
		}

		if (shaderProgramWrapper->getProjectionMatrixUniformLocation() >= 0)
		{
			glUniformMatrix4fv(shaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(projectionMatrix->m[0][0]));
		}

		// ====== DRAW ================================================================================================

		// Draw 
		glDrawElements(
			GL_TRIANGLES,      // Triangles
			(6 * 2) * 3,       // Number of indices: 36 indices (six faces, two triangles per face, 3 indices per triangle)
			GL_UNSIGNED_SHORT, // Data type
			0);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");
		if (m_OpenGLError)
			return;

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);
	}
}

/*
*/
void COpenGLRenderer::renderTexturedCube(unsigned int cubeTextureID, MathHelper::Matrix4 *objectTransformation)
{
	if (m_frameBufferWidth > 0
		&& m_frameBufferHeight > 0
		&& !m_OpenGLError
		&& m_activeShaderProgram != nullptr
		&& m_activeShaderProgramWrapper != nullptr)
	{
		if (!useShaderProgram(&m_mCCubeShaderProgramID))
		{
			cout << "ERROR: Cannot use shader program id: " << m_mCCubeShaderProgramID << endl;
			m_OpenGLError = true;
			glUseProgram(0);
			return;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(m_mCCubeShaderProgramID);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << m_mCCubeShaderProgramID << endl;
			return;
		}

		// BIND VERTEX ARRAY OBJECT !
		// ============================================================================================================
		glBindVertexArray(m_mCCubeVAOID);

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time

		if (shaderProgramWrapper->getModelMatrixUniformLocation() >= 0)
		{
			if (objectTransformation == NULL)
			{
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationY(0.0f);
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(modelMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(objectTransformation->m[0][0]));
			}
		}

		if (shaderProgramWrapper->getViewMatrixUniformLocation() >= 0)
		{
			MathHelper::Matrix4 viewMatrix = MathHelper::SimpleViewMatrix(m_cameraDistance);
			glUniformMatrix4fv(shaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(viewMatrix.m[0][0]));
		}

		if (shaderProgramWrapper->getProjectionMatrixUniformLocation() >= 0)
		{
			MathHelper::Matrix4 projectionMatrix = MathHelper::SimpleProjectionMatrix(float(m_frameBufferWidth) / float(m_frameBufferHeight));
			glUniformMatrix4fv(shaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(projectionMatrix.m[0][0]));
		}

		// Set the texture sampler uniform
		if (shaderProgramWrapper->getTextureSamplerUniformLocation() >= 0 && cubeTextureID > 0)
		{
			// DO NOT CALL glEnable(GL_TEXTURE_2D) OR OPENGL WILL RETURN AN "1280" ERROR
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cubeTextureID);
			glUniform1i(shaderProgramWrapper->getTextureSamplerUniformLocation(), 0);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// ====== DRAW ================================================================================================

		// Draw 
		glDrawElements(
			GL_TRIANGLES,      // Triangles
			(6 * 2) * 3,       // Number of indices: 36 indices (six faces, two triangles per face, 3 indices per triangle)
			GL_UNSIGNED_SHORT, // Data type
			0);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");
		if (m_OpenGLError)
			return;

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);
	}
}

/*
*/
void COpenGLRenderer::renderTexturedCube(
	unsigned int cubeTextureID, 
	MathHelper::Matrix4 *modelMatrix,
	MathHelper::Matrix4 *viewMatrix,
	MathHelper::Matrix4 *projectionMatrix)
{
	if (!m_OpenGLError
		&& modelMatrix != nullptr
		&& viewMatrix != nullptr
		&& projectionMatrix != nullptr
		&& cubeTextureID > 0
		&& m_activeShaderProgram != nullptr
		&& m_activeShaderProgramWrapper != nullptr
		)
	{
		if (!useShaderProgram(&m_mCCubeShaderProgramID))
		{
			cout << "ERROR: Cannot use shader program id: " << m_mCCubeShaderProgramID << endl;
			m_OpenGLError = true;
			glUseProgram(0);
			return;
		}

		COpenGLShaderProgram* shaderProgramWrapper = getShaderProgramWrapper(m_mCCubeShaderProgramID);
		if (shaderProgramWrapper == nullptr)
		{
			cout << "ERROR: Could not find shader program wrapper for shader program id: " << m_mCCubeShaderProgramID << endl;
			return;
		}

		// BIND VERTEX ARRAY OBJECT !
		// ============================================================================================================
		glBindVertexArray(m_mCCubeVAOID);

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time

		if (shaderProgramWrapper->getModelMatrixUniformLocation() >= 0)
		{
			glUniformMatrix4fv(shaderProgramWrapper->getModelMatrixUniformLocation(), 1, GL_FALSE, &(modelMatrix->m[0][0]));
		}

		if (shaderProgramWrapper->getViewMatrixUniformLocation() >= 0)
		{
			glUniformMatrix4fv(shaderProgramWrapper->getViewMatrixUniformLocation(), 1, GL_FALSE, &(viewMatrix->m[0][0]));
		}

		if (shaderProgramWrapper->getProjectionMatrixUniformLocation() >= 0)
		{
			glUniformMatrix4fv(shaderProgramWrapper->getProjectionMatrixUniformLocation(), 1, GL_FALSE, &(projectionMatrix->m[0][0]));
		}

		// Set the texture sampler uniform
		if (shaderProgramWrapper->getTextureSamplerUniformLocation() >= 0 && cubeTextureID > 0)
		{
			// DO NOT CALL glEnable(GL_TEXTURE_2D) OR OPENGL WILL RETURN AN "1280" ERROR
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cubeTextureID);
			glUniform1i(shaderProgramWrapper->getTextureSamplerUniformLocation(), 0);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// ====== DRAW ================================================================================================

		// Draw 
		glDrawElements(
			GL_TRIANGLES,      // Triangles
			(6 * 2) * 3,       // Number of indices: 36 indices (six faces, two triangles per face, 3 indices per triangle)
			GL_UNSIGNED_SHORT, // Data type
			0);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");
		if (m_OpenGLError)
			return;

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);
	}
}

/*
*/
bool COpenGLRenderer::checkOpenGLError(char *operationAttempted)
{
	bool errorDetected = false;

	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error on " <<  operationAttempted << ": " << err << endl;
		errorDetected = true;
	}

	return errorDetected;
}

/*
*/
void COpenGLRenderer::simpleCameraZoom(float direction)
{
	m_cameraDistance += (direction * MOVE_CAMERA_DELTA);

	if (m_cameraDistance > MAX_CAMERA_DISTANCE)
	{
		m_cameraDistance = MAX_CAMERA_DISTANCE;
	}
	else if (m_cameraDistance < MIN_CAMERA_DISTANCE)
	{
		m_cameraDistance = MIN_CAMERA_DISTANCE;
	}
}

/*
*/
void COpenGLRenderer::deleteTexture(unsigned int *id) 
{
	if (id != NULL && *id > 0) 
	{ 
		glDeleteTextures(1, id);
	} 
}

/*
*/
void COpenGLRenderer::drawString(unsigned int *textureObjectId, std::string &text, float x, float y, CVector3 &color)
{
	// TO-DO
}

/*
*/
bool COpenGLRenderer::isDebugContextEnabled() const
{
	GLint flags; 
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		return true;
	}

	return false;
}

/*
*/
void COpenGLRenderer::activateOpenGLDebugging()
{
// Only enable OpenGL debugging if compiling for a DEBUG configuration
#ifdef _DEBUG
	/* Check if a debug context could be created when creating the rendering context */
	if (isDebugContextEnabled())
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debugOutputCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif
}

/*
*/
void APIENTRY COpenGLRenderer::debugOutputCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const GLvoid *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------------" << std::endl;
	std::cout << "OpenGL error ocurred:" << std::endl; 
	std::cout << "---------------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: High"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: Medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: Low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: Notification"; break;
	} std::cout << std::endl;

	std::cout << std::endl;
}

/*
*/
GLenum COpenGLRenderer::translateBlendMode(OPENGL_BLEND_MODE factor)
{
	GLenum translatedValue = GL_ONE;

	switch (factor)
	{
	case BLEND_ZERO: 
		translatedValue = GL_ZERO;
		break;
	case BLEND_ONE: 
		translatedValue = GL_ONE;
		break;
	case BLEND_SRC_COLOR:
		translatedValue = GL_SRC_COLOR;
		break;
	case BLEND_ONE_MINUS_SRC_COLOR: 
		translatedValue = GL_ONE_MINUS_SRC_COLOR;
		break;
	case BLEND_DST_COLOR: 
		translatedValue = GL_DST_COLOR;
		break;
	case BLEND_ONE_MINUS_DST_COLOR: 
		translatedValue = GL_ONE_MINUS_DST_COLOR;
		break;
	case BLEND_SRC_ALPHA: 
		translatedValue = GL_SRC_ALPHA;
		break;
	case BLEND_ONE_MINUS_SRC_ALPHA: 
		translatedValue = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case BLEND_DST_ALPHA: 
		translatedValue = GL_DST_ALPHA;
		break;
	case BLEND_ONE_MINUS_DST_ALPHA: 
		translatedValue = GL_ONE_MINUS_DST_ALPHA;
		break;
	case BLEND_CONSTANT_COLOR: 
		translatedValue = GL_CONSTANT_COLOR;
		break;
	case BLEND_ONE_MINUS_CONSTANT_COLOR: 
		translatedValue = GL_ONE_MINUS_CONSTANT_COLOR;
		break;
	case BLEND_CONSTANT_ALPHA: 
		translatedValue = GL_CONSTANT_ALPHA;
		break;
	case BLEND_ONE_MINUS_CONSTANT_ALPHA: 
		translatedValue = GL_ONE_MINUS_CONSTANT_ALPHA;
		break;
	default:
		translatedValue = GL_ONE;
		break;
	}

	return translatedValue;
}

/*
*/
void COpenGLRenderer::setBlendingMode(OPENGL_BLEND_MODE sourceFactor, OPENGL_BLEND_MODE destinationFactor)
{
	GLenum srcF = translateBlendMode(sourceFactor);
	GLenum dstF = translateBlendMode(destinationFactor);

	glBlendFunc(srcF, dstF);
}


/*
*/
void COpenGLRenderer::startProfiling()
{
	// TO-DO
}

/*
*/
void COpenGLRenderer::endProfiling()
{
	// TO-DO
}

/*
*/
GLuint64 COpenGLRenderer::getProfilingTime()
{
	// TO-DO
	return 0;
}

/*
*/
void COpenGLRenderer::allocateGraphicsMemoryForLines(
	const unsigned int * const shaderProgramId,
	unsigned int *vertexArrayObjectID,
	GLfloat *lineVertices, int numLines,    // Coordinates (x1, y1, z1), (x2, y2, z2) of each line. 6 floats per line
	float lineWidth,                        // Width/thickness of the line in pixel
	float red, float green, float blue,     // RGB color components
	float backR, float backG, float backB,  // Color of background when alphablend=false,  Br=alpha of color when alphablend=true
	float cAlpha,                           // Alpha 
	bool alphablend)
{
	if (alphablend)
	{
		backR = cAlpha;
	}

	//line(
	//	x1, y1, x2, y2,
	//	lineWidth,
	//	red, green, blue,
	//	backR, backG, backB,
	//	alphablend);
}

/*
*/
void COpenGLRenderer::getLineParameters(
	float *x1, float *y1, float *x2, float *y2, // coordinates of the line
	float *Cr, float *Cg, float *Cb, float *Br,
	float w,
	bool alphablend,
	float *A,
	float *tx, float *ty,     // core thinkness of a line
	float *Rx, float *Ry,     // fading edge of a line
	float *cx, float *cy      // cap of a line
	)
{
	float t, R, f;
	f = w - static_cast<int>(w);
	*A = 1.0f;

	if (alphablend)
	{
		*A = *Br;
	}

	// determine parameters t,R
	if (w >= 0.0f && w < 1.0f) 
	{
		t = 0.05f; 
		R = 0.48f + 0.32f * (f);

		if (!alphablend) 
		{
			*Cr += 0.88f * (1.0f - f);
			*Cg += 0.88f * (1.0f - f);
			*Cb += 0.88f * (1.0f - f);
			if (*Cr > 1.0f) *Cr = 1.0f;
			if (*Cg > 1.0f) *Cg = 1.0f;
			if (*Cb > 1.0f) *Cb = 1.0f;
		}
		else 
		{
			*A *= f;
		}
	}
	else if (w >= 1.0f && w < 2.0f) 
	{
		t = 0.05f + (f) * 0.33f; 
		R = 0.768f + 0.312f * (f);
	}
	else if (w >= 2.0f && w < 3.0f) 
	{
		t = 0.38f + (f) * 0.58f;
		R = 1.08f;
	}
	else if (w >= 3.0f && w < 4.0f) 
	{
		t = 0.96f + (f) * 0.48f;
		R = 1.08f;
	}
	else if (w >= 4.0f && w < 5.0f) 
	{
		t = 1.44f + (f) * 0.46f;
		R = 1.08f;
	}
	else if (w >= 5.0f && w < 6.0f) 
	{
		t = 1.9f + (f) * 0.6f;
		R = 1.08f;
	}
	else if (w >= 6.0f) 
	{
		float ff = w - 6.0f;
		t = 2.5f + ff * 0.50f;
		R = 1.08f;
	}

	// determine angle of the line to horizontal
	*tx = 0;
	*ty = 0; //core thinkness of a line
	*Rx = 0;
	*Ry = 0; //fading edge of a line
	*cx = 0;
	*cy = 0; //cap of a line
	float ALW = 0.01f;
	float dx = *x2 - *x1;
	float dy = *y2 - *y1;

	if (GET_ABS(dx) < ALW) 
	{
		//vertical
		*tx = t; 
		*ty = 0.0f;
		*Rx = R; 
		*Ry = 0.0f;

		if (w > 0.0f && w <= 1.0f) 
		{
			*tx = 0.5f; 
			*Rx = 0.0f;
		}
	}
	else if (GET_ABS(dy) < ALW) 
	{
		//horizontal
		*tx = 0.0f; 
		*ty = t;
		*Rx = 0.0f; 
		*Ry = R;

		if (w > 0.0f && w <= 1.0f) 
		{
			*ty = 0.5f; 
			*Ry = 0.0f;
		}
	}
	else 
	{
		if (w < 3.0f) 
		{ //approximate to make things even faster
			float m = dy / dx;

			//and calculate tx,ty,Rx,Ry
			if (m > -0.4142f && m <= 0.4142f) 
			{
				// -22.5< angle <= 22.5, approximate to 0 (degree)
				*tx = t * 0.1f;
				*ty = t;
				*Rx = R * 0.6f;
				*Ry = R;
			}
			else if (m > 0.4142f && m <= 2.4142f)
			{
				// 22.5< angle <= 67.5, approximate to 45 (degree)
				*tx = t * -0.7071f;
				*ty = t * 0.7071f;
				*Rx = R * -0.7071f;
				*Ry = R * 0.7071f;
			}
			else if (m > 2.4142f || m <= -2.4142f) 
			{
				// 67.5 < angle <=112.5, approximate to 90 (degree)
				*tx = t;
				*ty = t * 0.1f;
				*Rx = R;
				*Ry = R * 0.6f;
			}
			else if (m > -2.4142f && m < -0.4142f)
			{
				// 112.5 < angle < 157.5, approximate to 135 (degree)
				*tx = t * 0.7071f;
				*ty = t * 0.7071f;
				*Rx = R * 0.7071f;
				*Ry = R * 0.7071f;
			}
			else 
			{
				// error in determining angle
				//printf( "error in determining angle: m=%.4f\n",m);
			}
		}
		else 
		{ //calculate to exact
			dx = *y1 - *y2;
			dy = *x2 - *x1;
			float L = sqrtf((dx * dx) + (dy * dy));
			dx /= L;
			dy /= L;
			*cx = -1.0f * (dy); 
			*cy = dx;
			*tx = t * (dx); 
			*ty = t * (dy);
			*Rx = R * (dx); 
			*Ry = R * (dy);
		}
	}

	*x1 += *cx * 0.5f; 
	*y1 += *cy * 0.5f;
	*x2 -= *cx * 0.5f; 
	*y2 -= *cy * 0.5f;
}

//static inline double GET_ABS(double x) { return x>0 ? x : -x; }
/*
* this implementation uses vertex array (opengl 1.1)
*   choose only 1 from vase_rend_draft_1.h and vase_rend_draft_2.h
*   to your need. if you have no preference, just use vase_rend_draft_2.h
*/
/* this is the master line() function which features:
* -premium quality anti-aliased line drawing
* -smaller CPU overhead than other CPU rasterizing algorithms
* -line thickness control
* -line color control
* -can choose to use alpha blend or not
*
* sample usage using alpha blending:
*
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
glOrtho( 0,context_width,context_height,0,0.0f,100.0f);

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);
line ( 10,100,100,300,		//coordinates
1.2,			//thickness in px
0.5, 0.0, 1.0, 1.0,	//line color RGBA
0,0,			//not used
true);			//enable alphablend

//more line() or glDrawArrays() calls
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);

//other drawing code...
glPopMatrix();
glDisable(GL_BLEND); //restore blending options
*
* and not using alpha blending (blend to background color):
*
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
glOrtho( 0,context_width,context_height,0,0.0f,100.0f);

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);
line ( 20,100,110,300,		//coordinates
1.2,			//thickness in px
0.5, 0.0, 1.0,		//line color *RGB*
1.0, 1.0, 1.0,		//background color
false);			//not using alphablend

//more line() or glDrawArrays() calls
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);

//other drawing code...
glPopMatrix();
*/

/*
void line(double x1, double y1, double x2, double y2, //coordinates of the line
	float w,			//width/thickness of the line in pixel
	float Cr, float Cg, float Cb,	//RGB color components
	float Br, float Bg, float Bb,	//color of background when alphablend=false,
									//  Br=alpha of color when alphablend=true
	bool alphablend)		//use alpha blend or not
{
	double t; double R; double f = w - static_cast<int>(w);
	float A;

	if (alphablend)
		A = Br;
	else
		A = 1.0f;

	//determine parameters t,R
	if (w >= 0.0 && w<1.0) {
		t = 0.05; R = 0.48 + 0.32*f;
		if (!alphablend) {
			Cr += 0.88*(1 - f);
			Cg += 0.88*(1 - f);
			Cb += 0.88*(1 - f);
			if (Cr>1.0) Cr = 1.0;
			if (Cg>1.0) Cg = 1.0;
			if (Cb>1.0) Cb = 1.0;
		}
		else {
			A *= f;
		}
	}
	else if (w >= 1.0 && w<2.0) {
		t = 0.05 + f*0.33; R = 0.768 + 0.312*f;
	}
	else if (w >= 2.0 && w<3.0) {
		t = 0.38 + f*0.58; R = 1.08;
	}
	else if (w >= 3.0 && w<4.0) {
		t = 0.96 + f*0.48; R = 1.08;
	}
	else if (w >= 4.0 && w<5.0) {
		t = 1.44 + f*0.46; R = 1.08;
	}
	else if (w >= 5.0 && w<6.0) {
		t = 1.9 + f*0.6; R = 1.08;
	}
	else if (w >= 6.0) {
		double ff = w - 6.0;
		t = 2.5 + ff*0.50; R = 1.08;
	}
	//printf( "w=%f, f=%f, C=%.4f\n", w,f,C);

	//determine angle of the line to horizontal
	double tx = 0, ty = 0; //core thinkness of a line
	double Rx = 0, Ry = 0; //fading edge of a line
	double cx = 0, cy = 0; //cap of a line
	double ALW = 0.01;
	double dx = x2 - x1;
	double dy = y2 - y1;
	if (GET_ABS(dx) < ALW) {
		//vertical
		tx = t; ty = 0;
		Rx = R; Ry = 0;
		if (w>0.0 && w <= 1.0) {
			tx = 0.5; Rx = 0.0;
		}
	}
	else if (GET_ABS(dy) < ALW) {
		//horizontal
		tx = 0; ty = t;
		Rx = 0; Ry = R;
		if (w>0.0 && w <= 1.0) {
			ty = 0.5; Ry = 0.0;
		}
	}
	else {
		if (w < 3) { //approximate to make things even faster
			double m = dy / dx;
			//and calculate tx,ty,Rx,Ry
			if (m>-0.4142 && m <= 0.4142) {
				// -22.5< angle <= 22.5, approximate to 0 (degree)
				tx = t*0.1; ty = t;
				Rx = R*0.6; Ry = R;
			}
			else if (m>0.4142 && m <= 2.4142) {
				// 22.5< angle <= 67.5, approximate to 45 (degree)
				tx = t*-0.7071; ty = t*0.7071;
				Rx = R*-0.7071; Ry = R*0.7071;
			}
			else if (m>2.4142 || m <= -2.4142) {
				// 67.5 < angle <=112.5, approximate to 90 (degree)
				tx = t; ty = t*0.1;
				Rx = R; Ry = R*0.6;
			}
			else if (m>-2.4142 && m<-0.4142) {
				// 112.5 < angle < 157.5, approximate to 135 (degree)
				tx = t*0.7071; ty = t*0.7071;
				Rx = R*0.7071; Ry = R*0.7071;
			}
			else {
				// error in determining angle
				//printf( "error in determining angle: m=%.4f\n",m);
			}
		}
		else { //calculate to exact
			dx = y1 - y2;
			dy = x2 - x1;
			double L = sqrt(dx*dx + dy*dy);
			dx /= L;
			dy /= L;
			cx = -dy; cy = dx;
			tx = t*dx; ty = t*dy;
			Rx = R*dx; Ry = R*dy;
		}
	}

	x1 += cx*0.5; y1 += cy*0.5;
	x2 -= cx*0.5; y2 -= cy*0.5;

	//draw the line by triangle strip
	float line_vertex[] =
	{
		x1 - tx - Rx - cx,   y1 - ty - Ry - cy, //fading edge1
		x2 - tx - Rx + cx,   y2 - ty - Ry + cy,
		x1 - tx - cx,        y1 - ty - cy,	  //core
		x2 - tx + cx,        y2 - ty + cy,
		x1 + tx - cx,        y1 + ty - cy,
		x2 + tx + cx,        y2 + ty + cy,
		x1 + tx + Rx - cx,   y1 + ty + Ry - cy, //fading edge2
		x2 + tx + Rx + cx,   y2 + ty + Ry + cy
	};

	glVertexPointer(2, GL_FLOAT, 0, line_vertex);

	if (!alphablend) {
		float line_color[] =
		{
			Br,Bg,Bb,
			Br,Bg,Bb,
			Cr,Cg,Cb,
			Cr,Cg,Cb,
			Cr,Cg,Cb,
			Cr,Cg,Cb,
			Br,Bg,Bb,
			Br,Bg,Bb
		};
		glColorPointer(3, GL_FLOAT, 0, line_color);
	}
	else {
		float line_color[] =
		{
			Cr,Cg,Cb,0,
			Cr,Cg,Cb,0,
			Cr,Cg,Cb,A,
			Cr,Cg,Cb,A,
			Cr,Cg,Cb,A,
			Cr,Cg,Cb,A,
			Cr,Cg,Cb,0,
			Cr,Cg,Cb,0
		};
		glColorPointer(4, GL_FLOAT, 0, line_color);
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

	//cap
	if (w < 3) {
		//do not draw cap
	}
	else {
		//draw cap
		float line_vertex[] =
		{
			x1 - tx - Rx - cx, y1 - ty - Ry - cy, //cap1
			x1 - tx - Rx, y1 - ty - Ry,
			x1 - tx - cx, y1 - ty - cy,
			x1 + tx + Rx, y1 + ty + Ry,
			x1 + tx - cx, y1 + ty - cy,
			x1 + tx + Rx - cx, y1 + ty + Ry - cy,
			x2 - tx - Rx + cx, y2 - ty - Ry + cy, //cap2
			x2 - tx - Rx, y2 - ty - Ry,
			x2 - tx + cx, y2 - ty + cy,
			x2 + tx + Rx, y2 + ty + Ry,
			x2 + tx + cx, y2 + ty + cy,
			x2 + tx + Rx + cx, y2 + ty + Ry + cy
		};

		glVertexPointer(2, GL_FLOAT, 0, line_vertex);

		if (!alphablend) {
			float line_color[] =
			{
				Br,Bg,Bb, //cap1
				Br,Bg,Bb,
				Cr,Cg,Cb,
				Br,Bg,Bb,
				Cr,Cg,Cb,
				Br,Bg,Bb,
				Br,Bg,Bb, //cap2
				Br,Bg,Bb,
				Cr,Cg,Cb,
				Br,Bg,Bb,
				Cr,Cg,Cb,
				Br,Bg,Bb
			};
			glColorPointer(3, GL_FLOAT, 0, line_color);
		}
		else {
			float line_color[] =
			{
				Cr,Cg,Cb, 0, //cap1
				Cr,Cg,Cb, 0,
				Cr,Cg,Cb, A,
				Cr,Cg,Cb, 0,
				Cr,Cg,Cb, A,
				Cr,Cg,Cb, 0,
				Cr,Cg,Cb, 0, //cap2
				Cr,Cg,Cb, 0,
				Cr,Cg,Cb, A,
				Cr,Cg,Cb, 0,
				Cr,Cg,Cb, A,
				Cr,Cg,Cb, 0
			};
			glColorPointer(4, GL_FLOAT, 0, line_color);
		}

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
		glDrawArrays(GL_TRIANGLE_STRIP, 6, 6);
	}
}*/