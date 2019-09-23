#pragma once

#ifndef C3DMODEL_H_FBX
#define C3DMODEL_H_FBX

#include <string>
using namespace std;
#include "C3DModel.h"

class C3DModel_FBX : public C3DModel
{
	private:
		int m_currentVertex, m_currentNormal, m_currentUV, m_currentFace, m_currentUV_Index;	// Aux counters used when loading an object from file

		//bool readFbxFile(const char * const file_name, bool count_Only);	// Read object from file 

		bool parseFbxLine(const char * const file_name, bool control);	// Parse line
			
		bool readMtllib(string m_tl_Lib_File_name);
	
	protected:
		void reset();														// Cleanup any allocated memory

		bool loadFromFile(const char * const file_name);
	
	public:
		C3DModel_FBX();
		~C3DModel_FBX();
};

#endif // !C3DMODEL_H_FBX