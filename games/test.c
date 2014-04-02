#include "../framework.h"

const uint8_t SPL_COLOR_64[8][8] =
		{
			{0b000000,0b000001,0b000010,0b000011,0b000100,0b000101,0b000110,0b000111},
			{0b001000,0b001001,0b001010,0b001011,0b001100,0b001101,0b001110,0b001111},
			{0b010000,0b010001,0b010010,0b010011,0b010100,0b010101,0b010110,0b010111},
			{0b011000,0b011001,0b011010,0b011011,0b011100,0b011101,0b011110,0b011111},
			{0b100000,0b100001,0b100010,0b100011,0b100100,0b100101,0b100110,0b100111},
			{0b101000,0b101001,0b101010,0b101011,0b101100,0b101101,0b101110,0b101111},
			{0b110000,0b110001,0b110010,0b110011,0b110100,0b110101,0b110110,0b110111},
			{0b111000,0b111001,0b111010,0b111011,0b111100,0b111101,0b111110,0b111111}
		};
#define SPLASH SPL_COLOR_64

void initGame()
{
}

uint8_t color1, color2;

void gameLoop()
{
	if (buttonsHit) {
		color1 = getRandom(7) + 1;
		color2 = getRandom(8) & ~color1;
		buttonsHit = 0;
	}

	uint8_t line = frameCounter % 16;

	uint8_t i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			uint8_t lineDist = ((i <= 3 ? 3 - i : i - 4) - j - line + 4) & 7;
			uint8_t bright = lineDist <= 3 ? lineDist : 7 - lineDist;
			uint8_t color = 0;
			if (color1 & 0b001) { color |= bright % 4; }
			if (color1 & 0b010) { color |= bright % 4 << 2; }
			if (color1 & 0b100) { color |= bright % 4 << 4; }
			if (color2 & 0b001) { color |= (3 - bright) % 4; }
			if (color2 & 0b010) { color |= (3 - bright) % 4 << 2; }
			if (color2 & 0b100) { color |= (3 - bright) % 4 << 4; }
			display[i][j] = color;
		}
	}
}
