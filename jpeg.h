#pragma once

#include "defType.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

// PI definition
constexpr double PI = 3.14159265358979323846;

// Class jpeg defination
class JPEG
{
private:
	// Names of the possible segments
	string segNames[64] =
	{
		// Debug messages used by parseSeg to tell us which segment we're at
		"Baseline DCT; Huffman",					//0x00
		"Extended sequential DCT; Huffman",			//0x01
		"Progressive DCT; Huffman",					//0x02
		"Spatial lossless; Huffman",				//0x03
		"Huffman table",							//0x04
		"Differential sequential DCT; Huffman",		//0x05
		"Differential progressive DCT; Huffman",	//0x06
		"Differential spatial; Huffman",			//0x07
		"[Reserved: JPEG extension]",				//0x08
		"Extended sequential DCT; Arithmetic",		//0x09
		"Progressive DCT; Arithmetic",				//0x0A
		"Spatial lossless; Arithmetic",				//0x0B
		"Arithmetic coding conditioning",			//0x0C
		"Differential sequential DCT; Arithmetic",	//0x0D
		"Differential progressive DCT; Arithmetic",	//0x0E
		"Differential spatial; Arithmetic",			//0x0F
		"Restart",									//0x10
		"Restart",									//0x11
		"Restart",									//0x12
		"Restart",									//0x13
		"Restart",									//0x14
		"Restart",									//0x15
		"Restart",									//0x16
		"Restart",									//0x17
		"Start of image",							//0x18
		"End of image",								//0x19
		"Start of scan",							//0x1A
		"Quantisation table",						//0x1B
		"Number of lines",							//0x1C
		"Restart interval",							//0x1D
		"Hierarchical progression",					//0x1E
		"Expand reference components",				//0x1F
		"JFIF header",								//0x20 
		"[Reserved: application extension]",		//0x21
		"[Reserved: application extension]",		//0x22
		"[Reserved: application extension]",		//0x23
		"[Reserved: application extension]",		//0x24
		"[Reserved: application extension]",		//0x25
		"[Reserved: application extension]",		//0x26
		"[Reserved: application extension]",		//0x27
		"[Reserved: application extension]",		//0x28
		"[Reserved: application extension]",		//0x29
		"[Reserved: application extension]",		//0x2A
		"[Reserved: application extension]",		//0x2B
		"[Reserved: application extension]",		//0x2C
		"[Reserved: application extension]",		//0x2D
		"[Reserved: application extension]",		//0x2E
		"[Reserved: application extension]",		//0x2F
		"[Reserved: JPEG extension]",				//0x30
		"[Reserved: JPEG extension]",				//0x31
		"[Reserved: JPEG extension]",				//0x32
		"[Reserved: JPEG extension]",				//0x33
		"[Reserved: JPEG extension]",				//0x34
		"[Reserved: JPEG extension]",				//0x35
		"[Reserved: JPEG extension]",				//0x36
		"[Reserved: JPEG extension]",				//0x37
		"[Reserved: JPEG extension]",				//0x38
		"[Reserved: JPEG extension]",				//0x39
		"[Reserved: JPEG extension]",				//0x3A
		"[Reserved: JPEG extension]",				//0x3B
		"[Reserved: JPEG extension]",				//0x3C
		"[Reserved: JPEG extension]",				//0x3D
		"Comment",									//0x3E
		"[Invalid]"									//0x3F
	};

	// Define file stream
	ifstream fin;

	string filename_;

	// Read word inline function
	WORD readWord() { return ((fin.get() << 8) | fin.get()); }



	// **************huffman table variable defination*******************

	// huffkey is comprised of "Length, Code"
	// Length is an integer, Code is up to 16 bits length
	typedef pair<int, WORD> huffKey;

	// Maps to hold the Huffman data
	// huffKey is the Huffman code, BYTE is Huffman code corresponding value
	typedef map<huffKey, BYTE> huffData;

	// Maps to hold the Huffman table
	// BYTE is the Huffman table ID, huffData is the Huffman Data
	typedef map<BYTE, huffData> huffTable;
	huffTable huff_table{};


	// *******************JPEG file variable defination***********************

	BYTE color_bits_depth{};
	WORD resolution_height{};
	WORD resolution_width{};
	BYTE component_number{};

	// Data of each component: component ID, sampling factor, QT id and HT id
	struct component
	{
		// Component ID: 1 = Y, 2 = Cb, 3 = Cr
		BYTE component_id{};
		BYTE sampling_factor{};
		BYTE QT_id{};
		BYTE HT_id{};
		// number of table of each component in a MCU
		unsigned int number{};
	}Y, Cb, Cr;

