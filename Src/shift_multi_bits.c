#include "include.h"

void shift_left(uint8_t *array, uint8_t len)
{
	for(uint8_t i=0; i<len; i++)
	{
		*(array+i) <<= 1;
		if(i<len-1)
		{
			if(*(array+i+1) & 0x80)
			{
				*(array+i) |= 0x01;
			}
		}
	}
}

void shift_right(uint8_t *array, uint8_t len)
{
	for(uint8_t i=0; i<len; i++)
	{
		*(array+len-1-i) >>= 1;
		if(i<len-1)
		{
			if(*(array+len-1-i-1) & 0x01)
			{
				*(array+len-1-i) |= 0x80;
			}
		}
	}
}

void shift_left_around(uint8_t *array, uint8_t len)
{
	bool temp = false;
	if(*array & 0x80)// overflow if shift left -> move to LSB of array[len-1]
	{
		temp = true;
	}
	for(uint8_t i=0; i<len; i++)
	{
		*(array+i) <<= 1;
		if(i<len-1)
		{
			if(*(array+i+1) & 0x80)
			{
				*(array+i) |= 0x01;
			}
		}
	}
	if(temp)
	{
		*(array+len-1) |= 0x01;
	}
}

void shift_right_around(uint8_t *array, uint8_t len)
{
	bool temp = false;
	if(*(array+len-1) & 0x01)// lose bit if shift right -> move to MSB of array[len-1]
	{
		temp = true;
	}
	for(uint8_t i=0; i<len; i++)
	{
		*(array+len-1-i) >>= 1;
		if(i<len-1)
		{
			if(*(array+len-1-i-1) & 0x01)
			{
				*(array+len-1-i) |= 0x80;
			}
		}
	}
	if(temp)
	{
		*array |= 0x80;
	}
}

int reverse_bits_recursive(unsigned int num, unsigned int numBits)
{
    unsigned int reversedNum;;
    unsigned int mask = 0;

    mask = (0x1 << (numBits/2)) - 1;

    if (numBits == 1) return num;
    reversedNum = reverse_bits_recursive(num >> numBits/2, numBits/2) |
                   reverse_bits_recursive((num & mask), numBits/2) << numBits/2;
    return reversedNum;
}

void shift_left_arround_column_2d_arr(uint8_t *array, uint8_t column, uint8_t height, uint8_t width)
{
	// point to this column in 2 dimentions array  
	uint8_t *p_data = array+column;
	uint8_t *temp		= (uint8_t *)calloc(height, sizeof(uint8_t));
	// printf("Data in column %d in scroll array\r\n", column);
	for(uint8_t i=0; i<height; i++)
	{
		// printf("0x%02x\r\n", *(p_data + i*width));
		*(temp+i) = *(p_data + i*width);	
	}
	shift_left_around(temp, height);
	for(uint8_t i=0; i<height; i++)
	{
		// printf("0x%02x\r\n", *(p_data + i*width));
		*(p_data + i*width) = *(temp+i);	
	}
	free(temp);
}

void shift_left_arround_2d_arr(uint8_t *array, uint8_t height, uint8_t width)
{
	for(uint8_t i=0; i<width; i++)
	{
		shift_left_arround_column_2d_arr(array, i, height, width);
	}
}

void shift_right_arround_column_2d_arr(uint8_t *array, uint8_t column, uint8_t height, uint8_t width)
{
	// point to this column in 2 dimentions array  
	uint8_t *p_data = array+column;
	uint8_t *temp		= (uint8_t *)calloc(height, sizeof(uint8_t));
	// printf("Data in column %d in scroll array\r\n", column);
	for(uint8_t i=0; i<height; i++)
	{
		*(temp+i) = *(p_data + i*width);	
	}
	shift_right_around(temp, height);
	for(uint8_t i=0; i<height; i++)
	{
		*(p_data + i*width) = *(temp+i);	
	}
	free(temp);
}

void shift_right_arround_2d_arr(uint8_t *array, uint8_t height, uint8_t width)
{
	for(uint8_t i=0; i<width; i++)
	{
		shift_right_arround_column_2d_arr(array, i, height, width);
	}
}
