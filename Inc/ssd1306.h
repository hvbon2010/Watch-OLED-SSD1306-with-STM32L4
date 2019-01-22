/*----------------------------------------------------*-
	SSD1306 OLED 0.96 Inch
	Author: Bon Van Ho
	Date:		2/7/2018
-*----------------------------------------------------*/

#ifndef SSD1306_H
#define SSD1306_H

#define SSD1306_ADDRESS					0x78
#define SCROLL_NUMBER_IMMEDIATE	6

#define SCROLL_SEC_UNIT					0
#define SCROLL_SEC_DEC					1
#define SCROLL_MIN_UNIT					2
#define SCROLL_MIN_DEC					3
#define SCROLL_HOUR_UNIT				4
#define SCROLL_HOUR_DEC					5
#define SCROLL									true
#define	NO_SCROLL								false
	
typedef struct ssd1306	ssd1306_t;

typedef struct{
	uint8_t font_name;
	uint8_t num;
	uint8_t column;
	uint8_t row;
	uint8_t scroll_num;
	bool 		scroll;
} scroll_number;

typedef struct{
	uint8_t row;
	uint8_t column;
	bool 		light;
} pixel;

struct ssd1306{
	struct ssd1306_param{
		pixel 	m_pixel;
		uint8_t contrast;
		scroll_number *scroll_num;
	} param;
	struct ssd1306_api{
		void (*write_command)(uint8_t *data, uint8_t len);
		void (*write_data)(uint8_t *data, uint8_t len);
		void (*select_constract)(ssd1306_t *self);
		void (*display_off)(ssd1306_t *self);
		void (*display_on)(ssd1306_t *self);
		void (*set_pixel)(ssd1306_t *self, uint8_t column, uint8_t row);
		void (*clear_8bit_pixel)(ssd1306_t *self);
		void (*set_position)(ssd1306_t *self, uint8_t column, uint8_t row);
		void (*over_pixel)(ssd1306_t *self);
		void (*display_number)(ssd1306_t *self, uint8_t font_num, uint8_t num, uint8_t column, uint8_t row);
		void (*scroll_number_repare)(uint8_t *array, uint8_t font_num, uint8_t num);
		void (*create_scroll_array)(uint8_t *array, uint8_t font_name);
		void (*scroll_number_only)(ssd1306_t *self, uint8_t *array, uint8_t font_num, uint8_t num, uint8_t column, uint8_t row);
		void (*scroll_number_all)(ssd1306_t *self);
	}api;
};

void ssd1306_init(ssd1306_t *self);

void write_command(uint8_t *data, uint8_t len);

void write_data(uint8_t *data, uint8_t len);

void select_constract(ssd1306_t *self);

void display_off(ssd1306_t *self);

void display_on(ssd1306_t *self);

void set_pixel(ssd1306_t *self, uint8_t column, uint8_t row);

void clear_8bit_pixel(ssd1306_t *self);

void set_position(ssd1306_t *self, uint8_t column, uint8_t row);

void over_pixel(ssd1306_t *self);

void display_number(ssd1306_t *self, uint8_t font_num, uint8_t num, uint8_t column, uint8_t row);

void scroll_number_repare(uint8_t *array, uint8_t font_num, uint8_t num);

void create_scroll_array(uint8_t *array, uint8_t font_name);

void scroll_number_only(ssd1306_t *self, uint8_t *array, uint8_t font_num, uint8_t num, uint8_t column, uint8_t row);

void scroll_number_all(ssd1306_t *self);

#endif
