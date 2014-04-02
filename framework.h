#include <stdint.h>

#define COLOR_BLACK 		0
#define COLOR_DARK_GRAY 	0b010101
#define COLOR_BRIGHT_GRAY 	0b101010
#define COLOR_WHITE 		0b111111

#define DISPLAY_WIDTH 8
#define DISPLAY_HEIGHT 8

#define BUTTON_UP		0b00000010
#define BUTTON_DOWN		0b00000100
#define BUTTON_LEFT		0b00001000
#define BUTTON_RIGHT	0b00000001
#define BUTTON_A		0b00010000
#define BUTTON_B		0b00100000
#define BUTTONS_ALL		0b00111111

// Counter of frame. Before each call of gameLoop() this counter will be incremented.
uint32_t frameCounter;

// Display data.
uint8_t display[DISPLAY_WIDTH][DISPLAY_HEIGHT];

// Currently pressed buttons
volatile uint8_t buttonsPressed;
// Recently pressed buttons
volatile uint8_t buttonsHit;

// Set length of frame in milliseconds
void setFrameLength(uint32_t msec);

// Get random value
uint8_t getRandom(uint8_t maxValue);

// Show 1 bit screen
void showScreen(uint8_t screen[DISPLAY_WIDTH][DISPLAY_HEIGHT]);

// Clear screen
void clearScreen();
