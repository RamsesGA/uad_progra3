#include "..\stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "..\Include\C3DModel_FBX.h"
#include "..\Include\C3DModel.h"
#include "..\Include\CVector3.h"


//-------------------------PUBLIC-------------------------
//Constructor
C3DModel_FBX::C3DModel_FBX() : C3DModel(), m_currentVertex(0), m_currentNormal(0), m_currentUV(0), m_currentFace(0)
{
	cout << "\nConstructor de C3DModel_FBX().cpp" << endl;
}

//Destructor
C3DModel_FBX::~C3DModel_FBX()
{
	cout << "\nDestructor de C3DModel_FBX().cpp" << endl;
	reset();
}



//-------------------------PROTECTED-------------------------
void C3DModel_FBX::reset()
{
	C3DModel::reset(); //Se accede a la función reset

	m_currentVertex = 0;
	m_currentNormal = 0;
	m_currentUV = 0;
	m_currentFace = 0;
}

bool C3DModel_FBX::loadFromFile(const char * const file_name)
{
	bool read_file = false;

	//Liberamos cualquier recurso que tenga
	reset();

	//Tenemos que contar el número de vertices, normales, UVs y caras
	read_file = readFbxFile(file_name, true);

	//Mostramos pantalla
	cout << "\nSe terminó de leer el modelo 3D FBX" << endl;
	cout << "\nNumero de vertices --> " << m_numVertices << endl;
	cout << "\nNumero de normales --> " << m_numNormals << endl;
	cout << "\nNumero de UV --> " << m_numUVCoords << endl;
	cout << "\nNumero de caras --> " << m_numFaces << endl;

	//Checamos si nuestro bool es verdadero
	if (read_file)
	{
		//Checamos el valor MAX de vertices, normales, UV y caras
		if (m_numVertices >= 65535 || m_numNormals >= 65535 || m_numUVCoords >= 65535 || m_numFaces >= 65535)
		{
			cout << "\nError, el objeto no puede tener mas de 65535 vertices" << endl;
			cout << "\nEl objeto que se cargo es --> " << m_numVertices << " vertices " << endl;

			return false;
		}

		//En caso de que la figura no tenga UV normales, definiremos un valor por default
		if (m_numNormals == 0)
		{
			m_modelHasNormals = false;
			m_numNormals = m_numVertices;
		}
		else
		{
			m_modelHasNormals = true;
		}
		if (m_numUVCoords == 0)
		{
			m_modelHasUVs = false;
			m_numUVCoords = m_numVertices;
		}
		else
		{
			m_modelHasUVs = true;
		}


		//Asignamos memoria para los arrays

		//Variables de C3DModel
		m_verticesRaw = new float [m_numVertices * 3];
		m_normalsRaw = new float [m_numNormals * 3];
		m_uvCoordsRaw = new float [m_numUVCoords * 2];
		m_vertexIndices = new unsigned short [m_numFaces * 3];
		m_normalIndices = new unsigned short[m_numFaces * 3];
		m_UVindices = new unsigned short[m_numFaces * 3];

		//Dejamos los valores de estos arrays en 0, para evitar problemas
		memset(m_verticesRaw, 0, sizeof(float) * m_numVertices * 3);
		memset(m_normalsRaw, 0, sizeof(float) * m_numNormals * 3);
		memset(m_uvCoordsRaw, 0, sizeof(float) * m_numUVCoords * 2);
		memset(m_vertexIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_normalIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_UVindices, 0, sizeof(unsigned short) * m_numFaces * 3);

		//Después pasamos a leer los datos
		read_file = readFbxFile(file_name, false);

		if (read_file)
		{
			m_modelGeometryLoaded = true;

			if (!m_modelHasNormals)
			{
				computeFaceNormals();
			}
		}
	}
	//En caso de que sea falso
	else
	{
		cout << "\nOcurrio un error al intentar leer el archivo 3D" << endl;
	}
	return read_file;
}






//-------------------------PRIVATED-------------------------
bool C3DModel_FBX::readFbxFile(const char * const file_name, bool count_Only)
{
	ifstream in_file;
	string line_buffer;
	bool read_file = true;
	int line_number = 0;

	in_file.open(file_name);

	while (!in_file.eof())
	{
		getline(in_file, line_buffer);
		line_number++;

		if (!(this->parseFbxLine(line_buffer, count_Only, line_number)))
		{
			read_file = false;
			break;
		}
	}

	in_file.close();

	return read_file;
}

//Mi turno o(╥﹏╥)o
bool C3DModel_FBX::parseFbxLine(std::string line, bool count_Only, int line_Number)
{
	return false;
}


bool C3DModel_FBX::readMtllib(std::string m_tl_Lib_File_name)
{
	
	ifstream in_file;
	string line_buffer;
	string current_material_name;

	int num_token = 0;

	float material_red, material_green, material_blue;

	char *next_token = nullptr;
	char *token = nullptr;
	const char *delimiter_token = " \t";

	bool read_texture_name = false;
	bool reading_material_name = false;
	bool reading_material_file_name = false;
	bool reading_material_color = false;

	m_materialNames.clear();
	m_materialFilenames.clear();
	m_materialColors.clear();

	in_file.open(m_tl_Lib_File_name);



	while (!in_file.eof())
	{
		getline(in_file, line_buffer);

		reading_material_name = false;
		reading_material_file_name = false;
		reading_material_color = false;
		material_red = 0.0f;
		material_green = 0.0f;
		material_blue = 0.0f;
		num_token = 0;

		token = strtok_s((char *)line_buffer.c_str(), delimiter_token, &next_token);
		++num_token;

		// If there are any tokens left
		while (token != nullptr)
		{
			if (num_token == 1)
			{
				if (0 == strcmp(token, "newmtl"))
				{
					reading_material_name = true;
					reading_material_color = false;
					reading_material_file_name = false;
				}
				else if (0 == strcmp(token, "map_Kd"))
				{
					reading_material_name = false;
					reading_material_color = false;
					reading_material_file_name = true;
				}
				else if (0 == strcmp(token, "Kd"))
				{
					reading_material_name = false;
					reading_material_color = true;
					reading_material_file_name = false;
				}
			}
			else if (num_token == 2)
			{
				if (reading_material_name)
				{
					current_material_name = token;
					m_materialNames.push_back(token);
				}
				else if (reading_material_file_name && !current_material_name.empty())
				{
					m_materialFilenames.insert(std::make_pair(current_material_name, token));
					read_texture_name = true;
					break;
				}
				else if (reading_material_color)
				{
					material_red = (float)atof(token);

					if (material_red < 0.0f || material_red > 1.0f)
					{
						material_red = 0.0f;
					}
				}
			}
			else if (num_token == 3)
			{
				if (reading_material_color)
				{
					material_green = (float)atof(token);

					if (material_green < 0.0f || material_green > 1.0f)
					{
						material_green = 0.0f;
					}
				}
			}
			else if (num_token == 4)
			{
				if (reading_material_color && !current_material_name.empty())
				{
					material_blue = (float)atof(token);

					if (material_blue < 0.0f || material_blue > 1.0f)
					{
						material_blue = 0.0f;
					}

					m_materialColors.insert(std::make_pair(current_material_name, CVector3(material_red, material_green, material_blue)));
				}
			}

			token = strtok_s(nullptr, delimiter_token, &next_token);
			++num_token;
		}
	}

	in_file.close();

	return (m_materialNames.size() > 0);
}
