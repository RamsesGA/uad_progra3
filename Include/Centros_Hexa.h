#pragma once
#include <vector>
#include "../Include/CVector3.h"
#include "../Include/C3DModel.h"


class Centers
{
	public:
		//Constructores
		Centers(CVector3 _center);
		~Centers();

		//Variables
		CVector3 centers;
		C3DModel *instance_obj;
};