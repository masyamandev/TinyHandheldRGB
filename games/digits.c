#include "../framework.h"

#define IMAGES 10

#include "images/digits6.c"

//const uint8_t COLORS[IMAGES] = {
//		0b111111, //0
//		0b110110, //1
//		0b001111, //2
//		0b110011, //3
//		0b001011, //4
//		0b000011, //5
//		0b001100, //6
//		0b010111, //7
//		0b110100, //8
//		0b111100, //9
//};
const uint8_t COLORS[IMAGES] = {
		0b111111, //0
		0b000011, //5
		0b001011, //4
		0b001111, //2
		0b001100, //6
		0b111100, //9
		0b110100, //8
		0b110110, //1
		0b110011, //3
		0b010111, //7
};

void initGame()
{
	setFrameLength(100);
}


//void gameLoop()
//{
//	if (buttonsHit & (BUTTON_RIGHT | BUTTON_DOWN)) {
//		image = (image + 1) % IMAGES;
//		buttonsHit = 0;
//	}
//	if (buttonsHit & (BUTTON_LEFT | BUTTON_UP)) {
//		image = (image + IMAGES - 1) % IMAGES;
//		buttonsHit = 0;
//	}
//	uint8_t y, x;
//	for (y = 0; y < DISPLAY_HEIGHT; y++)
//	{
//		for (x = 0; x < DISPLAY_WIDTH; x++) {
//			display[DISPLAY_WIDTH - 1 - x][y] = (DIGITS5[image][y] & (1 << x)) ? COLORS[image] : 0;
//		}
//	}
//}

void gameLoop()
{
	if (buttonsPressed) {
		uint8_t y, x;
		for (y = 0; y < DISPLAY_HEIGHT; y++)
		{
			for (x = 0; x < DISPLAY_WIDTH; x++) {
				uint8_t color = y / 3 * 4 + x / 2;
				display[x][y] = color >= IMAGES ? 0 : COLORS[color];
			}
		}
		return;
	}
	uint8_t dx = frameCounter % (DIGIT_WIDTH * IMAGES);
	uint8_t y, x;
	for (y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for (x = 0; x < DISPLAY_WIDTH; x++) {
			uint8_t image = (x + dx) / DIGIT_WIDTH % IMAGES;
			uint8_t dxi = DIGIT_WIDTH - 1 - (x + dx) % DIGIT_WIDTH;
			display[x][y] = (DIGITS5[image][y] & (1 << dxi)) ? COLORS[image] : 0;
		}
	}
}

