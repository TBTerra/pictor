# Version Changelog
### 0.2
* Initial release

### 0.2.2
* \+ Added speed optimised functions
* \+ Added sprite drawing functions
* ~ Refined some drawing functions

### 0.2.3
* \+ Added LCD_FASTMODE, use #define LCD_FASTMODE to switch to alternate versions of functions that are 20-40% faster, but use use much more program memory
* \+ Added sprite data type
* \+ Allowed reassigning of the IO ports.
* \+ Added lcdDrawAll() to fill the screen with a colour
* ~ Changed sprite drawing to use sprite type
* ~ Made code conform to style guide
* \! Spotted bug in lcdDrawLine that prevents it from drawing lines between 
* * ~ Changed lcdDrawLine to be enabled using #define LCD_DRAWLINE_ENABLE until fixed
* ~ Changed how lcdDrawBox works to allow boxes larger than 65535 pixels

### 0.2.4
* \+ Drastically increased draw sprite and draw sprite scaled performance (50-70% increase)
* ~ Changed the low level functions to inline for minor speed increase at negligible extra memory
* ~ Changed some loops to use pre-decrement rather than post-decrement because somehow that's faster
* \+ Added set_rotation for rotated screen
* \+ Added scale functionality to draw char, string and value functions (will break current implementations)

### 0.2.4.1
* ~ Changed the drawAll function to support rotated screens

### 0.2.5
* \+ Added lcdDrawSpritePartial(for when only part of the sprite needed to be drawn (i.e. only part on screen)
* \+ Added lcdDrawSpriteScaledPartial for above purpose
* \+ Added \n functionality to lcdDrawS (allows multi-line strings)
* ~ Changed fonts to be stored and used from program memory (saves about 900bytes of memory per font)
* ~ Heavily reworked the way fonts work (no visible difference to end user)
* \+-? Added memDump(). use at your own peril

### 0.2.5.1
* \+ Changed lcdDrawX to take 8-bits at a time and put out 2 characters always, behaviour now far more predictable

### 0.2.6pre1
* \+ Added lcdDrawSP to draw strings from program memory
* \+ Added FONTSTART and FONTEND for use in extended fonts (must be defined before including pictor.h)
* ~ Changed string/char drawing to use unsigned chars (again to support extended fonts)[should not affect normal fonts]

### 0.2.6
* \- Removed lcdDrawLine (unusual behaviour in edge cases, will re-add when fixed)
* \- Removed lcdDrawShape (never finished implementation due to DrawLine issues)
* \- Removed shape typedef (no longer needed)
* \- Removed colour typedef (its just uint16_t)
* \- Removed point arithmetic (hardly used)

### 0.3.0
* \- Removed changelog from pictor.h as it is now here
* ~ Changed names of functions and defines to make them more consistent
* ~ Changed FONTSTART and FONTEND to PICTOR_FONTSTART and PICTOR_FONTEND
* ~ Changed LCD_MAX and LCD_SIZE to PICTOR_MAX and PICTOR_SIZE
* ~ Changed LCD_FASTMODE to PICTOR_FASTMODE
* ~ Changed lcdCmdWrite to pictorCmdWrite
* ~ Changed lcdByteWrite to pictorByteWrite
* ~ Changed lcdWordWrite to pictorWordWrite
* ~ Changed lcdByteRead to pictorByteRead
* ~ Changed lcdWordRead to pictorWordRead
* ~ Changed lcdBacklightState to pictorBacklightState
* ~ Changed lcdWrite to pictorWrite
* ~ Changed lcdRead to pictorRead
* ~ Changed lcdCanvasSet to pictorCanvasSet
* ~ Changed set_rotation to pictorSetRotation
* ~ Changed lcdDrawC to pictorDrawC
* ~ Changed lcdDrawS to pictorDrawS
* ~ Changed lcdDrawSP to pictorDrawSP
* ~ Changed lcdDrawD to a special case of the new pictorDrawD (len=0)
* ~ Changed lcdDrawDl to pictorDrawD
* ~ Changed lcdDrawX to pictorDrawX
* ~ Changed lcdDrawPixel to pictorDrawPixel
* ~ Changed lcdDrawBox to pictorDrawBox
* ~ Changed lcdDrawAll to pictorDrawAll
* ~ Changed lcdDrawSprite to a special case of the new pictorDrawSprite (scale=1)
* ~ Changed lcdDrawSpriteScaled to pictorDrawSprite
* ~ Changed lcdDrawSpritePartial to a special case of the new pictorDrawSpritePartial (scale=1)
* ~ Changed lcdDrawSpriteScaledPartial to pictorDrawSpritePartial
* ~ Changed lcdDrawCircle to pictorDrawCircle
* ~ Changed lcdInit to pictorInit
* ~ Changed memDump to pictorMemDump
* ~ Changed lcdRot to pictorRot
* \- Removed the ability to scale separately in x and y (simplifies code as was rarely used)
* \+ Fixed lcdDrawD issues with negative numbers

### 0.3.1
* \+ Added back in pictorWrite (accidentally removed in 0.3.0)
* ~ Updated pictorDrawCircle description to better represent functionality
* \+ Added documentation of functions in doc.txt
* ~ Minor text fixes

### 0.3.1b
* ~ Changed example.c to include pictor.h and the fonts in place
* ~ Recompiled example.c with version 0.3.1b

### 0.3.2
* \+ Added number drawing function to example.c
* ~ Changed LED function in example.c to demonstrate the speed of the draw functions
* ~ Renamed pacman to ghost in sprite_ghost.h
* ~ Fixed padding in pictorDrawD()
* ~ First pass for standardising the formatting throughout pictor

### 0.3.3
* \+ Added pictorDrawLine function
* \+ Added 12 new types of sprite.
  * 0 - full colour in memory 
  * 1 - full colour in PROGMEM 
  * 2 - 1bit in memory 
  * 3 - 1bit in PROGMEM 
  * 4 - Palette and indexed sprite in memory 
  * 5 - Indexed sprite in PROGMEM, palette in memory 
  * 6 - Indexed sprite in memory, palette in PROGMEM 
  * 7 - Palette and indexed sprite in PROGMEM 
  * 8 - Palette and packed pixels in memory
  * 9 - Packed pixels in PROGMEM, palette in memory
  * 10 - Packed pixels in memory, palette in PROGMEM
  * 11 - Palette and packed pixels in PROGMEM
* ~ Changed sprite drawing to the pictorDrawSpriteType() function, which adds functionality for 8 types of sprite. 
* ~ updated example to use new sprite functions.
* \+ Added a spriteCreator to generate sprites from PNG images.
* \+ Added VSYNC functionality