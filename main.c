#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//#include "colorHelpers.c"

#define DDR_MASK 0b11111111

#define MAX_BRIGHT 3

//#define SPEED_MEASURE

//#include "games/test.c"
#include "games/snake.c"
//#include "games/icons.c"
//#include "games/digits.c"
//#include "games/colors.c"

uint16_t frameLength;
volatile uint16_t timeCounter;
volatile uint32_t cycleCounter;

volatile uint16_t randomSeed;

volatile uint8_t buttonsState;

#ifdef SPEED_MEASURE
	volatile uint8_t interruptTimerStart;
	volatile uint8_t interruptTimerEnd;
#endif


uint8_t displayRaw[DISPLAY_HEIGHT * 3][MAX_BRIGHT];
uint8_t displayLine;
uint8_t displayBright;


void setFrameLength(uint32_t msec)
{
	frameLength = msec * F_CPU / 1000 / 256;
}

void clearScreen()
{
	uint8_t y, x;
	for (y = 0; y < DISPLAY_HEIGHT; y++)
		for (x = 0; x < DISPLAY_WIDTH; x++)
			display[x][y] = COLOR_BLACK;
}

void showScreen(uint8_t screen[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	uint8_t y, x;
	for (y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for (x = 0; x < DISPLAY_WIDTH; x++)
		{
			//display[x][y] = ((screen[y] & (1 << (7 - x))) != 0) ? COLOR_WHITE : COLOR_BLACK;
			display[x][y] = screen[y][x];
		}
	}
}

void refreshDisplay()
{
	uint8_t i, j, b;
	uint8_t rawBit;
	for (j = 0, rawBit = 0b10000000; j < DISPLAY_WIDTH; j++, rawBit >>= 1) // x
	{
//		uint8_t rawBit = 1 << (7 - j);
		uint8_t rawBitN = ~rawBit;
		for (i = 0; i < DISPLAY_HEIGHT; i++) // y
		{
			uint8_t yr = DISPLAY_HEIGHT - 1 - i; // back i
			uint8_t yg = DISPLAY_HEIGHT + i; // forward i + DISPLAY_HEIGHT
			uint8_t yb = DISPLAY_HEIGHT * 3 - 1 - i; // back i + DISPLAY_HEIGHT * 2

			uint8_t color = display[j][i];
			uint8_t colorR, colorG, colorB;

			colorR = color & 0b11;
			colorG = (color >> 2) & 0b11;
			colorB = (color >> 4) & 0b11;

			for (b = 0; b < MAX_BRIGHT; b++) // brightness
			{
				// R
				if (colorR > b) {
					displayRaw[yr][b] |= rawBit;
				} else {
					displayRaw[yr][b] &= rawBitN;
				}
				// G
				if (colorG > b) {
					displayRaw[yg][b] |= rawBit;
				} else {
					displayRaw[yg][b] &= rawBitN;
				}
				// B
				if (colorB > b) {
					displayRaw[yb][b] |= rawBit;
				} else {
					displayRaw[yb][b] &= rawBitN;
				}
			}

//			switch (color & 0b11000000) {
//			case 0b00000000:
//				colorR = colorChannel2to3bits(color & 0b11);
//				colorG = colorChannel2to3bits((color >> 2) & 0b11);
//				colorB = colorChannel2to3bits((color >> 4) & 0b11);
//				break;
//			case 0b01000000:
//				bright = (color >> 3);// & 0b111;
//				colorR = color & 0b001 ? bright : 0;
//				colorG = color & 0b010 ? bright : 0;
//				colorB = color & 0b100 ? bright : 0;
//				break;
//			}
//
//			uint8_t bBit;
//			for (b = 0, bBit = 0b001; b < MAX_BRIGHT; b++, bBit <<= 1) // brightness
//			{
//				// R
//				if ((colorR) & bBit) {
//					displayRaw[yr][b] |= rawBit;
//				} else {
//					displayRaw[yr][b] &= rawBitN;
//				}
//				// G
//				if ((colorG) & bBit) {
//					displayRaw[yg][b] |= rawBit;
//				} else {
//					displayRaw[yg][b] &= rawBitN;
//				}
//				// B
//				if ((colorB) & bBit) {
//					displayRaw[yb][b] |= rawBit;
//				} else {
//					displayRaw[yb][b] &= rawBitN;
//				}
//			}
		}
	}
}

int main(void)
{
	DDRB = DDR_MASK;
	DDRD = DDR_MASK;
	
	DDRC = !DDR_MASK;
	PORTC = DDR_MASK;

	displayLine = 0;
	timeCounter = 1;
	cycleCounter = 0;

	buttonsState = BUTTONS_ALL;

	// init timer
//	// prescale timer to 1/8th the clock rate
//	TCCR0 |= (1<<CS01);
	// prescale timer to 1/1st the clock rate
	TCCR0 |= (1<<CS00);
	// enable timer overflow interrupt
	TIMSK |= (1<<TOIE0);

	#ifdef SPEED_MEASURE
		TCCR1B |= (1<<CS10);
	#endif

	setFrameLength(50);

	sei();

	// show splash
	#ifdef SPLASH
		showScreen(&SPLASH);
//		uint8_t i;
//		for (i = 0; i < 64; i++) {
//			display[i % 8][i / 8] = i;
//		}
		refreshDisplay();
		while (buttonsState == BUTTONS_ALL) {}
	#endif

	// init game
	initGame();

	frameCounter = 0;
	while (1)
	{
		if (frameCounter != cycleCounter) {
			frameCounter = cycleCounter;

			// do buttons state update
			uint8_t currentButtons = (~buttonsState) & BUTTONS_ALL;
			buttonsState = BUTTONS_ALL;
			buttonsHit |= currentButtons & ~buttonsPressed;
			buttonsPressed = currentButtons;

			// do game loop
			gameLoop();

			#ifdef SPEED_MEASURE
			{
				uint8_t tInterruptTimerStart = interruptTimerStart;
				uint8_t tInterruptTimerEnd = interruptTimerEnd;
				display[0][0] = (tInterruptTimerStart & 0b10000000) != 0;
				display[1][0] = (tInterruptTimerStart & 0b01000000) != 0;
				display[2][0] = (tInterruptTimerStart & 0b00100000) != 0;
				display[3][0] = (tInterruptTimerStart & 0b00010000) != 0;
				display[4][0] = (tInterruptTimerStart & 0b00001000) != 0;
				display[5][0] = (tInterruptTimerStart & 0b00000100) != 0;
				display[6][0] = (tInterruptTimerStart & 0b00000010) != 0;
				display[7][0] = (tInterruptTimerStart & 0b00000001) != 0;
				display[0][7] = (tInterruptTimerEnd & 0b10000000) != 0;
				display[1][7] = (tInterruptTimerEnd & 0b01000000) != 0;
				display[2][7] = (tInterruptTimerEnd & 0b00100000) != 0;
				display[3][7] = (tInterruptTimerEnd & 0b00010000) != 0;
				display[4][7] = (tInterruptTimerEnd & 0b00001000) != 0;
				display[5][7] = (tInterruptTimerEnd & 0b00000100) != 0;
				display[6][7] = (tInterruptTimerEnd & 0b00000010) != 0;
				display[7][7] = (tInterruptTimerEnd & 0b00000001) != 0;
			}
			#endif

			// draw frame
			refreshDisplay();
		}
	}
}

uint8_t getRandom(uint8_t maxValue) {
	uint8_t random = randomSeed % maxValue;
	randomSeed = randomSeed / maxValue;
	randomSeed += ((randomSeed * 53) + timeCounter + cycleCounter) & 0xFFFF;
	return random;
}

uint8_t xxx;

// Timer interrupt
ISR(TIMER0_OVF_vect)
{
	#ifdef SPEED_MEASURE
		uint16_t tcnt1 = TCNT1;
		interruptTimerStart = TCNT1 - tcnt1;
	#endif

	// update counters
	if (!(--timeCounter)) {
		timeCounter = frameLength;
		cycleCounter ++;
	}

	// update display
	if ((++displayLine) >= 24) {
		displayLine = 0;
		if ((++displayBright) >= 15) {
			displayBright = 0;
		}
	}
	uint8_t bright = displayBright;
	if (bright >= 4) {
		bright = 2;
	} else if (bright >= 1) {
		bright = 1;
//	} else {
//		bright = 0;
	}
	uint8_t lineTransformed = displayRaw[displayLine][bright];

//	if ((++displayLine) >= 24) {
//		displayLine = 0;
//		if ((++displayBright) >= 24) {
//			displayBright = 0;
//		}
//	}
//	uint8_t lineTransformed;
//	if (displayBright >= 14) {
//		lineTransformed = displayRaw[displayLine][0] & displayRaw[displayLine][1] & displayRaw[displayLine][2];
//	} else if (displayBright >= 8) {
//		lineTransformed = displayRaw[displayLine][1] & displayRaw[displayLine][2];
//	} else if (displayBright >= 5) {
//		lineTransformed = displayRaw[displayLine][0] & displayRaw[displayLine][2];
//	} else if (displayBright >= 3) {
//		lineTransformed = displayRaw[displayLine][2];
//	} else if (displayBright >= 2) {
//		lineTransformed = displayRaw[displayLine][0] & displayRaw[displayLine][1];
//	} else if (displayBright >= 1) {
//		lineTransformed = displayRaw[displayLine][1];
//	} else {
//		lineTransformed = displayRaw[displayLine][0];
//	}

	PORTB = 0b00011111;//0xFF;
	PORTD = lineTransformed;
	PORTB = displayLine;

	// Sound
	if (!(xxx++)) {
		PORTB |= 0b00100000;
	} else {
		PORTB &= ~0b00100000;
	}

	// update keyboard
	uint8_t prevButtonsState = buttonsState;
	buttonsState &= PINC;

	// update random
	if (prevButtonsState != buttonsState) {
		randomSeed = (randomSeed * 31) + timeCounter;
	}

	#ifdef SPEED_MEASURE
		interruptTimerEnd = TCNT1 - tcnt1;
		interruptTimerStart = TCNT1 - tcnt1 >> 8;
	#endif
}
