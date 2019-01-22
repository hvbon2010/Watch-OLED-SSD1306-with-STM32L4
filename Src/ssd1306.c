/*----------------------------------------------------*-
	SSD1306 OLED 0.96 Inch
	Author: Bon Van Ho
	Date:		2/7/2018
-*----------------------------------------------------*/

#include "include.h"

ssd1306_t m_ssd1306;

extern font_t m_font;

extern I2C_HandleTypeDef hi2c1;

uint8_t array_sec_unit[(FONT_11x16_HEIGHT/8*2+1)*FONT_11x16_WIDTH];
uint8_t array_sec_dec[(FONT_11x16_HEIGHT/8*2+1)*FONT_11x16_WIDTH];
uint8_t array_min_unit[(FONT_19x24_HEIGHT/8*2+1)*FONT_19x24_WIDTH];
uint8_t array_min_dec[(FONT_19x24_HEIGHT/8*2+1)*FONT_19x24_WIDTH];	
uint8_t array_hour_unit[(FONT_19x24_HEIGHT/8*2+1)*FONT_19x24_WIDTH];
uint8_t array_hour_dec[(FONT_19x24_HEIGHT/8*2+1)*FONT_19x24_WIDTH];
	
void ssd1306_init(ssd1306_t *self)
{
	self->param.scroll_num 	= (scroll_number *) calloc(SCROLL_NUMBER_IMMEDIATE, sizeof(scroll_number));
	self->param.scroll_num[SCROLL_SEC_UNIT].font_name 	= FONT_11x16;
	self->param.scroll_num[SCROLL_SEC_DEC].font_name 		= FONT_11x16;
	self->param.scroll_num[SCROLL_MIN_UNIT].font_name 	= FONT_19x24;
	self->param.scroll_num[SCROLL_MIN_DEC].font_name 		= FONT_19x24;	
	self->param.scroll_num[SCROLL_HOUR_UNIT].font_name 	= FONT_19x24;
	self->param.scroll_num[SCROLL_HOUR_DEC].font_name 	= FONT_19x24;
	for(uint8_t i=0; i<SCROLL_NUMBER_IMMEDIATE; i++)
	{
		self->param.scroll_num[i].scroll = NO_SCROLL;
	}
	self->param.scroll_num[SCROLL_SEC_UNIT].column 	= 115;
	self->param.scroll_num[SCROLL_SEC_UNIT].row     = 32;
	self->param.scroll_num[SCROLL_SEC_DEC].column		= 101;
	self->param.scroll_num[SCROLL_SEC_DEC].row			= 32;
	self->param.scroll_num[SCROLL_MIN_UNIT].column	= 79;
	self->param.scroll_num[SCROLL_MIN_UNIT].row			= 24;
	self->param.scroll_num[SCROLL_MIN_DEC].column		= 57;
	self->param.scroll_num[SCROLL_MIN_DEC].row			= 24;
	self->param.scroll_num[SCROLL_HOUR_UNIT].column	= 25;
	self->param.scroll_num[SCROLL_HOUR_UNIT].row		= 24;
	self->param.scroll_num[SCROLL_HOUR_DEC].column	= 3;
	self->param.scroll_num[SCROLL_HOUR_DEC].row			= 24;
	
	self->api.write_command = &write_command;
	self->api.write_data		= &write_data;
	self->api.set_pixel			= &set_pixel;
	self->api.clear_8bit_pixel	= &clear_8bit_pixel;
	self->api.select_constract	= &select_constract;
	self->api.over_pixel				= &over_pixel;
	self->api.set_position			= &set_position;
	self->api.display_on				= &display_on;
	self->api.display_off				= &display_off;
	self->api.display_number		= &display_number;
	self->api.scroll_number_repare			= &scroll_number_repare;
	self->api.create_scroll_array				= &create_scroll_array;
	self->api.scroll_number_only				= &scroll_number_only;
	self->api.scroll_number_all					= &scroll_number_all;
	
	uint8_t data[29] = { 	0xAE, //display off
												0x20, //Set Memory Addressing Mode   
												0x10, //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
												0xB0, //Set Page Start Address for Page Addressing Mode,0-7
												0xC8, //Set COM Output Scan Direction
												0x00, //---set low column address
												0x10, //---set high column address
												0x40, //--set start line address
												0x81, //--set contrast control register
												0x00,
												0xA1, //--set segment re-map 0 to 127
												0xA6, //--set normal display
												0xA8, //--set multiplex ratio(1 to 64)
												0x3F, //
												0xA4, //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
												0xD3, //-set display offset
												0x00, //-not offset
												0xD5, //--set display clock divide ratio/oscillator frequency
												0xF0, //--set divide ratio
												0xD9, //--set pre-charge period
												0x22, //
												0xDA, //--set com pins hardware configuration
												0x12,
												0xDB, //--set vcomh
												0x20, //0x20,0.77xVcc
												0x8D, //--set DC-DC enable
												0x14, //
												0xAF,	//--turn on SSD1306 panel
										}; 
	uint8_t *p_data = data;
	for(uint8_t i=0; i<29; i++)
	{
		write_command((p_data+i), 1);
	}
}

