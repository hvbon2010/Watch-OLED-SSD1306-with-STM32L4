#ifndef SHIFT_MULTI_BITS_H
#define SHIFT_MULTI_BITS_H

void shift_left(uint8_t *array, uint8_t len);

void shift_right(uint8_t *array, uint8_t len);

void shift_left_around(uint8_t *array, uint8_t len);

void shift_right_around(uint8_t *array, uint8_t len);

void shift_left_arround_column_2d_arr(uint8_t *array, uint8_t column, uint8_t height, uint8_t width);

int reverse_bits_recursive(unsigned int num, unsigned int numBits);

void shift_left_arround_2d_arr(uint8_t *array, uint8_t height, uint8_t width);

void shift_right_arround_column_2d_arr(uint8_t *array, uint8_t column, uint8_t height, uint8_t width);

void shift_right_arround_2d_arr(uint8_t *array, uint8_t height, uint8_t width);

#endif
