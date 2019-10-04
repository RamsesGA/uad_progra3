#pragma once

#ifndef C3DMODEL_H_OBJ
#define C3DMODEL_H_OBJ

#include <string>
using namespace std;

#include "C3DModel.h"

class C3DModel_Obj : public C3DModel
{
private:
	int m_currentVertex, m_currentNormal, m_currentUV, m_currentFace;	// Aux counters used when loading an object from file

	bool readObjFile(const char * const filename, bool countOnly);		// Read object from file 
	bool parseObjLine(													// Parse line
		std::string line,								  
		bool countOnly, 
		int lineNumber); 
	bool readMtllib(std::string mtlLibFilename);	 

protected:
	void reset();														// Cleanup any allocated memory
	bool loadFromFile(const char * const filename);

public:
	C3DModel_Obj();
	~C3DModel_Obj();
};

#endif // !C3DMODEL_H_OBJ