void write_command(uint8_t *data, uint8_t len)
{
	HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDRESS, 0x00, 1, data, len, 1000);
}

void write_data(uint8_t *data, uint8_t len)
{
	HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDRESS, 0x40, 1, data, len, 1000);
}

void select_constract(ssd1306_t *self)
{
	uint8_t data[2] = {0x81,self->param.contrast};
	self->api.write_command(data, 2);
}

void display_off(ssd1306_t *self)
{
	uint8_t data = 0xAE;
	self->api.write_command(&data, 1);
}

void display_on(ssd1306_t *self)
{
	uint8_t data = 0xAF;
	self->api.write_command(&data, 1);	
}

void set_pixel(ssd1306_t *self, uint8_t column, uint8_t row)
{
	// Select Memory Addressing Mode
	uint8_t data[2] = {0x20,
										 0x02, // Page Adressing Mode
										};
	uint8_t lower_nib_col = column&0x0F;
	uint8_t upper_nib_col = ((column>>4)&0x0F)|0x10;
	self->api.write_command(data, 2);
	// Set Page Start Address B0 - B7H
	data[0] = row/8 + 0xB0;
	self->api.write_command(data, 1);
	// Set Start column Address
	self->api.write_command(&lower_nib_col, 1);
	self->api.write_command(&upper_nib_col, 1);
	// Write data(Select Pixel in 8 Bit Pixel)
	data[0] = row%8;
	data[0] = pow(2, (double) data[0]);
	self->api.write_data(data, 1);
}

void clear_8bit_pixel(ssd1306_t *self)
{
	// Select Memory Addressing Mode
	uint8_t data[2] = {0x20,
										 0x02, // Page Adressing Mode
										};
	uint8_t lower_nib_col = self->param.m_pixel.column&0x0F;
	uint8_t upper_nib_col = ((self->param.m_pixel.column>>4)&0x0F)|0x10;
	self->api.write_command(data, 2);
	// Set Page Start Address B0 - B7H
	data[0] = self->param.m_pixel.row/8 + 0xB0;
	write_command(data, 1);
	// Set Start column Address
	write_command(&lower_nib_col, 1);
	write_command(&upper_nib_col, 1);
	// Write data(Select Pixel in 8 Bit Pixel)
	data[0] = 0;
	self->api.write_data(data, 1);
}

void set_position(ssd1306_t *self, uint8_t column, uint8_t row)
{
	// Select Memory Addressing Mode
	uint8_t data[2] = {0x20,
										 0x02, // Page Adressing Mode
										};
	uint8_t lower_nib_col = column&0x0F;
	uint8_t upper_nib_col = ((column>>4)&0x0F)|0x10;
	self->api.write_command(data, 2);
	// Set Page Start Address B0 - B7H
	data[0] = row/8 + 0xB0;
	self->api.write_command(data, 1);
	// Set Start column Address
	self->api.write_command(&lower_nib_col, 1);
	self->api.write_command(&upper_nib_col, 1);
}