	int hor_samp_factor{};
	int ver_samp_factor{};
	int MCU_width{};
	int MCU_height{};


	// *********SOS segment varibles and methods defination*********

	// varible defined for readbit function

	// byte readed for processing bit
	BYTE byte_tmp{};
	// byte mask used for reading bits
	BYTE byte_mask{};

	// bit code pair <zeros length , bit code>
	typedef pair<int, int> bitCode;
	vector<vector<bitCode>> bit_code_Y{};
	vector<vector<bitCode>> bit_code_Cb{};
	vector<vector<bitCode>> bit_code_Cr{};



	// read bit inline function
	WORD readOneBit();

	// read n bits function
	WORD readNBits(unsigned int n);

	// transfer complement code to original code
	int complement2int(WORD complement, int bit_code_length);

	// parse SOS segment data
	int parseData();

	// find huffman code once, return the huffman value
	int findHMCode(BYTE HT_id, vector<bitCode>& bit_code_group);

	// turn the bit_code into original data
	vector<int> RLE_to_org(vector<bitCode> bit_code);




	// ************** quantization table variable defination *******************

	// quantization 8*8 Matrix
	typedef vector<vector<int>> array2D;

	// quantization table precision
	int quanTablePrecision{};

	// maps to hold quantization table
	// BYTE is the quantization table id, vector<int> is a 64 bits array
	typedef map<BYTE, vector<int>> quanTable;
	quanTable quan_table{};

	// anti zig zag function
	// input an 64 bits array
	vector<int> q_zig_zag(vector<int> array);

	short zigzag[64] =
	{
		0,  1, 8,16, 9, 2, 3,10,
		17,24,32,25,18,11, 4, 5,
		12,19,26,33,40,48,41,34,
		27,20,13, 6, 7,14,21,28,
		35,42,49,56,57,50,43,36,
		29,22,15,23,30,37,44,51,
		58,59,52,45,38,31,39,46,
		53,60,61,54,47,55,62,63
	};

	// turn the 64 1D array into 8*8 2D array
	array2D array_to_matrix(vector<int> array);



	// anti quantization function
	vector<int> q_quan(vector<int> array, BYTE table_id);

	// IDCT
	array2D IDCT(array2D ar);

	double alpha(short u);

	// YCbCr to RGB
	vector<array2D> YCbCr_to_RGB(vector<array2D> MCU);

	// divide the Cb and Cr 
	array2D divide_matrix(array2D ar);


	vector<vector<array2D>> RGB_total;
	vector<array2D> RGB_total_after_merge;

	vector<array2D> merge_RGB(vector<vector<array2D>> RGB_total);

	// bmp file
	ofstream fout;

	void RGB_to_bmp(void);

	// merge 4 matrices to 1 matrix
	array2D merge_matrices(vector<array2D> vecar);


	BYTE* pf{};

	int MCU_number_width{};
	int MCU_number_height{};

	int get_MCU_number();

	int Y_dc{};
	int Cb_dc{};
	int Cr_dc{};


	// huffman table segment
	int DHT();

	// start of frame0, extract JPEG file information
	int SOF0();

	// start of scan
	int SOS();

	// quantization table segment
	int DQT();


	// Parse the segment
	int parseSeg();

public:
	// Construct a JPEG object given a filename
	JPEG(string);
};

// assistant function
WORD JPEG::readOneBit()
{
	if (byte_mask == 0)
	{
		byte_tmp = fin.get();
		if (byte_tmp == 0xFF)
		{
			BYTE tmp = fin.get();
			if (tmp != 0x00)
			{
				fin.seekg(-1, ios_base::cur);
			}
		}
		byte_mask = 0x80;
	}
	bool is_1 = byte_tmp & byte_mask;
	byte_mask >>= 1;
	if (is_1)
	{
		return 0x0001;
	}
	else
	{
		return 0x0000;
	}
}


WORD JPEG::readNBits(unsigned int n)
{
	WORD result{};
	for (size_t i = 0; i < n; i++)
	{
		if (byte_mask == 0)
		{
			byte_tmp = fin.get();
			if (byte_tmp == 0xFF)
			{
				BYTE tmp = fin.get();
				if (tmp != 0x00)
				{
					fin.seekg(-1, ios_base::cur);
				}
			}
			byte_mask = 0x80;
		}

		bool is_1 = byte_tmp & byte_mask;
		byte_mask >>= 1;
		result <<= 1;
		if (is_1)
		{
			result += 1;
		}
	}
	return result;
}


