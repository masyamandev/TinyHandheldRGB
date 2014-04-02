#include "../framework.h"

#define IMAGES 4

uint8_t image;

void initGame()
{
	image = 0;
}


void gameLoop()
{
	if (buttonsHit & (BUTTON_RIGHT | BUTTON_DOWN)) {
		image = (image + 1) % IMAGES;
		buttonsHit = 0;
	}
	if (buttonsHit & (BUTTON_LEFT | BUTTON_UP)) {
		image = (image + IMAGES - 1) % IMAGES;
		buttonsHit = 0;
	}
	uint8_t y, x;
	for (y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for (x = 0; x < DISPLAY_WIDTH; x++) {
			display[x][y] = x + y * 8 + (image << 6);
		}
	}
}
