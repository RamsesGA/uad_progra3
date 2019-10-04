#pragma once

#ifndef _WIDESTRING_HELPER_H
#define _WIDESTRING_HELPER_H

#include <Shlwapi.h>
#include <iostream>
#include <vector>
using namespace std;

class CWideStringHelper
{
public:
	CWideStringHelper() {}
	~CWideStringHelper() {}

	// ============================================================================================================================================
	// Given a resource filename (model, texture, shader), this method returns the full path name of where the resource should be
	// within the /Resources/MEDIA/ folder.
	// This method also checks for the file existence. If the resource is not found, this method returns false
	//
	// Documentation for GetModuleFilename:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
	// Documentation for GetCurrentDirectory:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa364934(v=vs.85).aspx
	// Documentation for fopen_s and _wfopen_s
	// https://msdn.microsoft.com/es-mx/library/z5hh6ee9.aspx
	// Documentation for MultiByteToWideChar
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072(v=vs.85).aspx
	// Documentation for WideCharToMultiByte
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130(v=vs.85).aspx
	// Secure version of CRT functions
	// https://msdn.microsoft.com/en-us/library/wd3wzwts.aspx
	// ============================================================================================================================================
	static bool GetResourceFullPath(const char *resourceFileName, std::wstring &resourceFullPathUTF16, std::string &resourceFullPath)
	{
		bool resourceFound = false;
		FILE *filePtr;

		// ----------------------------------------------------------------------------------
		// UNICODE 
		// ----------------------------------------------------------------------------------
		// Check if UNICODE is defined in the project properties:
		//   Configuration Properties --> General --> Project Defaults --> Character Set
#ifdef UNICODE

		// Get the full path of the current module (the .EXE that's being executed)
		vector<wchar_t> pathBuf;
		DWORD copied = 0;

		do {
			pathBuf.resize(pathBuf.size() + MAX_PATH);
			copied = GetModuleFileNameW(0, &pathBuf.at(0), (DWORD)pathBuf.size());
		} while (copied >= pathBuf.size());

		// Remove the filename of the .exe, to leave only the full path, without the filename
		PathRemoveFileSpecW(&pathBuf.at(0));

		//std::wstring path(pathBuf.begin(), pathBuf.end());
		std::wstring path(&pathBuf[0]);

		// Copy the resulting path to the output param
		resourceFullPathUTF16.append(path);

		// Convert the resource name from char* to wchar_t
		wchar_t* wResource = UTF8_to_UTF16(resourceFileName);

		if (wResource == nullptr)
		{
			return false;
		}

		// Now append the relative path where the resources are located
		resourceFullPathUTF16.append(L"\\..\\..\\..\\..\\Resources\\MEDIA\\");
		
		// Append the filename
		resourceFullPathUTF16.append(wResource);
		delete[] wResource;

		// Finally, check for the file existence
		errno_t err = _wfopen_s(&filePtr, &resourceFullPathUTF16[0], L"r, ccs=UTF-16LE");

		if (err == 0)
		{
			resourceFound = true;

			// We need to return both UTF8 and UTF16
			// -------------------------------------
			char* resource = UTF16_to_UTF8(&resourceFullPathUTF16.at(0));

			if (resource == nullptr)
			{
				if (filePtr != nullptr)
				{
					fclose(filePtr);
				}
				return false;
			}

			resourceFullPath.append(resource);
			delete[] resource;
		}

		if (filePtr != nullptr)
		{
			fclose(filePtr);
		}

#else
		// ----------------------------------------------------------------------------------
		// NOT-UNICODE 
		// ----------------------------------------------------------------------------------

		// Get the full path of the current module (the .EXE that's being executed)
		char *workingDirectory = new char[MAX_PATH + 1];
		GetModuleFileNameA(NULL, workingDirectory, MAX_PATH + 1);

		// Remove the filename of the .exe, to leave only the full path, without the filename
		PathRemoveFileSpecA(workingDirectory);

		// Copy the resulting path to the output param
		resourceFullPath.append(workingDirectory);
		delete[] workingDirectory;

		// Now append the relative path where the resources are located
		resourceFullPath.append("\\..\\..\\..\\..\\Resources\\MEDIA\\");

		// Append the filename
		resourceFullPath.append(resourceFileName);

		// Finally, check for the file existence
		errno_t err = fopen_s(&filePtr, resourceFullPath.c_str(), "r, ccs=UTF-8");

		if (err == 0)
		{
			resourceFound = true;

			// We need to return both UTF8 and UTF16
			// -------------------------------------
			wchar_t* resourceW = UTF8_to_UTF16(resourceFullPath.c_str());

			if (resourceW == nullptr)
			{
				if (filePtr != nullptr)
				{
					fclose(filePtr);
				}
				return false;
			}

			resourceFullPathUTF16.append(resourceW);
			delete[] resourceW;
		}

		if (filePtr != nullptr)
		{
			fclose(filePtr);
		}
#endif

		return resourceFound;
	}