void over_pixel(ssd1306_t *self)
{
	self->api.set_position(self, 0, 0);
	// Select Memory Addressing Mode
	uint8_t data[2] = {0x20,
										 0x00, // Horizontial Adressing Mode
										};
	self->api.write_command(data, 2);
	for(uint8_t row = 0; row < 64; row = row+8)
	{
		for(uint8_t column = 0; column < 128; column++)
		{
			uint8_t data = 0x00;
			self->api.write_data(&data, 1);
		}
	}
}

void display_number(ssd1306_t *self, uint8_t font_num, uint8_t num, uint8_t column, uint8_t row)
{
	self->api.set_position(self, column, row);
	// Write data(8 Bit Pixel)
	const uint8_t *p_data;
	p_data = m_font.param.font_name[font_num].p_font + 
		m_font.param.font_name[font_num].width * (m_font.param.font_name[font_num].height / 8 
	+ m_font.param.font_name[font_num].dummy_height) * num;
//	printf("address of p_font point to font_18x16: %d\r\n", m_font.param.font_name[font_num].p_font);
//	printf("address of p_data point to font_18x16: %d\r\n", p_data);
	for(uint8_t i = 0; i < m_font.param.font_name[font_num].width * (m_font.param.font_name[font_num].height / 8); i++)
	{
		if(i%m_font.param.font_name[font_num].width == 0 && i != 0)
		{
			row += 8;
			self->api.set_position(self, column, row);
		}
		uint8_t temp_data = *(p_data+i);
		printf("0x%02x ", temp_data);		
		self->api.write_data(&temp_data, 1);
	}
}

void scroll_number_repare(uint8_t *array, uint8_t font_num, uint8_t num)
{
	// repare scroll array
	memset(array, 0x00, sizeof((char *)array));
	if(num == 0)
	{
		memcpy(array, m_font.param.font_name[font_num].p_font + 
			m_font.param.font_name[font_num].width * (m_font.param.font_name[font_num].height / 8 
		+ m_font.param.font_name[font_num].dummy_height) * 9,
		(m_font.param.font_name[font_num].height / 8 + m_font.param.font_name[font_num].dummy_height) * m_font.param.font_name[font_num].width);

		memcpy((char *)(array + (m_font.param.font_name[font_num].height / 8 
			+ m_font.param.font_name[font_num].dummy_height) * m_font.param.font_name[font_num].width)
			, (char *)m_font.param.font_name[font_num].p_font,
			(m_font.param.font_name[font_num].height / 8) * m_font.param.font_name[font_num].width); 
	}
	else
	{
		memcpy(array, m_font.param.font_name[font_num].p_font + 
			m_font.param.font_name[font_num].width * (m_font.param.font_name[font_num].height / 8 
		+ m_font.param.font_name[font_num].dummy_height) * (num - 1), (m_font.param.font_name[font_num].height / 8 * 2
		+ m_font.param.font_name[font_num].dummy_height) * m_font.param.font_name[font_num].width);
	}
	for (uint16_t i = 0; i < (m_font.param.font_name[font_num].height/8*2
	+ m_font.param.font_name[font_num].dummy_height) * m_font.param.font_name[font_num].width; i++)
	{
		*(array+i) = (uint8_t) reverse_bits_recursive(*(array+i), 8);
	}
}

