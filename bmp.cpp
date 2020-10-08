#include <iostream>
#include <fstream>
#include "defType.h"
using namespace std;
#pragma pack(1)


ifstream fin;
ofstream fout;
BITMAPFILEHEADER   bf;
BITMAPINFOHEADER   bi;
DWORD              NumColors;
DWORD              LineBytes;
DWORD              ImgWidth = 8, ImgHeight = 8;
DWORD              ImgSize{};
char* lpPtr{};

//int main()
//{
//	cout << sizeof(BITMAPFILEHEADER);
//
//	string RGB_values_file = "test.dat";
//	fin.open(RGB_values_file.c_str(), ios_base::binary);
//	if (!fin.is_open())
//	{
//		cerr << "File doesn't exit" << endl;
//		exit(EXIT_FAILURE);
//	}
//
//
//	bi.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
//	bi.biWidth = (LONG)(ImgWidth);
//	bi.biHeight = (LONG)(ImgHeight);
//	bi.biPlanes = 1;
//	bi.biBitCount = 24;
//	bi.biClrUsed = 0;
//	bi.biClrImportant = 0;
//	bi.biCompression = 0L;
//	NumColors = 0;
//	printf("bi.biWidth is %ld\n", bi.biWidth);
//	printf("bi.biBitCount is %ld\n", bi.biBitCount);
//	//LineBytes = (DWORD)WIDTHBYTES(bi.biWidth * bi.biBitCount);
//	printf("LineBytes is %ld\n", LineBytes);
//	ImgSize = (DWORD)LineBytes * bi.biHeight;//???????
//	printf("size is %ld\n", ImgSize);
//	bf.bfType = 0x4d42;
//	bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + NumColors * sizeof(RGBQUAD) + ImgSize;
//	bf.bfOffBits = 54;//(DWORD)(NumColors*sizeof(RGBQUAD)+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER));
//
//	fout.open("test.bmp", ios_base::binary);
//	fout.write((LPSTR)&bf, sizeof(BITMAPFILEHEADER));
//	fout.write((LPSTR)&bi, sizeof(BITMAPINFOHEADER));
//
//	char c{};
//	for (int s = 0; s < 3; s++)
//	{
//		for (int i = 0; i < 8; i++)
//		{
//			for (int j = 0; j < 8; j++)
//			{
//				fin.read(&c, 1);
//				fout.write(&c, 1);
//			}
//		}
//	}
//}
