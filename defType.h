#pragma once
#pragma pack(1)


typedef char CHAR;
typedef short SHORT;
typedef long LONG;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned int UINT;

typedef CHAR* LPSTR, * PSTR;

typedef struct tagBITMAPINFOHEADER {
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage{};
	LONG       biXPelsPerMeter{};
	LONG       biYPelsPerMeter{};
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER, * LPBITMAPINFOHEADER, * PBITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1{};
	WORD    bfReserved2{};
	DWORD   bfOffBits;
}  BITMAPFILEHEADER, * LPBITMAPFILEHEADER, * PBITMAPFILEHEADER;

typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} RGBQUAD;
typedef RGBQUAD* LPRGBQUAD;



// Segment parsing error codes
constexpr int JPEG_SEG_OK = 0;
constexpr int JPEG_SEG_ERR = 1;
constexpr int JPEG_SEG_EOF = 2;

// bit code parsing codes
constexpr int END_OF_TABLE = 0;
constexpr int IN_TABLE = 1;