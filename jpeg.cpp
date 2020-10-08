#include "jpeg.h"

int complement2original(WORD complement, int bit_code_length)
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
		return int(complement);
	}
}



int main(int argc, char* argv[])
{
	JPEG jpeg(argv[1]);




	//WORD a = 0x0006;
	//WORD b = 0x0000;
	//int code_length = 5;
	//int a1 = complement2original(a, code_length);
	//int b1 = complement2original(b, code_length);
	//cout << a << endl << a1 << endl;
	//cout << b << endl << b1 << endl;
}

