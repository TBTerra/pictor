#define PICTOR_FASTMODE
#include <avr/io.h>
#include <util/delay.h>

#include "../../pictor.h"
#include "test.h"

#define SPRITE_SCALE 3
#define SPRITE_X 24
#define SPRITE_Y 24

/*
In this example a selection of sprite functions are demonstrated and the on board LED is lit for the duration of the each sprite drawing process
*/

int main() {
	DDRB |= _BV(PB7);
	pictorInit(0);
	pictorSetRotation(1);
	pictorDrawAll(CYAN);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite0, (point) {0,0}, 0, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite1, (point) {((320 - (SPRITE_SCALE * SPRITE_X))/3), 0}, 1, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite2, (point) {(((320 - (SPRITE_SCALE * SPRITE_X))/3)*2), 0}, 2, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite3, (point) {(((320 - (SPRITE_SCALE * SPRITE_X)))), 0}, 3, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite4, (point) {0, 240 - (SPRITE_SCALE * SPRITE_Y)}, 4, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite5, (point) {(((320 - (SPRITE_SCALE * SPRITE_X))/3)), 240 - (SPRITE_SCALE * SPRITE_Y)}, 5, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite6, (point) {(((320 - (SPRITE_SCALE * SPRITE_X))/3)*2), 240 - (SPRITE_SCALE * SPRITE_Y)}, 6, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite7, (point) {(((320 - (SPRITE_SCALE * SPRITE_X)))), 240 - (SPRITE_SCALE * SPRITE_Y)}, 7, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(1000);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite8, (point) {0, 10}, 8, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite9, (point) {((320 - (SPRITE_SCALE * SPRITE_X))/3), 10}, 9, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &spriteA, (point) {(((320 - (SPRITE_SCALE * SPRITE_X))/3)*2), 10}, 10, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &spriteB, (point) {(((320 - (SPRITE_SCALE * SPRITE_X)))), 10}, 11, SPRITE_SCALE);
	PORTB &= ~_BV(PB7);
	_delay_ms(20);
	
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite0, (point) {0, 120 - SPRITE_Y/2}, 0, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite1, (point) {(SPRITE_X + 2), 120 - SPRITE_Y/2}, 1, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite2, (point) {(SPRITE_X + 2) * 2, 120 - SPRITE_Y/2}, 2, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite3, (point) {(SPRITE_X + 2) * 3, 120 - SPRITE_Y/2}, 3, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite4, (point) {(SPRITE_X + 2) * 4, 120 - SPRITE_Y/2}, 4, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite5, (point) {(SPRITE_X + 2) * 5, 120 - SPRITE_Y/2}, 5, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite6, (point) {(SPRITE_X + 2) * 6, 120 - SPRITE_Y/2}, 6, 1);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite7, (point) {(SPRITE_X + 2) * 7, 120 - SPRITE_Y/2}, 7, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite8, (point) {(SPRITE_X + 2) * 8, 120 - SPRITE_Y/2}, 8, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &sprite9, (point) {(SPRITE_X + 2) * 9, 120 - SPRITE_Y/2}, 9, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &spriteA, (point) {(SPRITE_X + 2) * 10, 120 - SPRITE_Y/2}, 10, 1);
	PORTB &= ~_BV(PB7);
	_delay_ms(10);
	PORTB |= _BV(PB7);
	pictorDrawSpriteType((void*) &spriteB, (point) {(SPRITE_X + 2) * 11, 120 - SPRITE_Y/2}, 11, 1);
	PORTB &= ~_BV(PB7);
	while(1);
	return 1;
}