int JPEG::complement2int(WORD complement, int bit_code_length)
{
	// generate highest bit mask 
	WORD highest_bit_mask = 0x0001;
	for (size_t i = 1; i < bit_code_length; i++)
	{
		highest_bit_mask <<= 1;
	}

	// judge whethe it's negative or not
	if ((highest_bit_mask & complement) == 0)
	{
		// if it's negative, negate every bit
		WORD negate_mask = 0x0001;
		for (size_t i = 0; i < bit_code_length; i++)
		{
			// negate current bit
			complement ^= negate_mask;
			// shift leftly one bit
			negate_mask <<= 1;
		}
		return -int(complement);
	}
	else
	{
		return complement;
	}
}


int JPEG::findHMCode(BYTE HT_id, vector<bitCode>& bit_code_group)
{
	// code length we want to know whether is the correct length
	int guessed_length{ 2 };

	// detect whether guessed_code is same with real huffman code
	WORD guessed_code = readNBits(2);

	// zero length
	int zero_length{};

	// readed bit code
	WORD bit_code{};

	// bit code value
	int bit_code_value{};

	// bit code length (not the huffman code length)
	int bit_code_length{};

	huffData::iterator iter{};
	for (iter = huff_table[HT_id].begin();
		iter != huff_table[HT_id].end(); iter++)
	{
		// if huffman code length == guessed length
		if (iter->first.first == guessed_length)
		{
			// if huffman code == guessed code
			// we find the correct huffman code!
			if (iter->first.second == guessed_code)
			{
				// huffman value first part -> zeros length
				zero_length = int((iter->second & 0xF0) >> 4);

				// huffman value second part -> bit code length
				bit_code_length = int(iter->second & 0x0F);

				// so we read such length bit code
				bit_code = readNBits(bit_code_length);

				// tranfer complement code to bit_code_value
				bit_code_value = complement2int(bit_code, bit_code_length);

				// use vector push_back method
				bit_code_group.push_back(
					// construct a bitCode pair for storing
					bitCode(zero_length, bit_code_value)
				);
				//cout << "           " << setfill('0')
				//	<< setw(4) << guessed_code << "             "
				//	<< setw(2) << int(iter->second) << "            "
				//	<< setw(3) << bit_code << "          "
				//	<< setw(4) << dec << setfill(' ') << bit_code_value << hex
				//	<< endl;
				if (zero_length == 0 and bit_code_length == 0)
				{
					return END_OF_TABLE;
				}
				else
				{
					return IN_TABLE;
				}
			}
		}
		// if guessed length is shorter than huffman code length
		// read 1 bit to guessed code and increase length by 1
		// go back by 1
		else if (iter->first.first > guessed_length)
		{
			guessed_code = (guessed_code << 1) + readOneBit();
			guessed_length++;
			iter--;
		}
		else
		{
			cerr << "Unexpected error!";
		}
	}
}


vector<int> JPEG::RLE_to_org(vector<bitCode> bit_code)
{
	vector<int> org_data{};
	for (auto i = bit_code.begin(); i != bit_code.end(); i++)
	{
		// store the zeros
		for (int j = 0; j < i->first; j++)
		{
			org_data.push_back(0);
		}

		// store the non-zeros value
		org_data.push_back(i->second);
	}

	// if size not same with 64, fill the remainig with zeros
	if (org_data.size() < 64)
	{
		for (auto i = org_data.size(); i < 64; i++)
		{
			org_data.push_back(0);
		}
	}
	else if (org_data.size() > 64)
	{
		cerr << "Unexpected bit code vector" << endl;
	}

	//cout << "Original data: " << endl;

	//cout << dec;
	//for (size_t i = 0; i < org_data.size(); i++)
	//{
	//	cout << org_data[i] << " ";
	//}
	//cout << endl;

	return vector<int>(org_data);
}