void create_scroll_array(uint8_t *array, uint8_t font_name)
{
	switch(font_name)
	{
		case FONT_27x24:
			array = (uint8_t *) calloc((FONT_27x24_HEIGHT/8*2+1)*FONT_27x24_WIDTH, sizeof(uint8_t));
			if(array == NULL)
				Error_Handler();
			break;
		case FONT_19x24:
			array = (uint8_t *) calloc((FONT_19x24_HEIGHT/8*2+1)*FONT_19x24_WIDTH, sizeof(uint8_t));
			if(array == NULL)
				Error_Handler();
			break;
		case FONT_7x24:
			array = (uint8_t *) calloc((FONT_7x24_HEIGHT/8*2+1)*FONT_7x24_WIDTH, sizeof(uint8_t));
			if(array == NULL)
				Error_Handler();
			break;
		case FONT_18x16:
			array = (uint8_t *) calloc((FONT_18x16_HEIGHT/8*2+1)*FONT_18x16_WIDTH, sizeof(uint8_t));
			if(array == NULL)
				Error_Handler();
			break;
		case FONT_11x16:
			array = (uint8_t *) calloc((FONT_11x16_HEIGHT/8*2+1)*FONT_11x16_WIDTH, sizeof(uint8_t));
			if(array == NULL)
				Error_Handler();
			break;
	}	
}

void scroll_number_only(ssd1306_t *self, uint8_t *array, uint8_t font_num, uint8_t num, uint8_t column, uint8_t row)
{
	uint8_t temp_row = 0;
	temp_row = row;
	uint8_t lower_nib_col = column&0x0F;
	uint8_t uper_nib_col = ((column>>4)&0x0F)|0x10;
	// Set Page Start Address B0 - B7H
	uint8_t data = temp_row/8 + 0xB0;
	self->api.write_command(&data, 1);
	// Set Start column Address
	self->api.write_command(&lower_nib_col, 1);
	self->api.write_command(&uper_nib_col, 1);
	// Write data(8 Bit Pixel)
	shift_left_arround_2d_arr(array, (m_font.param.font_name[font_num].height / 8 * 2+
		m_font.param.font_name[font_num].dummy_height), m_font.param.font_name[font_num].width);
	for(uint8_t i=0; i<m_font.param.font_name[font_num].width*(m_font.param.font_name[font_num].height/8); i++)
	{
		if(i%m_font.param.font_name[font_num].width == 0 && i != 0)
		{
			temp_row += 8;
			self->api.set_position(self, column, temp_row);
		}
		uint8_t num = reverse_bits_recursive(*(array+i), 8);
		self->api.write_data(&num, 1);
	}
}

