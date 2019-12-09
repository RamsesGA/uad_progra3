#pragma once
#include <vector>
#include "../Include/CVector3.h"
#include "../Include/C3DModel.h"


class Centers
{
	public:
		//Constructores
		Centers();
		Centers(CVector3 _center);
		void
		getTriangleCount();
		~Centers();

		//Variables
		CVector3 centers;
		C3DModel *instance_obj;

};