int JPEG::parseData()
{
	vector<bitCode> bit_code_temp{};

	// extract Y bitcode
	for (size_t i = 0; i < Y.number; i++)
	{
		//cout << setfill(' ');
		//cout << "Y" << i + 1 << " bitcode:" << endl;
		//cout << setw(15) << "huffman code" << setw(15) << "huffman value" << setw(15) << "bit code" << endl;

		// find the Y DC portion using HT00
		findHMCode(0x00, bit_code_temp);

		// current number of bit code, shouldn't exceed 63
		int bit_code_num{};

		// find the Y AC portion using HT10
		while (findHMCode(0x10, bit_code_temp) != END_OF_TABLE)
		{
			bit_code_num += bit_code_temp.back().first + 1;
			if (bit_code_num >= 63)
			{
				break;
			}
		}

		bit_code_Y.push_back(bit_code_temp);
		//cout << endl;
		bit_code_temp = {};
	}

	// extract Cb bitcode
	for (size_t i = 0; i < Cb.number; i++)
	{
		//cout << setfill(' ');
		//cout << "Cb" << i + 1 << " bitcode:" << endl;
		//cout << setw(15) << "huffman code" << setw(15) << "huffman value" << setw(15) << "bit code" << endl;

		// find the Cb DC portion using HT01
		findHMCode(0x01, bit_code_temp);

		// current number of bit code, shouldn't exceed 63
		int bit_code_num{};

		// find the Cb AC portion using HT11
		while (findHMCode(0x11, bit_code_temp) != END_OF_TABLE)
		{
			bit_code_num += bit_code_temp.back().first + 1;
			if (bit_code_num >= 63)
			{
				break;
			}
		}
		bit_code_Cb.push_back(bit_code_temp);
		//cout << endl;
		bit_code_temp = {};
	}

	// extract Cr bitcode
	for (size_t i = 0; i < Cr.number; i++)
	{
		//cout << setfill(' ');
		//cout << "Cr" << i + 1 << " bitcode:" << endl;
		//cout << setw(15) << "huffman code" << setw(15) << "huffman value" << setw(15) << "bit code" << endl;

		// find the Cr DC portion using HT01
		findHMCode(0x01, bit_code_temp);

		// current number of bit code, shouldn't exceed 63
		int bit_code_num{};

		// find the Cr AC portion using HT11
		while (findHMCode(0x11, bit_code_temp) != 0x00)
		{
			bit_code_num += bit_code_temp.back().first + 1;
			if (bit_code_num >= 63)
			{
				break;
			}
		}
		bit_code_Cr.push_back(bit_code_temp);
		//cout << endl;
		bit_code_temp = {};
	}

	return JPEG_SEG_OK;
}


vector<int> JPEG::q_zig_zag(vector<int> array)
{
	vector<int> tmp(64);
	for (auto i = 0; i < array.size(); i++)
	{
		tmp[zigzag[i]] = array[i];
	}
	//cout << "After q zig zag" << endl;
	//for (size_t i = 0; i < tmp.size(); i++)
	//{
	//	cout << tmp[i] << " ";
	//}
	//cout << endl;
	//cout << endl;
	return tmp;
}


JPEG::array2D JPEG::array_to_matrix(vector<int> array)
{
	vector<int> tmp;
	array2D matrix{};
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			tmp.push_back(array[8 * i + j]);
			//cout << setw(4) << tmp[j] << " ";
		}
		matrix.push_back(tmp);
		tmp = {};
		//cout << endl;
	}
	//cout << endl;
	return array2D(matrix);
}


vector<int> JPEG::q_quan(vector<int> array, BYTE table_id)
{
	vector<int> tmp(64);
	//cout << "After anti quantization: " << endl;
	for (size_t i = 0; i < 64; i++)
	{
		tmp[i] = array[i] * quan_table[table_id][i];
		//cout << tmp[i] << " ";
	}
	//cout << endl;
	return vector<int>(tmp);
}


inline double JPEG::alpha(short u)
{
	return u == 0 ? 1 / sqrt(8) : 0.5;
}


JPEG::array2D JPEG::IDCT(array2D ar)
{
	array2D ar_{};
	vector<int> ar_tmp{};
	double tmp{};

	//cout << "After IDCT: " << endl;
	//cout << setprecision(1) << fixed;
	for (short x = 0; x < 8; x++)
	{
		for (short y = 0; y < 8; y++)
		{
			for (short u = 0; u < 8; u++)
			{
				for (short v = 0; v < 8; v++)
				{
					tmp += alpha(u) * alpha(v) * ar[u][v]
						* cos((2 * double(x) + 1) / 16 * u * PI)
						* cos((2 * double(y) + 1) / 16 * v * PI);
				}
			}
			ar_tmp.push_back(tmp);
			//cout << setw(7) << tmp << " ";
			tmp = 0;
		}
		ar_.push_back(ar_tmp);
		ar_tmp = {};
		//cout << endl;
	}
	//cout << endl;
	return ar_;
}


JPEG::array2D JPEG::divide_matrix(array2D ar)
{
	array2D tmp(MCU_height, vector<int>(MCU_width));
	for (size_t i = 0, n = 0; i < MCU_height; i += 2, n++)
	{
		for (size_t j = 0, m = 0; j < MCU_width; j += 2, m++)
		{
			tmp[i][j] = tmp[i + 1][j] = tmp[i][j + 1] = tmp[i + 1][j + 1]
				= ar[n][m];
		}
	}

	// show tmp
	//cout << "after expand: " << endl;
	//for (size_t i = 0; i < MCU_height; i++)
	//{
	//	for (size_t j = 0; j < MCU_width; j++)
	//	{
	//		cout << setw(4) << tmp[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	return tmp;
}


