#pragma once

#ifndef __LOAD_TGA_H__
#define __LOAD_TGA_H__

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

	typedef struct
	{
		unsigned char  imageTypeCode;
		short int      imageWidth;
		short int      imageHeight;
		unsigned char  bitCount;
		unsigned char *imageData;
	} TGAFILE;

	bool LoadTGAFile(const char *filename, TGAFILE *tgaFile);

	//----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __LOAD_TGA_H__