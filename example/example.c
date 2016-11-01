#define LCD_FASTMODE
#include <avr/io.h>

#include "pictor.h"
#include "OryxB.h"
#include "Mash.h"
#include "sprite_ghost.h"
#include "sprite_check.h"

#include <avr/pgmspace.h>

void main() {
	DDRB |= _BV(PB7);
	lcdInit(0);
	set_rotation(1);
	//	Welcome
	lcdDrawS("Pictor - ILI9341 driver V0.2.5", ORIGIN, RED, BLACK, Mash,1);
	//	Draw chars
	uint8_t I, J, K, L;
	lcdDrawS("  Mash  ", (point){0, 8}, BLACK, ORANGE, Mash,1);
	lcdDrawBox( (point){0, 24}, (point){127, 151}, DARK WHITE);
	for (I = 0; I < 8; I++) {
		for (J = 0; J < 16; J++) {
			lcdDrawC( I * 16 + J, pointXY(I * 8, 24 + J * 8), ORANGE, BLACK, Mash,1);
		}
	}
	lcdDrawS("  OryxB ", (point){64, 8}, BLACK, PALE BLUE, OryxB,1);
	for (I = 0; I < 8; I++) {
		for (J = 0; J < 16; J++) {
			lcdDrawC( I * 16 + J, pointXY(64 + I * 8, 24 + J * 8), PALE BLUE, BLACK, OryxB,1);
		}
	}
	//	Draw sprites
	lcdDrawSprite(&pacman, (point){0, 152});
	lcdDrawSprite(&pacman, (point){0, 168});
	//draw parts of sprite
	lcdDrawSpritePartial(&pacman, (point){0, 184}, (point){0,0}, (point){7,7});
	lcdDrawSpritePartial(&pacman, (point){16, 184}, (point){8,0}, (point){15,7});
	lcdDrawSpritePartial(&pacman, (point){0, 200}, (point){0,8}, (point){7,15});
	lcdDrawSpritePartial(&pacman, (point){16, 200}, (point){8,8}, (point){15,15});
	//	Draw scaled sprites
	J = 16;
	for (I = 2; I <= 32; I <<= 1) {
		L = 152;
		for (K = 32 / I; K; K--) {
			lcdDrawSpriteScaled(&check, pointXY(J, L), I >> 1 , I >> 1 );
			L += I;
		}
		J += I;
	}
	lcdDrawSpriteScaled(&pacman, (point){80, 152}, 3,2 );
	//draw parts of scaled sprites
	for(I=0;I<4;I++){
		for(J=0;J<4;J++){
			lcdDrawSpriteScaledPartial(&pacman, (point){128+(I*18),88+(J*18)}, 4, 4, (point){I*4,J*4}, (point){(I*4)+3,(J*4)+3});
		}
	}
	//draw scaled text
	lcdDrawS("Hello world", (point){128,8}, WHITE, BLACK, Mash,2);
	lcdDrawS("Game", (point){128,24}, RED, BLACK, Mash,4);
	lcdDrawS("Over", (point){128,56}, RED, BLACK, Mash,4);
	//	Complete
	lcdDrawS("Pictor - ILI9341 driver V0.2.5", ORIGIN, GREEN, BLACK, Mash,1);
	PORTB |= _BV(PB7);
}