vector<JPEG::array2D> JPEG::YCbCr_to_RGB(vector<array2D> MCU)
{
	vector<JPEG::array2D> RGB(3, array2D(MCU_height, vector<int>(MCU_width)));
	for (size_t i = 0; i < MCU_height; i++)
	{
		for (size_t j = 0; j < MCU_width; j++)
		{
			RGB[0][i][j] = MCU[0][i][j] + 1.4802 * MCU[2][i][j] + 128;
			RGB[1][i][j] = MCU[0][i][j] - 0.3441363 * MCU[1][i][j] - 0.71413636 * MCU[2][i][j] + 128;
			RGB[2][i][j] = MCU[0][i][j] + 1.772 * MCU[1][i][j] + 128;
			for (size_t s = 0; s < 3; s++)
			{
				if (RGB[s][i][j] > 255)
				{
					RGB[s][i][j] = 255;
				}
				else if (RGB[s][i][j] < 0)
				{
					RGB[s][i][j] = 0;
				}
			}
		}
	}

	//for (size_t s = 0; s < 3; s++)
	//{
	//	cout << s << " RGB" << endl;
	//	for (size_t i = 0; i < MCU_height; i++)
	//	{
	//		for (size_t j = 0; j < MCU_width; j++)
	//		{
	//			cout << setw(3) << RGB[s][i][j] << " ";
	//		}
	//		cout << endl;
	//	}
	//	cout << endl;
	//}
	return RGB;
}


void JPEG::RGB_to_bmp(void)
{
#define WIDTHBYTES(i)    ((i + 31) / 32 * 4)

	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	DWORD              NumColors;
	DWORD              LineBytes{};
	DWORD              ImgWidth = resolution_width, ImgHeight = resolution_height;
	DWORD              ImgSize{};


	//string RGB_values_file = "sunflower.dat";
	//fin1.open(RGB_values_file.c_str(), ios_base::binary);
	//if (!fin1.is_open())
	//{
	//	cerr << "File doesn't exit" << endl;
	//	exit(EXIT_FAILURE);
	//}



	bi.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	bi.biWidth = (LONG)(ImgWidth);
	bi.biHeight = (LONG)(ImgHeight);
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	bi.biCompression = 0L;
	NumColors = 0;
	//printf("bi.biWidth is %ld\n", bi.biWidth);
	//printf("bi.biBitCount is %ld\n", bi.biBitCount);
	LineBytes = (DWORD)WIDTHBYTES(bi.biWidth * bi.biBitCount);
	//printf("LineBytes is %ld\n", LineBytes);
	ImgSize = (DWORD)LineBytes * bi.biHeight;//???????
	//printf("size is %ld\n", ImgSize);
	bf.bfType = 0x4d42;
	bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ NumColors * sizeof(RGBQUAD) + ImgSize;
	bf.bfOffBits = 54;
	//(DWORD)(NumColors*sizeof(RGBQUAD)+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER));

	fout.open(filename_ + ".bmp", ios_base::binary);
	fout.write((LPSTR)&bf, sizeof(BITMAPFILEHEADER));
	fout.write((LPSTR)&bi, sizeof(BITMAPINFOHEADER));

	//fin1.seekg(192, ios_base::cur);

	for (int i = ImgHeight - 1; i >= 0; i--)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			for (int s = 2; s >= 0; s--)
			{
				fout.write((char*)&RGB_total_after_merge[s][i][j], 1);
			}
		}
	}

}


JPEG::array2D JPEG::merge_matrices(vector<array2D> vecar)
{
	array2D ar{};
	vector<int> ar_tmp{};
	for (size_t v = 0; v < ver_samp_factor; v++)
	{
		for (size_t i = 0; i < 8; i++)
		{
			for (size_t h = 0; h < hor_samp_factor; h++)
			{
				for (size_t j = 0; j < 8; j++)
				{
					ar_tmp.push_back(vecar[v * hor_samp_factor + h][i][j]);
				}
			}
			ar.push_back(ar_tmp);
			ar_tmp = {};
		}
	}

	//cout << "after merge: " << endl;
	//for (size_t i = 0; i < hor_samp_factor * 8; i++)
	//{
	//	for (size_t j = 0; j < ver_samp_factor * 8; j++)
	//	{
	//		cout << setw(3) << ar[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	return array2D(ar);
}