void scroll_number_all(ssd1306_t *self)
{
	// Select Memory Addressing Mode
	uint8_t data[2] = {0x20,
										 0x02, // Page Adressing Mode
										};
	self->api.write_command(data, 2);
//	printf("scroll number all start\r\n");
//	uint8_t *array_sec_unit;
	if(self->param.scroll_num[SCROLL_SEC_UNIT].scroll == SCROLL)
	{
//		printf("scroll second unit init\r\n");
//		self->api.create_scroll_array(array_sec_unit, self->param.scroll_num[SCROLL_SEC_UNIT].font_name);
//		self->api.scroll_number_repare(array_sec_unit, self->param.scroll_num[SCROLL_SEC_UNIT].font_name,
//			self->param.scroll_num[SCROLL_SEC_UNIT].num);		
//		self->param.scroll_num[SCROLL_SEC_UNIT].scroll_num = 0;
		self->api.scroll_number_repare(array_sec_unit, self->param.scroll_num[SCROLL_SEC_UNIT].font_name,
			self->param.scroll_num[SCROLL_SEC_UNIT].num);		
		self->param.scroll_num[SCROLL_SEC_UNIT].scroll_num = 0;
	}
	if(self->param.scroll_num[SCROLL_SEC_DEC].scroll == SCROLL)
	{
		self->api.scroll_number_repare(array_sec_dec, self->param.scroll_num[SCROLL_SEC_DEC].font_name,
			self->param.scroll_num[SCROLL_SEC_DEC].num);		
		self->param.scroll_num[SCROLL_SEC_DEC].scroll_num = 0;	
	}
	if(self->param.scroll_num[SCROLL_MIN_UNIT].scroll == SCROLL)
	{
//		printf("scroll minute unit init\r\n");
//		self->api.create_scroll_array(array_min_unit, self->param.scroll_num[SCROLL_MIN_UNIT].font_name);
		self->api.scroll_number_repare(array_min_unit, self->param.scroll_num[SCROLL_MIN_UNIT].font_name,
			self->param.scroll_num[SCROLL_MIN_UNIT].num);	
		self->param.scroll_num[SCROLL_MIN_UNIT].scroll_num = 0;
	}
	if(self->param.scroll_num[SCROLL_MIN_DEC].scroll == SCROLL)
	{
//		printf("scroll minute dec init\r\n");
//		self->api.create_scroll_array(array_min_dec, self->param.scroll_num[SCROLL_MIN_DEC].font_name);
		self->api.scroll_number_repare(array_min_dec, self->param.scroll_num[SCROLL_MIN_DEC].font_name,
			self->param.scroll_num[SCROLL_MIN_DEC].num);	
		self->param.scroll_num[SCROLL_MIN_DEC].scroll_num = 0;
	}
	if(self->param.scroll_num[SCROLL_HOUR_UNIT].scroll == SCROLL)
	{
//		printf("scroll hour unit init\r\n");
//		self->api.create_scroll_array(array_hour_unit, self->param.scroll_num[SCROLL_HOUR_UNIT].font_name);
		self->api.scroll_number_repare(array_hour_unit, self->param.scroll_num[SCROLL_HOUR_UNIT].font_name,
			self->param.scroll_num[SCROLL_HOUR_UNIT].num);	
		self->param.scroll_num[SCROLL_HOUR_UNIT].scroll_num = 0;
	}
	if(self->param.scroll_num[SCROLL_HOUR_DEC].scroll == SCROLL)
	{
//		printf("scroll hour dec init\r\n");
//		self->api.create_scroll_array(array_hour_dec, self->param.scroll_num[SCROLL_HOUR_DEC].font_name);
		self->api.scroll_number_repare(array_hour_dec, self->param.scroll_num[SCROLL_HOUR_DEC].font_name,
			self->param.scroll_num[SCROLL_HOUR_DEC].num);	
		self->param.scroll_num[SCROLL_HOUR_DEC].scroll_num = 0;
	}
	while(1)
	{
//		printf("loop\t");
		for(uint8_t num = 0; num < SCROLL_NUMBER_IMMEDIATE; num++)
		{
			if(self->param.scroll_num[num].scroll == SCROLL)
			{
				if(self->param.scroll_num[num].scroll_num++ 
				< m_font.param.font_name[self->param.scroll_num[num].font_name].height + 8) 
				{
					uint8_t *p_data;
					switch(num)
					{
						case 0:
							p_data = array_sec_unit;
							break;
						case 1:
							p_data = array_sec_dec;
							break;
						case 2:
							p_data = array_min_unit;
							break;
						case 3:
							p_data = array_min_dec;
							break;
						case 4:
							p_data = array_hour_unit;
							break;
						case 5:
							p_data = array_hour_dec;
							break;												
					}
					self->api.scroll_number_only(self, p_data, self->param.scroll_num[num].font_name, 
						self->param.scroll_num[num].num, self->param.scroll_num[num].column, self->param.scroll_num[num].row);
				}
				else
				{
//					printf("Scroll done\r\n");
					self->param.scroll_num[num].scroll = NO_SCROLL;
				}
			}
			else if(self->param.scroll_num[SCROLL_SEC_UNIT].scroll	== NO_SCROLL &&
							self->param.scroll_num[SCROLL_SEC_DEC].scroll		== NO_SCROLL &&
							self->param.scroll_num[SCROLL_MIN_UNIT].scroll 	== NO_SCROLL &&
							self->param.scroll_num[SCROLL_MIN_DEC].scroll 	== NO_SCROLL &&
							self->param.scroll_num[SCROLL_HOUR_UNIT].scroll == NO_SCROLL &&
							self->param.scroll_num[SCROLL_HOUR_DEC].scroll 	== NO_SCROLL)
			{
//					printf("exit while\r\n");
					return;
			}
		}
		osDelay(15);
	}
}
