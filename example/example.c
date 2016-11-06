#define PICTOR_FASTMODE
#include <avr/io.h>

#include "../pictor.h"
#include "../fonts/OryxB.h"
#include "../fonts/Mash.h"
#include "sprite_ghost.h"
#include "sprite_check.h"

/*
In this example a selection of draw functions are demonstrated and the on board LED is lit for the duration of the initial screen drawing process
*/

static const unsigned char text1[] PROGMEM = "Pictor - ILI9341 driver V0.3.1b";
//this text is stored entirely in program memory, useful for long strings that won't change
//it is also useful if the same string is used many times in the code, to declare the string once at the beginning rather than every time it is used
//this means that it is only in the code once, rather than once for every time it is used

int main() {
	DDRB |= _BV(PB7);
	pictorInit(0);
	PORTB |= _BV(PB7);
	pictorSetRotation(1);
	//	Welcome
	pictorDrawSP(text1, ORIGIN, RED, BLACK, Mash,1);
	//	Draw chars
	uint8_t I, J, K, L;
	pictorDrawS("  Mash  ", (point){0, 8}, BLACK, ORANGE, Mash,1);
	pictorDrawBox( (point){0, 24}, (point){127, 151}, DARK WHITE);
	for (I = 0; I < 8; I++) {
		for (J = 0; J < 16; J++) {
			pictorDrawC( I * 16 + J, (point){I * 8, 24 + J * 8}, ORANGE, BLACK, Mash,1);
		}
	}
	pictorDrawS("  OryxB ", (point){64, 8}, BLACK, PALE BLUE, OryxB,1);
	for (I = 0; I < 8; I++) {
		for (J = 0; J < 16; J++) {
			pictorDrawC( I * 16 + J, (point){64 + I * 8, 24 + J * 8}, PALE BLUE, BLACK, OryxB,1);
		}
	}
	//	Draw sprites
	pictorDrawSprite(&ghost, (point){0, 152}, 1);
	pictorDrawSprite(&ghost, (point){0, 168}, 1);
	//draw parts of sprite
	pictorDrawSpritePartial(&ghost, (point){0, 184}, 1, (point){0,0}, (point){7,7});
	pictorDrawSpritePartial(&ghost, (point){16, 184}, 1, (point){8,0}, (point){15,7});
	pictorDrawSpritePartial(&ghost, (point){0, 200}, 1, (point){0,8}, (point){7,15});
	pictorDrawSpritePartial(&ghost, (point){16, 200}, 1, (point){8,8}, (point){15,15});
	//	Draw scaled sprites
	J = 16;
	for (I = 2; I <= 32; I <<= 1) {
		L = 152;
		for (K = 32 / I; K; K--) {
			pictorDrawSprite(&check, (point){J, L}, I >> 1);
			L += I;
		}
		J += I;
	}
	pictorDrawSprite(&ghost, (point){80, 152}, 2);
	//draw parts of scaled sprites
	for(I=0;I<4;I++){
		for(J=0;J<4;J++){
			pictorDrawSpritePartial(&ghost, (point){128+(I*18),88+(J*18)}, 4, (point){I*4,J*4}, (point){(I*4)+3,(J*4)+3});
		}
	}
	//draw scaled text
	pictorDrawS("Hello world", (point){128,8}, WHITE, BLACK, Mash,2);
	pictorDrawS("Game\nOver", (point){128,24}, RED, BLACK, Mash,4);
	//	Complete
	pictorDrawSP(text1, ORIGIN, GREEN, BLACK, Mash,1);
	pictorDrawS("Last Modified:", (point){0, 231}, MAGENTA, BLACK, Mash,1);
	pictorDrawS(__TIMESTAMP__, (point){120, 231}, MAGENTA, BLACK, Mash,1);
	PORTB &= ~_BV(PB7);
	I=0;
	while(1){
		pictorDrawD(I, (point){128,178}, CYAN, BLACK, Mash, 4, 3);
		I++;
	}
	return 1;
}