inline int JPEG::get_MCU_number()
{
	int width = resolution_width / MCU_width;
	if (resolution_width % MCU_width != 0)
	{
		width++;
	}

	int height = resolution_height / MCU_height;
	if (resolution_height % MCU_height != 0)
	{
		height++;
	}

	MCU_number_width = width;
	MCU_number_height = height;
	return width * height;
}


vector<JPEG::array2D> JPEG::merge_RGB(vector<vector<array2D>> RGB_total)
{
	vector<array2D> RGB_total_after_merge(3,
		array2D(MCU_number_height * MCU_height,
			vector<int>(MCU_number_width * MCU_width)));

	for (int s = 0; s < 3; s++)
	{
		for (int m = 0; m < MCU_number_height; m++)
		{
			for (int n = 0; n < MCU_number_width; n++)
			{
				for (int i = 0; i < MCU_height; i++)
				{
					for (int j = 0; j < MCU_width; j++)
					{
						RGB_total_after_merge[s][m * MCU_height + i][n * MCU_width + j] 
							= RGB_total[m * MCU_number_width + n][s][i][j];
					}
				}
			}
		}
	}

	return vector<array2D>(RGB_total_after_merge);
}


// main segment parsing function
int JPEG::DHT()
{
	// Read huffman table length
	WORD size = readWord();

	// read table ID
	BYTE table_id{};
	table_id = fin.get();

	// Next sixteen bytes are the counts for each code length
	unsigned int counts[16];
	for (size_t i = 0; i < 16; i++)
	{
		counts[i] = fin.get();
	}

	// huffman code, increase 1 in the same length
	// Shift left 1 with length increase 1
	WORD code{};

	for (size_t i = 0; i < 16; i++)
	{
		for (size_t j = 0; j < counts[i]; j++)
		{
			huff_table[table_id][huffKey(i + 1, code)] = fin.get();
			code++;
		}
		code <<= 1;
	}

	// cout the huffman table and set the format
	//cout << "Huffman table " << hex << setw(2) << setfill('0')
	//	<< int(table_id) << ":" << endl;
	//huffData::iterator iter;
	//cout << setfill(' ') << setw(8) << "length" << setw(8) << "code" << setw(8) << "value" << endl;
	//for (iter = huff_table[table_id].begin(); iter != huff_table[table_id].end(); iter++)
	//{
	//	cout << setfill(' ') << setw(8) << dec << iter->first.first
	//		<< setfill('0') << "    " << hex << setw(4) << iter->first.second
	//		<< "    " << setw(2) << int(iter->second)
	//		<< endl;
	//}

	return JPEG_SEG_OK;

}


int JPEG::SOF0()
{
	// read SOF0 segment length
	WORD size = readWord();

	color_bits_depth = fin.get();
	resolution_height = readWord();
	resolution_width = readWord();
	component_number = fin.get();

	// read Y, Cb, Cr component
	Y.component_id = fin.get();
	Y.sampling_factor = fin.get();
	Y.QT_id = fin.get();

	Cb.component_id = fin.get();
	Cb.sampling_factor = fin.get();
	Cb.QT_id = fin.get();

	Cr.component_id = fin.get();
	Cr.sampling_factor = fin.get();
	Cr.QT_id = fin.get();

	cout << hex;
	cout << "\tColor bits depth: " << int(color_bits_depth) << endl;
	cout << "\tResolution: " << resolution_width << "*"
		<< resolution_height << endl;
	cout << "\tComponent number: " << int(component_number) << endl;
	cout << "\tSampling_factor  Y:" << int(Y.sampling_factor)
		<< "  Cb:" << int(Cb.sampling_factor)
		<< "  Cr:" << int(Cr.sampling_factor) << endl;
	cout << "\tSampling factor: "
		<< ((Y.sampling_factor & 0XF0) >> 4) * (Y.sampling_factor & 0X0F)
		<< ":"
		<< ((Cb.sampling_factor & 0XF0) >> 4) * (Cb.sampling_factor & 0X0F)
		<< ":"
		<< ((Cr.sampling_factor & 0XF0) >> 4) * (Cr.sampling_factor & 0X0F)
		<< endl;

	// horizontal sampling factor
	hor_samp_factor = int((Y.sampling_factor & 0XF0) >> 4);
	MCU_width = hor_samp_factor * 8;
	// vertical sampling factor
	ver_samp_factor = int(Y.sampling_factor & 0X0F);
	MCU_height = ver_samp_factor * 8;

	Y.number = hor_samp_factor * ver_samp_factor;
	Cb.number = 1;
	Cr.number = 1;


	return JPEG_SEG_OK;
}


