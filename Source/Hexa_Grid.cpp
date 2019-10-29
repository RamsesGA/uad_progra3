#include "..\Include\Hexa_Grid.h"
#include "../Include/Hexa_World.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

Hexa_Grid::Hexa_Grid() {}
Hexa_Grid::~Hexa_Grid(){}

void Hexa_Grid::initialize(unsigned int _num_cols, unsigned int _num_rows, float _cell_size, bool _p_or_f, CVector3 _center)
{
	system("cls");

	cout << "\nNumero de columnas - -> " << _num_cols << endl;
	cout << "\nNumero de filas - ->    " << _num_rows << endl;
	cout << "\nTamaño de la celda - -> " << _cell_size << endl;

	if (_p_or_f == true) //Finalmente checamos en que posición debe estár el panal
	{
		cout << "\nEl panal sera POINTY\n" << endl;
		hexa_pointy_points(_center, 1, _cell_size,_p_or_f);
		structure_hexa(_p_or_f);
	}
	else
	{
		cout << "\nEl panal sera FLAT\n" << endl;
		hexa_pointy_points(_center, 1, _cell_size, _p_or_f);
		structure_hexa(_p_or_f);
	}
}

CVector3 Hexa_Grid::hexa_pointy_points(CVector3 _center, int _index, float _cell_size, bool _p_or_f)
{
	CVector3 point;
	float angle = 0;
	float angle_rad = 0.0f;

	if (_p_or_f == true)//POINTY
	{
		angle = (60 * _index) - 30;//index	
		angle_rad = angle * PI_OVER_180;
		point.Y = 0.0f;
		point.X = _center.X + _cell_size * cos(angle_rad);
		point.Z = _center.Y + _cell_size * sin(angle_rad);
	}
	else if (_p_or_f == false)//FLAT
	{
		angle = (60 * _index);//index
		angle_rad = angle * PI_OVER_180;
		point.Y = 0.0f;
		point.X = _center.X + _cell_size * cos(angle_rad);
		point.Z = _center.Y + _cell_size * sin(angle_rad);
	}
	return point;
}

void Hexa_Grid::structure_hexa(bool _p_or_f)
{
	CVector3 p_1, p_2, p_3, p_4, p_5, p_6;
	CVector3 v1, v2, v3, v1v2, v1v3, norm;

	p_1 = hexa_pointy_points(CVector3(0.0f, 0.0f, 0.0f), 1, 3, _p_or_f);
	p_2 = hexa_pointy_points(CVector3(0.0f, 0.0f, 0.0f), 2, 3, _p_or_f);
	p_3 = hexa_pointy_points(CVector3(0.0f, 0.0f, 0.0f), 3, 3, _p_or_f);
	p_4 = hexa_pointy_points(CVector3(0.0f, 0.0f, 0.0f), 4, 3, _p_or_f);
	p_5 = hexa_pointy_points(CVector3(0.0f, 0.0f, 0.0f), 5, 3, _p_or_f);
	p_6 = hexa_pointy_points(CVector3(0.0f, 0.0f, 0.0f), 6, 3, _p_or_f);

	//numero de vertices * 3
	float vertex_data[18] =
	{
	 p_1.X, p_1.Y, p_1.Z ,
	 p_2.X, p_2.Y, p_2.Z ,
	 p_3.X, p_3.Y, p_3.Z ,
	 p_4.X, p_4.Y, p_4.Z ,
	 p_5.X, p_5.Y, p_5.Z ,
	 p_6.X, p_6.Y, p_6.Z };
	for (int i = 0; i < 18; i++)
	{
		m_vertex_data[i] = vertex_data[i];
	}

	//numero de vertices *2
	float vertex_UVs[12] = { 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5 };
	for (int i = 0; i < 12; i++)
	{
		m_vertex_UVs[i] = vertex_UVs[i];
	}

	//numero de caras y cada cara tiene 3 componentes (4*3 = 12)
	unsigned short face_Indices[12] =
	{
	0,1,2,
	2,3,5,
	3,4,5,
	5,0,2
	};
	for (int i = 0; i < 12; i++)
	{
		m_face_Indices[i] = face_Indices[i];
	}

	//numero de caras * sus 3 componentes (4*3 = 12)
	float normal_Data[12] = { 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0 };
	for (int i = 0; i < 12; i++)
	{
		m_normal_Data[i] = normal_Data[i];
	}

	unsigned short face_Normal_Indices[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 };
	for (int i = 0; i < 12; i++)
	{
		m_face_Normal_Indices[i] = face_Normal_Indices[i];
	}

	numFaces = 4;

	for (int i = 0; i < numFaces; i++)
	{
		// Vertex 1
		v1.setValues(
			vertex_data[face_Indices[i * 3]],
			vertex_data[face_Indices[i * 3] + 1],
			vertex_data[face_Indices[i * 3] + 2]);

		// Vertex 2
		v2.setValues(
			vertex_data[face_Indices[(i * 3) + 1]],
			vertex_data[face_Indices[(i * 3) + 1] + 1],
			vertex_data[face_Indices[(i * 3) + 1] + 2]
		);

		// Vertex 3
		v3.setValues(
			vertex_data[face_Indices[(i * 3) + 2]],
			vertex_data[face_Indices[(i * 3) + 2] + 1],
			vertex_data[face_Indices[(i * 3) + 2] + 2]
		);

		// Vector from v1 to v2
		v1v2 = v2 - v1;

		// Vector from v1 to v3
		v1v3 = v3 - v1;

		norm = v1v2.cross(v1v3);
		norm.normalize();

		normal_Data[i * 3] = norm.X;
		normal_Data[(i * 3) + 1] = norm.Y;
		normal_Data[(i * 3) + 2] = norm.Z;
	}
}