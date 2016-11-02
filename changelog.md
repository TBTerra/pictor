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
* \+ Added lcdDrawAll() to fill the screen with a Colour.
* ~ Changed sprite drawing to use sprite type
* ~ Made code conform to style guide
* \! Spotted bug in lcdDrawLine that prevents it from drawing lines between 
* * ~ Changed lcdDrawLine to be enabled using #define LCD_DRAWLINE_ENABLE until fixed
* ~ Changed how lcdDrawBox works to allow boxes larger than 65535 pixels

### 0.2.4
* \+ drasticly increased draw sprite and draw sprite scaled performance (50-70% increase)
* ~ changed the low level functions to inline for minor speed increase at neglagable extra memory
* ~ changed some loops to use pre-decrement rather than post-decrement because somehow thats faster
* \+ added set_rotation for rotated screen
* \+ added scale functionality to draw char, string and value functions (will break current implimentations)

### 0.2.4.1
* ~changed the drawAll function to support rotated scrrens

### 0.2.5
* \+ added lcdDrawSpritePartial(for when only part of the sprite needed to be drwn (ie only part on screen)
* \+ added lcdDrawSpriteScaledPartial for above purpose
* \+ added \n functionality to lcdDrawS (alows multiline strings)
* ~ changed fonts to be stored and used from program memory (saves about 900bytes of memory per font)
* ~ hevaly reworked the way fonts work (no visasble difference to end user)
* \+-? added memDump(). use at your own peril

### 0.2.5.1
* \+changed lcdDrawX to take 8bits at a time and put out 2characters always, behaviour now far more predictable

### 0.2.6pre1
* \+ added lcdDrawSP to draw strings from program memory
* \+ added FONTSTART and FONTEND for use in extended fonts (must be defined before including pictor.h)
* ~ changed string/char drawing to used unsigned chars (again to support extended fonts)[should not affect normal fonts]

### 0.2.6
* \- removed lcdDrawLine (unusual behaviour in edge cases, will re-add when fixed)
* \- remoded lcdDrawShape (never finished implementation due to DrawLine issues)
* \- remoded shape typedef (no longer needed)
* \- removed color typedef (its just uint16_t)
* \- removed point arithmatic (hardly used)