int JPEG::SOS()
{
	// read SOS segment length
	WORD size = readWord();

	// SOF0 component number and SOS component number compare
	BYTE component_number_temp = fin.get();
	if (component_number_temp != component_number)
	{
		cerr << ":Unexpected component number in SOS segment" << endl;
		return JPEG_SEG_ERR;
	}

	// read huffman table id of each component
	for (size_t i = 0; i < component_number; i++)
	{
		BYTE component_id = fin.get();
		switch (component_id)
		{
			case 0x01:
				Y.HT_id = fin.get();
				break;

			case 0x02:
				Cb.HT_id = fin.get();
				break;

			case 0x03:
				Cr.HT_id = fin.get();
				break;

			default:
				cerr << "Unexpect component id" << endl;
				return JPEG_SEG_ERR;
		}
	}

	// read 00 3F 00
	fin.get();
	fin.get();
	fin.get();

	//process_sos_data();

	cout << dec;
	// parseData
	int MCU_size = get_MCU_number();
	for (size_t MCU_index = 0; MCU_index < MCU_size; MCU_index++)
	{

		// original data vector
		vector<vector<int>> org_data_Y{};
		vector<vector<int>> org_data_Cb{};
		vector<vector<int>> org_data_Cr{};



		// data table after zigzag
		vector<array2D> org_data_zz_Y{};
		vector<array2D> org_data_zz_Cb{};
		vector<array2D> org_data_zz_Cr{};

		// data table after IDCT
		vector<array2D> data_Y{};
		vector<array2D> data_Cb{};
		vector<array2D> data_Cr{};

		// final data, 3 tables, Y, Cb, Cr, use a vector to store 3 array2D
		vector<array2D> MCU = vector<array2D>(3);

		// RGB values
		vector<array2D> RGB{};




		parseData();

		array2D Y{};
		array2D Cb{};
		array2D Cr{};

		// transfer bit_code into original data
		// Y
		for (auto i = 0; i < bit_code_Y.size(); i++)
		{
			org_data_Y.push_back(RLE_to_org(bit_code_Y[i]));

			// DC coefficient differential coding
			if (i == 0)
			{
				org_data_Y[i][0] += Y_dc;
			}
			else if (i > 0)
			{
				org_data_Y[i][0] += org_data_Y[i - 1][0];
			}
			org_data_zz_Y.push_back(array_to_matrix(q_zig_zag(q_quan(org_data_Y[i], 0x00))));
			data_Y.push_back(IDCT(org_data_zz_Y[i]));

		}
		Y_dc = (org_data_Y.back())[0];
		bit_code_Y = {};
		Y = merge_matrices(data_Y);

		// Cb
		for (auto i = 0; i < bit_code_Cb.size(); i++)
		{
			org_data_Cb.push_back(RLE_to_org(bit_code_Cb[i]));

			// DC coefficient differential coding
			if (i == 0)
			{
				org_data_Cb[i][0] += Cb_dc;
			}
			else if (i > 0)
			{
				org_data_Cb[i][0] += org_data_Cb[i - 1][0];
			}

			org_data_zz_Cb.push_back(array_to_matrix(q_zig_zag(q_quan(org_data_Cb[i], 0x01))));
			data_Cb.push_back(IDCT(org_data_zz_Cb[i]));
			Cb = divide_matrix(data_Cb[i]);
		}
		bit_code_Cb = {};
		Cb_dc = (org_data_Cb.back())[0];

		// Cr
		for (auto i = 0; i < bit_code_Cr.size(); i++)
		{
			org_data_Cr.push_back(RLE_to_org(bit_code_Cr[i]));

			// DC coefficient differential coding
			if (i == 0)
			{
				org_data_Cr[i][0] += Cr_dc;
			}
			else if (i > 0)
			{
				org_data_Cr[i][0] += org_data_Cr[i - 1][0];
			}

			org_data_zz_Cr.push_back(array_to_matrix(q_zig_zag(q_quan(org_data_Cr[i], 0x01))));
			data_Cr.push_back(IDCT(org_data_zz_Cr[i]));
			Cr = divide_matrix(data_Cr[i]);
		}
		bit_code_Cr = {};
		Cr_dc = (org_data_Cr.back())[0];

		MCU = { Y, Cb, Cr };
		RGB = YCbCr_to_RGB(MCU);
		RGB_total.push_back(RGB);

		cout << "\tMCU " << MCU_index << " parsed..." << endl;
	}
	//fout.open("sunflower.dat", ios_base::binary);

	//MCU
	//for (int i = 0; i < RGB.size(); i++)
	//{
	//	// one matrix
	//	// from bottom to up, from left to right
	//	for (int m = 7; m >= 0; m--)
	//	{
	//		for (int n = 0; n < 8; n++)
	//		{
	//			// B, G, R
	//			for (int j = 2; j >= 0; j--)
	//			{
	//				fout.write((char*)&RGB[i][j][m][n], sizeof(char));
	//			}
	//		}
	//	}
	//}


	// whole bmp
	// one matrix
	// from bottom to up, from left to right
	//for (int m = 7; m >= 0; m--)
	//{
	//	for (int n = 0; n < 8; n++)
	//	{
	//		// B, G, R
	//		for (int j = 2; j >= 0; j--)
	//		{
	//			fout.write((char*)&RGB[2][j][m][n], sizeof(char));
	//		}
	//	}
	//	for (int n = 0; n < 8; n++)
	//	{
	//		// B, G, R
	//		for (int j = 2; j >= 0; j--)
	//		{
	//			fout.write((char*)&RGB[3][j][m][n], sizeof(char));
	//		}
	//	}
	//}
	//for (int m = 7; m >= 0; m--)
	//{
	//	for (int n = 0; n < 8; n++)
	//	{
	//		// B, G, R
	//		for (int j = 2; j >= 0; j--)
	//		{
	//			fout.write((char*)&RGB[0][j][m][n], sizeof(char));
	//		}
	//	}
	//	for (int n = 0; n < 8; n++)
	//	{
	//		// B, G, R
	//		for (int j = 2; j >= 0; j--)
	//		{
	//			fout.write((char*)&RGB[1][j][m][n], sizeof(char));
	//		}
	//	}
	//}

	RGB_total_after_merge = merge_RGB(RGB_total);
	RGB_to_bmp();

	return JPEG_SEG_OK;
}