	// ============================================================================================================================================
	// Convert a multi-byte string to UTF16
	// IMPORTANT: The caller of this function is responsible for freeing up the memory allocated for the wchar_t*
	// ============================================================================================================================================
	static wchar_t* UTF8_to_UTF16(const char * str)
	{
		/*
		int MultiByteToWideChar(
			_In_      UINT   CodePage,
			_In_      DWORD  dwFlags,
			_In_      LPCSTR lpMultiByteStr,
			_In_      int    cbMultiByte,
			_Out_opt_ LPWSTR lpWideCharStr,
			_In_      int    cchWideChar
		);	*/

		if (str == nullptr)
		{
			return nullptr;
		}

		// Calculate the number of characters (including the null terminator) needed to hold the wide character version of the string
		int numWideCharsNeeded = MultiByteToWideChar(CP_UTF8, 0, str, (int)-1, NULL, 0);

		if (numWideCharsNeeded == 0)
		{
			cout << "Failed converting UTF-8 string to UTF-16" << endl;
			return nullptr;
		}

		wchar_t *wstr = new wchar_t[numWideCharsNeeded];
		int numCharsConverted = MultiByteToWideChar(
			CP_UTF8, 
			0, 
			str, 
			(int)( sizeof(char) * (strlen(str) + 1) ), 
			&wstr[0], 
			numWideCharsNeeded);

		if (numCharsConverted == 0 || 
			numWideCharsNeeded != numCharsConverted)
		{
			cout << "Failed converting UTF-8 string to UTF-16" << endl;
			return nullptr;
		}

		return wstr;
	}

	// ============================================================================================================================================
	// Convert a UTF16 string to UTF8
	// IMPORTANT: The caller of this function is responsible for freeing up the memory allocated for the char*
	// ============================================================================================================================================
	static char* UTF16_to_UTF8(const wchar_t *wstr)
	{
		/*
		int WideCharToMultiByte(
			_In_      UINT    CodePage,
			_In_      DWORD   dwFlags,
			_In_      LPCWSTR lpWideCharStr,
			_In_      int     cchWideChar,
			_Out_opt_ LPSTR   lpMultiByteStr,
			 _In_      int     cbMultiByte,
			_In_opt_  LPCSTR  lpDefaultChar,
			_Out_opt_ LPBOOL  lpUsedDefaultChar
		); */

		if (wstr == nullptr)
		{
			return nullptr;
		}

		// Calculate the number of characters (including the null terminator) needed to hold the multibyte character version of the string
		int numCharsNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)-1, NULL, 0, NULL, NULL);

		if (numCharsNeeded == 0)
		{
			cout << "Failed converting UTF-16 string to UTF-8" << endl;
			return nullptr;
		}

		char *str = new char[numCharsNeeded];
		int numCharsConverted = WideCharToMultiByte(
			CP_UTF8,
			0,
			wstr,
			numCharsNeeded,
			&str[0],
			(int)(numCharsNeeded * sizeof(char)),
			NULL,
			NULL);

		if (numCharsConverted == 0 ||
			numCharsNeeded != numCharsConverted)
		{
			cout << "Failed converting UTF-16 string to UTF-8" << endl;
			return nullptr;
		}

		return str;
	}


};

#endif // !_WIDESTRING_HELPER_H
