#include "jpeg.h"



int main(int argc, char* argv[])
{
	int a = 40 / 16;
	if (40 % 16 != 0)
	{
		a++;
	}
	cout << a;
	JPEG jpeg(argv[1]);




	//WORD a = 0x0006;
	//WORD b = 0x0000;
	//int code_length = 5;
	//int a1 = complement2original(a, code_length);
	//int b1 = complement2original(b, code_length);
	//cout << a << endl << a1 << endl;
	//cout << b << endl << b1 << endl;
}

