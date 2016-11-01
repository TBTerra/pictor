/*	Pictor.h	VERSION 0.2.6
 *	Screen driver for the IlMatto and 2.2" screen combo
 *	Use port A for control & port C for data.
 *
 *	Note that this code is subject to change, check the changelog when updating.
 *	Project by Henry Wilson, Terra Barber, Emma Curati-Alasonatti and Chris Turner.
 *
 *	This work is licenced under the Creative Commons Attribution-ShareAlike 3.0 licence
 *	This licence can be found at http://creativecommons.org/licenses/
 */

/* 	CHANGELOG
 * 	== 0.2
 * 		+ Initial release
 *	== 0.2.2
 *		+ Added speed optimised functions
 *		+ Added sprite drawing functions
 *		~ Refined some drawing functions
 *	== 0.2.3
 *		+ Added LCD_FASTMODE, use #define LCD_FASTMODE to switch to alternate versions of functions that are 20-40% faster, but use use much more program memory
 *		+ Added sprite data type
 *		+ Allowed reassigning of the IO ports.
 *		+ Added lcdDrawAll() to fill the screen with a Colour.
 *		~ Changed sprite drawing to use sprite type
 *		~ Made code conform to style guide
 *		! Spotted bug in lcdDrawLine that prevents it from drawing lines between 
 *			~ Changed lcdDrawLine to be enabled using #define LCD_DRAWLINE_ENABLE until fixed
 *		~ Changed how lcdDrawBox works to allow boxes larger than 65535 pixels
 *	== 0.2.4
 *		+ drasticly increased draw sprite and draw sprite scaled performance (50-70% increase)
 *		~ changed the low level functions to inline for minor speed increase at neglagable extra memory
 *		~ changed some loops to use pre-decrement rather than post-decrement because somehow thats faster
 *		+ added set_rotation for rotated screen
 *		+ added scale functionality to draw char, string and value functions (will break current implimentations)
 *	== 0.2.4.1
 *		~changed the drawAll function to support rotated scrrens
 *	== 0.2.5
 *		+added lcdDrawSpritePartial(for when only part of the sprite needed to be drwn (ie only part on screen)
 *		+added lcdDrawSpriteScaledPartial for above purpose
 *		+added \n functionality to lcdDrawS (alows multiline strings)
 *		~changed fonts to be stored and used from program memory (saves about 900bytes of memory per font)
 *		~hevaly reworked the way fonts work (no visasble difference to end user)
 *		+-?added memDump(). use at your own peril
 *	== 0.2.5.1
 *		+changed lcdDrawX to take 8bits at a time and put out 2characters always, behaviour now far more predictable
 *	== 0.2.6pre1
 *		+added lcdDrawSP to draw strings from program memory
 *		+added FONTSTART and FONTEND for use in extended fonts (must be defined before including pictor.h)
 *		~changed string/char drawing to used unsigned chars (again to support extended fonts)[should not affect normal fonts]
 *	== 0.2.6
 *		-removed functions that dont work poroperly or lack purpose in prep for moving to a modular system in 1.0
 *		-removed lcdDrawLine
 *		-remoded lcdDrawShape
 *		-remoded shape typedef
 *		-removed color typedef (its just uint16_t)
 *		-removed point arithmatic (hardly used)
 */

#ifndef PICTOR
#define PICTOR

#ifndef FONTSTART
#define FONTSTART ' '
#endif
#ifndef FONTEND
#define FONTEND '~'
#endif

//	Generic libraries
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//	AVR libraries
#include <avr/io.h>
//#include <avr/interrupt.h>
#include <util/delay.h>//needed for scrreen initaliseation
#include <avr/pgmspace.h> //TODO: Work out where this really should go...

//	POINT STRUCTURE	TODO: Look into signed mode.
typedef struct {
	uint16_t X;
	uint16_t Y;
} point;
//	SPRITE STRUCTURE
typedef struct/*PROGMEM*/ {
	const point Size; // remember that the size is {X > 0, Y > 0}!
	const uint16_t RGB[];
} sprite;
//	FONT STRUCTURE
#define font uint8_t
// Colour type, unused atm
typedef uint16_t colour;

static uint8_t lcdRot = 0;

//	DATA PORTS -- don't use port B if you intend to use USB!
#define CTRLPORT	PORTA
#define CTRLDDR		DDRA
#define DATAPORT	PORTC
#define DATADDR		DDRC
#define DATAPINS	PINC

//	CONRTOL BITS
#define BACKLIGHT	0b00000010
#define RESET		0b11111011
#define WRITE		0b11110111
#define COMND		0b11101111
#define READ		0b11011111

