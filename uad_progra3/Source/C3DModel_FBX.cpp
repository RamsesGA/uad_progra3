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
	read_file = parseFbxLine(file_name, true);

	m_numVertices = m_currentVertex;
	m_numNormals = m_currentNormal;
	m_numUVCoords = m_currentUV;
	m_numFaces = m_currentFace;

	//Mostramos pantalla
	cout << "\nSe termino de leer el modelo 3D FBX" << endl;
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
//bool C3DModel_FBX::readFbxFile(const char * const file_name, bool count_Only){}

//Mi turno o(╥﹏╥)o
bool C3DModel_FBX::parseFbxLine(const char * const file_name, bool control)
{
	char *token = nullptr;
	char *nextToken = nullptr;
	char *nextToken2 = nullptr;
	char delimiterToken = '\t';
	char nextdelimiterToken = '*';

	bool read_file = true;

	int number = 0;
	int line_number = 0;

	float num_add = 0;

	//int num_face_norm = 0;
	int index = 0;

	ifstream in_file;
	string line_buffer;


	//----------INICIO
	in_file.open(file_name);

	while (!in_file.eof())
	{
		getline(in_file, line_buffer);	//Obtenemos una linea

		line_number++;	//Aumentamos una linea

		//Creamos un token del lado izquierdo del texto
		token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

		if (token != NULL)	//---------VERTICES
		{
			if (0 == strcmp(token, "Vertices:"))
			{
				line_buffer = nextToken;	//El buffer obtiene el valor que se obtuvo antes a la derecha

				//Creamos un token del lado izquierdo y sobreescribimos lo que tenía el buffer
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;

				//Aqui token te da la cantidad de datos de los vertices
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				number = 0;
				istringstream iss(token);	//Aqui se transforma token en int para darle tamaño al arreglo
				iss >> number;

				m_currentVertex = number / 3;	//Definimos los triangulos
				m_verticesRaw = new float[number];	//Damos el tamaño de todas las coordenadas

				getline(in_file, line_buffer);
				line_number++;

				index = 0;	//Inicializamos el index

				//Obtenemos el 
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						num_add = 0;

						token = strtok_s(nextToken, ",", &nextToken2);	//Se convierte en token las coordenadas

						if (index < number)
						{
							istringstream iss2(token);	//Se convierte el token (string) en int
							iss2 >> num_add;

							m_verticesRaw[index++] = num_add;	//Dependiendo de la posición ubicamos las coordenadas
						}
						else
						{
							token = NULL;
						}

						if (*nextToken2 == NULL || *nextToken2 == '\0' || *nextToken2 == '\n' || *nextToken2 == ' ')
						{
							getline(in_file, line_buffer);
							line_number++;

							if (line_buffer == "} ")
							{
								break;
							}
							else
							{
								nextToken2 = (char *)line_buffer.c_str();
							}

						}
						nextToken = nextToken2;	//Aqui va la lista de lo que se va ha guardar
					}
					index = 0;
				}
			}

			//---------PolygonVertexIndex /3 (CARAS)
			else if (0 == strcmp(token, "PolygonVertexIndex:"))
			{
				line_buffer = nextToken;	//El buffer obtiene el valor que se obtuvo antes a la derecha

				//Creamos un token del lado izquierdo y sobreescribimos lo que tenía el buffer
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;

				//Aqui token te da la cantidad de datos de los vertices
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				number = 0;
				istringstream iss(token);	//Aqui se transforma token en int para darle tamaño al arreglo
				iss >> number;

				m_currentFace = number / 3;	//Definimos los triangulos
				m_vertexIndices = new unsigned short[number];

				getline(in_file, line_buffer);
				line_number++;

				index = 0;	//Inicializamos el index

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						num_add = 0;

						token = strtok_s(nextToken, ",", &nextToken2);	//Se convierte en token las coordenadas

						if (index < number)
						{
							istringstream iss2(token);	//Se convierte el token (string) en int
							iss2 >> num_add;

							if (num_add < 0)
							{
								num_add = (num_add * -1) - 1;
							}
							m_vertexIndices[index++] = num_add;	//Dependiendo de la posición ubicamos las coordenadas
						}

						if (*nextToken2 == NULL || *nextToken2 == '\0' || *nextToken2 == '\n' || *nextToken2 == ' ')
						{
							getline(in_file, line_buffer);
							line_number++;

							if (line_buffer == "} ")
							{
								break;
							}
							else
							{
								nextToken2 = (char *)line_buffer.c_str();
							}

						}
						nextToken = nextToken2;	//Aqui va la lista de lo que se va ha guardar
					}
					index = 0;
				}
			}

			//---------NORMALES
			else if (0 == strcmp(token, "Normals:"))
			{
				line_buffer = nextToken;	//El buffer obtiene el valor que se obtuvo antes a la derecha

				//Creamos un token del lado izquierdo y sobreescribimos lo que tenía el buffer
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;

				//Aqui token te da la cantidad de datos de los vertices
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				number = 0;
				istringstream iss(token);	//Aqui se transforma token en int para darle tamaño al arreglo
				iss >> number;

				m_currentNormal = (number / 3) / 3;	//Definimos los triangulos
				m_normalsRaw = new float[number];

				getline(in_file, line_buffer);
				line_number++;

				index = 0;	//Inicializamos el index

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						num_add = 0;

						token = strtok_s(nextToken, ",", &nextToken2);	//Se convierte en token las coordenadas

						if (index < number)
						{
							istringstream iss2(token);	//Se convierte el token (string) en int
							iss2 >> num_add;

							m_normalsRaw[index++] = num_add;	//Dependiendo de la posición ubicamos las coordenadas
						}

						if (*nextToken2 == NULL || *nextToken2 == '\0' || *nextToken2 == '\n' || *nextToken2 == ' ')
						{
							getline(in_file, line_buffer);
							line_number++;

							if (line_buffer == "} ")
							{
								break;
							}
							else
							{
								nextToken2 = (char *)line_buffer.c_str();
							}

						}
						nextToken = nextToken2;	//Aqui va la lista de lo que se va ha guardar
					}
					index = 0;
				}
			}

			//---------UV
			else if (0 == strcmp(token, "UV:"))
			{
				line_buffer = nextToken;	//El buffer obtiene el valor que se obtuvo antes a la derecha

				//Creamos un token del lado izquierdo y sobreescribimos lo que tenía el buffer
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;

				//Aqui token te da la cantidad de datos de los vertices
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				number = 0;
				istringstream iss(token);	//Aqui se transforma token en int para darle tamaño al arreglo
				iss >> number;

				m_currentUV = number / 2;	//Definimos los triangulos
				m_uvCoordsRaw = new float[number];

				getline(in_file, line_buffer);
				line_number++;

				index = 0;	//Inicializamos el index

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						num_add = 0;

						token = strtok_s(nextToken, ",", &nextToken2);	//Se convierte en token las coordenadas

						if (index < number)
						{
							istringstream iss2(token);	//Se convierte el token (string) en int
							iss2 >> num_add;

							m_uvCoordsRaw[index++] = num_add;	//Dependiendo de la posición ubicamos las coordenadas
						}

						if (*nextToken2 == NULL || *nextToken2 == '\0' || *nextToken2 == '\n' || *nextToken2 == ' ')
						{
							getline(in_file, line_buffer);
							line_number++;

							if (line_buffer == "} ")
							{
								break;
							}
							else
							{
								nextToken2 = (char *)line_buffer.c_str();
							}

						}
						nextToken = nextToken2;	//Aqui va la lista de lo que se va ha guardar
					}
					index = 0;
				}
			}

			//---------UVIndex
			else if (0 == strcmp(token, "UVIndex:"))
			{
				line_buffer = nextToken;	//El buffer obtiene el valor que se obtuvo antes a la derecha

				//Creamos un token del lado izquierdo y sobreescribimos lo que tenía el buffer
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;

				//Aqui token te da la cantidad de datos de los vertices
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				number = 0;
				istringstream iss(token);	//Aqui se transforma token en int para darle tamaño al arreglo
				iss >> number;

				m_currentUV_Index = number / 3;	//Definimos los triangulos
				m_UVindices = new unsigned short[number];

				getline(in_file, line_buffer);
				line_number++;

				index = 0;	//Inicializamos el index

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);

				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						num_add = 0;

						token = strtok_s(nextToken, ",", &nextToken2);	//Se convierte en token las coordenadas

						if (index < number)
						{
							istringstream iss2(token);	//Se convierte el token (string) en int
							iss2 >> num_add;

							m_UVindices[index++] = num_add;	//Dependiendo de la posición ubicamos las coordenadas
						}

						if (*nextToken2 == NULL || *nextToken2 == '\0' || *nextToken2 == '\n' || *nextToken2 == ' ')
						{
							getline(in_file, line_buffer);
							line_number++;

							if (line_buffer == "} ")
							{
								break;
							}
							else
							{
								nextToken2 = (char *)line_buffer.c_str();
							}

						}
						nextToken = nextToken2;	//Aqui va la lista de lo que se va ha guardar
					}
				}
				index = 0;
			}
		}
	}
	in_file.close();
	return true;
}

bool C3DModel_FBX::readMtllib(string m_tl_Lib_File_name)
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