int JPEG::DQT()
{
	// read quantization table size
	WORD size = readWord();

	// read the quantization table infomation
	BYTE quanInfo = fin.get();

	BYTE table_id{};

	// high 4 bits are the precion
	if (((quanInfo & 0xF0) >> 4) == 0)
	{
		quanTablePrecision = 8;
	}
	else if (((quanInfo & 0xF0) >> 4) == 1)
	{
		quanTablePrecision = 16;
	}
	// low 4 bits are the table_id
	table_id = quanInfo & 0x0F;

	//// cout the quantization table information
	//cout << "Quantization table " << int(table_id)
	//	<< " (precision " << quanTablePrecision << ")" << ":" << endl;

	// read quantization array
	for (size_t i = 0; i < 64; i++)
	{
		quan_table[table_id].push_back(int(fin.get()));
	}

	//for (size_t i = 0; i < 8; i++)
	//{
	//	cout << "\t";
	//	for (size_t j = 0; j < 8; j++)
	//	{
	//		cout << quan_table[table_id][8 * i + j] << " ";
	//	}
	//	cout << endl;
	//}

	return 0;
}


// main segment classification function
int JPEG::parseSeg()
{
	// Tell the position
	streampos position = fin.tellg();

	// Read the segment mark
	WORD mark = readWord();

	// If find unknown mark, return error
	if (mark < 0xFFC0)
	{
		cerr << "Unknown segment mark at " << position << endl;
		return JPEG_SEG_ERR;
	}

	cout << "Found segment at " << hex << uppercase << position << ": " << segNames[mark - 0xFFC0] << endl;

	// segment size
	WORD size{};

	switch (mark)
	{
		// parse respective segment
		case 0xFFD8:
			break;

		case 0xFFE0:
			size = readWord() - 2;
			fin.seekg(size, ios_base::cur);
			break;

		case 0xFFE1:
			size = readWord() - 2;
			fin.seekg(size, ios_base::cur);
			break;

		case 0xFFDB:
			return DQT();

		case 0xFFC0:
			return SOF0();

		case 0xFFC4:
			return DHT();

		case 0xFFDA:
			return SOS();

		case 0xFFD9:
			return JPEG_SEG_EOF;

		default:
			size = readWord() - 2;
			fin.seekg(size, ios_base::cur);
			break;
	}

	return JPEG_SEG_OK;
}


// construct function
JPEG::JPEG(string filename)
{
	// Open the requested file, keep parsing blocks until we run
	// out of file, then close it.
	filename_ = filename;
	fin.open(filename + ".jpg", ios_base::binary);
	if (!fin.is_open())
	{
		cerr << "File doesn't exit";
		exit(EXIT_FAILURE);
	}
	while (parseSeg() == JPEG_SEG_OK);
	fin.close();
}