//	USEFUL
#define ON			1
#define OFF			0
#define INVERT		-1

//	POINTS
#define ORIGIN		(point){0,	0}
#define UNIT_POINT	(point){1,	1}
#define LCD_MAX		(point){239,319}
#define LCD_SIZE	(point){240,320}

//	COLOURS		  RRRRRGGGGGGBBBBB
#define BLACK	0b0000000000000000
#define WHITE	0b1111111111111111
#define RED		0b1111100000000000
#define ORANGE	0b1111101011100000
#define YELLOW	0b1111111111100000
#define GREEN	0b0000011111100000
#define CYAN	0b0000011111111111
#define BLUE	0b0000000000011111
#define MAGENTA	0b1111100000011111
//	COLOUR MODIFIERS
#define PALE	0b1000010000010000 |
#define DEEP	0b0111001111001110 &
#define DARK	0b0011000111000110 &


//	Create a colour encoded in 656-RGB from individual RGB bytes.
uint16_t encode656Colour(const uint8_t Red, const uint8_t Green, const uint8_t Blue);

//===============
// BASE LCD CMDS
//===============
//	Sends a command byte to the screen.
inline void lcdCmdWrite(const uint8_t Cmd);
//	Sends a data byte to the screen.
inline void lcdByteWrite(const uint8_t Data);
//	Sends two data bytes to the srceen.
inline void lcdWordWrite(uint16_t Value);
//	Returns a data byte read from the screen.
inline uint8_t lcdByteRead();
//	Returns a word from two data bytes read from the screen.
inline uint16_t lcdWordRead();
//	Sets the state of the back-light, 1 = ON, 0 = OFF, -1 = TOGGLE
void lcdBacklightState(const int8_t State);
//================
// LCD ARRAY CMDS
//================

//	Writes a command and a byte array containing its parameters to the screen.
void lcdWrite(const uint8_t Cmd, const uint8_t * Parameter, const uint8_t Count);
//	Writes a command and reads out an array of byte data into a buffer from the screen.
void lcdRead(const uint8_t Cmd, uint8_t * Buffer, const uint8_t Count);
//====================
// LCD COMPOSITE CMDS
//====================

//	Sets a drawing canvas on the screen.
void lcdCanvasSet(const point A, const point B);
void set_rotation(const uint8_t rotation);
//=========================
// LCD DRAW CHAR FUNCTIONS
//=========================

//	Draws Char onto the screen at Pos with two colours of Font.
void lcdDrawC(const unsigned char Char, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale);
//	Draws String onto the screen at Pos with two colours of Font.
uint8_t lcdDrawS(const unsigned char * Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale);
uint8_t lcdDrawSP(const unsigned char * Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale);
//	Draws a Number in decimal onto the screen at Pos with two colours of Font.
uint8_t lcdDrawD(const int Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale);
void lcdDrawDl(const int Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale, uint8_t len);
//	Draws a Value in hex onto the screen at Pos with two colours of Font.
uint8_t lcdDrawX(const uint8_t Value, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale);
//=================
// LCD DRAW SHAPES
//=================

//	Draws a single pixel with Colour at Pos
void lcdDrawPixel(const point Pos, const uint16_t Colour);
//	Draws a solid box with Colour between points A and B.
void lcdDrawBox(point A, point B, const uint16_t Colour);
//	Fills the screen with Colour.
void lcdDrawAll(const uint16_t Colour);
//	Draws a Sprite top left aligned at Pos.
void lcdDrawSprite(const sprite * Sprite, const point Pos);
//	Draws a Sprite top left aligned at Pos scaled by Scale.
void lcdDrawSpriteScaled(const sprite * Sprite, const point Pos, const uint8_t ScaleX, const uint8_t ScaleY);
//draw part of a sprite selected by X1 and X2
void lcdDrawSpritePartial(const sprite * Sprite, const point Pos, point X1, point X2);
void lcdDrawSpriteScaledPartial(const sprite * Sprite, const point Pos, const uint8_t ScaleX, const uint8_t ScaleY, point X1, point X2);
//	Draws a circle using Bresenham's algorithm with colour between points A and B.
void lcdDrawCircle(const point Centre, const uint8_t Radius, const uint16_t Colour);
//====================
// LCD INTITALISATION
//====================

//	Initialise the screen. Currently Mode is unused, but will later do stuff (Use 0 for now)
void lcdInit(const uint8_t Mode);

// Dump the entire of ram and program memory to the screen
void memDump();

#include "pictor.c"

//#include <avr/pgmspace.h>

#endif
