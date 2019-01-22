#ifndef FONT_H
#define FONT_H

#define FONT_27x24					0
#define FONT_27x24_WIDTH		27
#define FONT_27x24_HEIGHT		24

#define FONT_19x24					1
#define FONT_19x24_WIDTH		19
#define FONT_19x24_HEIGHT		24

#define FONT_7x24						2
#define FONT_7x24_WIDTH			7
#define FONT_7x24_HEIGHT		24

#define FONT_18x16					3
#define FONT_18x16_WIDTH		18
#define FONT_18x16_HEIGHT		16

#define FONT_11x16					4
#define FONT_11x16_WIDTH		11
#define FONT_11x16_HEIGHT		16

#define FONT_TOTAL					5

typedef struct font font_t;

typedef struct{
	const uint8_t *p_font;
	uint8_t width;
	uint8_t height;
	uint8_t dummy_height;
}font_manager;

struct font{
	struct font_param{
		font_manager *font_name;
	} param;
};

void font_init(font_t *self);

#endif
