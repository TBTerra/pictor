#include "pictor.h"

uint16_t encode656Colour(const uint8_t Red, const uint8_t Green, const uint8_t Blue) {
	uint16_t Colour = Red & 0xF8;	//	00000000RRRRR000
	Colour <<= 5;					//	000RRRRR00000000
	Colour |= Green & 0xFC;			//	000RRRRRGGGGGG00
	Colour <<= 3;					//	RRRRRGGGGGG00000
	Colour |= Blue >> 3;			//	RRRRRGGGGGGBBBBB
	return Colour;
}
//===============
// BASE LCD CMDS (now with weird inline stile define bassed functions)
//===============
inline void lcdCmdWrite(const uint8_t Cmd) {
	CTRLPORT &=  COMND;
	DATAPORT  =  Cmd;
	CTRLPORT &=  WRITE;
	CTRLPORT |= ~WRITE;
	CTRLPORT |= ~COMND;
}
inline void lcdByteWrite(const uint8_t Data) {
	DATAPORT  =  Data;
	CTRLPORT &=  WRITE;
	CTRLPORT |= ~WRITE;
}
inline void lcdWordWrite(uint16_t Value) {
	lcdByteWrite( (uint8_t)(Value >> 8) );
	lcdByteWrite( (uint8_t)(Value & 0xFF) );
}
inline uint8_t lcdByteRead() {
	uint8_t Data;
	CTRLPORT &=  READ;
	Data   =  DATAPINS;
	CTRLPORT |= ~READ;
	return Data;
}
inline uint16_t lcdWordRead() {
	return ( ( (uint16_t)lcdByteRead() ) << 8 ) + lcdByteRead();
}
void lcdBacklightState(const int8_t State) {
	if (State > 0) {
		CTRLPORT |=  BACKLIGHT;
	} else if (!State) {
		CTRLPORT &= ~BACKLIGHT;
	} else {
		CTRLPORT ^=  BACKLIGHT;
	}
}
//===============
// LCD BULK CMDS
//===============

void lcdArrayWrite(const uint8_t * Data, const uint16_t Count){
#ifndef LCD_FASTMODE
	uint16_t I = 0;
	while (I < Count) {
		lcdByteWrite( Data[I++] );
	}
#else
	uint8_t HighCount = (uint8_t)(Count >> 8);
	uint8_t LowCount = (uint8_t)Count & 0xFF; //std name
	uint8_t I;
	uint16_t Index = 0;
	while (HighCount--) { // moved from for loops to while loops
		I = 17;
		while (--I) {//moved from post decrement to pre decrement (because somehow its faster)
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
		}
	}
	while (LowCount--) {
		DATAPORT = Data[Index++];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
	}
#endif
}


//	void lcdByteArrayWrite(const uint8_t Cmd, const uint8_t * Data, const uint16_t Count) {
void lcdByteArrayWrite(const uint8_t Cmd, const uint8_t * Data, const uint16_t Count) {
	lcdCmdWrite(Cmd);
	lcdArrayWrite(Data,Count);
}
#ifndef LCD_FASTMODE	//	void lcdWordArrayWrite(const uint8_t Cmd, const uint16_t * Data, const uint16_t Count) {
void lcdWordArrayWrite(const uint8_t Cmd, const uint16_t * Data, const uint16_t Count) {
	uint8_t I = 0;
	lcdCmdWrite(Cmd);
	while (I < Count) {
		lcdWordWrite( Data[I++] );
	}
}
#else
void lcdWordArrayWrite(const uint8_t Cmd, const uint16_t * Data, const uint16_t Count) { // changed to accept uint16_t as argument, it can cast the pointer itself and added Cmd.
	const uint8_t * Byte = (uint8_t *)Data;
	lcdCmdWrite(Cmd);
	uint8_t I; // std name
	uint8_t HighCount = (uint8_t)(Count >> 8);
	uint8_t LowCount = (uint8_t)Count & 0xFF; //std name
	uint16_t Index = 0;
	while (HighCount--) { // moved from for loops to while loops
		I = 17;
		while (--I) {
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
			DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;
			DATAPORT = Byte[Index];		CTRLPORT &= WRITE;	CTRLPORT |= ~WRITE;	Index+= 2;
		}
	}
	while (LowCount--) {
		DATAPORT = Byte[Index+1];	CTRLPORT &= WRITE;		CTRLPORT |= ~WRITE;
		DATAPORT = Byte[Index];	CTRLPORT &= WRITE;		CTRLPORT |= ~WRITE;
		Index += 2;
	}
}
#endif
#ifndef LCD_FASTMODE	//	void lcdRepeatedByteWrite(const uint8_t Cmd, const uint8_t Data, uint16_t Count) {
void lcdRepeatedByteWrite(const uint8_t Cmd, const uint8_t Data, uint16_t Count) { // NEW
	lcdCmdWrite(Cmd);
	Count++;
	while (--Count) { // moved for loops to while loops(using predecrements rather than post decrements
		lcdByteWrite(Data);
	}
}
#else
void lcdRepeatedByteWrite(const uint8_t Cmd, const uint8_t Data, uint16_t Count) { // NEW
	uint8_t HighCount = (uint8_t)(Count >> 8);
	uint8_t LowCount = (uint8_t)Count & 0xFF; //std name
	uint8_t I;
	lcdCmdWrite(Cmd);
	DATAPORT  =  Data;
	while (HighCount--) {
		I = 17;
		while (--I) {
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
		}
	}
	while (LowCount--) {
		CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
	}
}
#endif
#ifndef LCD_FASTMODE	//	void lcdRepeatedWordWrite(const uint8_t Cmd, const uint16_t Data, uint16_t Count) {	
void lcdRepeatedWordWrite(const uint8_t Cmd, const uint16_t Data, uint16_t Count) { // NEW
	uint8_t HighByte = (uint8_t)(Data >> 8);
	uint8_t LowByte  = (uint8_t)(Data & 0xFF);
	lcdCmdWrite(Cmd);
	while (Count--) {
		lcdByteWrite(HighByte);
		lcdByteWrite(LowByte);
	}
}
#else
void lcdRepeatedWordWrite(const uint8_t Cmd, const uint16_t Data, uint16_t Count) {	//std name, added cmd, made a fastmode cmd
	uint8_t I; // std name
	uint8_t HighCount = (uint8_t)(Count >> 8),			LowCount = (uint8_t)Count & 0xFF;//std name
	uint8_t HighByte = (uint8_t)(Data >> 8),	LowByte = (uint8_t)(Data) & 0xFF;//added for opti, optimised brackets & std name
	lcdCmdWrite(Cmd);
	while (HighCount--) { // moved for loops to while loops
		I = 17;
		while (--I) { // 16x16 is using a fair bit of memory(predecrement rather than postdecrement)
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
			DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
		}
	}
	while (LowCount--) {
		DATAPORT  =  HighByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE; DATAPORT  =  LowByte; CTRLPORT &=  WRITE; CTRLPORT |= ~WRITE;
	}
}
#endif
void lcdRead(const uint8_t Cmd, uint8_t * Buffer, const uint8_t Count) {
	uint8_t I = 0;
	lcdCmdWrite(Cmd);
	DATAPORT  =  0x00;
	DATADDR   =  0x00;
	CTRLPORT &=  READ;
	CTRLPORT |= ~READ;
	while (I < Count) {
		Buffer[I++] = lcdByteRead();
	}
	DATADDR   =  0xFF;
}
//====================
// LCD COMPOSITE CMDS
//====================
void lcdCanvasSet(const point A, const point B) {
	lcdCmdWrite(0x2A); // Column Address Set
	lcdWordWrite(A.X);
	lcdWordWrite(B.X);
	lcdCmdWrite(0x2B); // Page Address Set
	lcdWordWrite(A.Y);
	lcdWordWrite(B.Y);
}
void set_rotation(const uint8_t rotation){
	lcdCmdWrite(0x36);
	switch(rotation){
		case 0:
			lcdByteWrite(0x48);
			break;
		case 1:
			lcdByteWrite(0x28);
			break;
		case 2:
			lcdByteWrite(0x88);
			break;
		case 3:
			lcdByteWrite(0xE8);
			break;
	}
	lcdRot = rotation;
}
//=========================
// LCD DRAW CHAR FUNCTIONS
//=========================
void lcdDrawC(const unsigned char Char, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const uint8_t * Font, uint8_t scale){
	if (Char < FONTSTART || Char > FONTEND) return;
	const uint8_t * fdata;
	uint8_t bits, mask, I, J;
	uint8_t k, l;
	uint8_t HighFCol = (uint8_t)(ForegroundColour >> 8), LowFCol = (uint8_t)(ForegroundColour & 0xFF);//added for opti, standardised name
	uint8_t HighBCol = (uint8_t)(BackgroundColour >> 8), LowBCol = (uint8_t)(BackgroundColour & 0xFF);//added for opti, standardised name
	point Pos2 = {Pos.X + (8*scale) - 1, Pos.Y + (8*scale) - 1};
	lcdCanvasSet(Pos, Pos2);
	fdata = (Char - FONTSTART)*8 + Font;
	lcdCmdWrite(0x2C); // Memory Write
	for (I = 0; I < 8; I++) {//draw hoisontal line
		bits = pgm_read_byte(fdata++);
		for(k = 0;k<scale;k++){//draw repeated horisontal line for scale factor
			for (J = 0, mask=0x80; J < 8; J++, mask>>=1) {//draw each pixel on line
				for(l = 0;l<scale;l++){//draw repeats for scale
					if(bits & mask){
						lcdByteWrite(HighFCol);//added for opti
						lcdByteWrite(LowFCol);//added for opti
					}else{
						lcdByteWrite(HighBCol);//added for opti
						lcdByteWrite(LowBCol);//added for opti
					}
				}
			}
		}
	}
}
uint8_t lcdDrawS(const unsigned char * Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale) {
	uint8_t i = 0;
	point X1 = Pos;
	while (Buffer[i]) {
		if(Buffer[i] == '\n'){
			X1.Y += 8*scale;
			X1.X = Pos.X;
			i++;
		}else{
			lcdDrawC( Buffer[i++], X1, ForegroundColour, BackgroundColour, Font, scale);
			X1.X += 8*scale;
		}
	}
	return i;
}


uint8_t lcdDrawSP(const unsigned char * Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale) {
	uint8_t i = 0;
	unsigned char dat;
	point X1 = Pos;
	do{
		dat = pgm_read_byte(Buffer++);
		if(dat == '\n'){
			X1.Y += 8*scale;
			X1.X = Pos.X;
		}else{
			lcdDrawC( dat, X1, ForegroundColour, BackgroundColour, Font, scale);
			X1.X += 8*scale;
		}
	}while(dat);
	return i;
}

uint8_t lcdDrawD(const int Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale) {
	// TODO: Revise me, cope with negatives.
	int16_t I = 2, J = abs(Number);
	while (J >= 10) {
		I ++;
		J /= 10;
	}
	if (Number < 0) {
		I++;
	}

	char NumberString[I];
	itoa(Number, NumberString, 10);
	return lcdDrawS((unsigned char*)NumberString, Pos, ForegroundColour, BackgroundColour, Font, scale);
}
void lcdDrawDl(const int Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale, uint8_t len) {
	// TODO: Revise me, cope with negatives.
	char NumberString[len+1];
	itoa(Number, NumberString, 10);
	uint16_t i = 0;
	while(i<len){
		lcdDrawC((NumberString[i])?NumberString[i]:' ', (point){Pos.X+(scale*8*i),Pos.Y}, ForegroundColour, BackgroundColour, Font, scale);
		i++;
	}
}
uint8_t lcdDrawX(const uint8_t Value, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font * Font, uint8_t scale) {
	uint8_t c1=Value&0x0F,c2=(Value&0xF0)>>4;//load inital two values(0-15)
	c1=(c1>=10)?(c1+55):(c1+48);
	c2=(c2>=10)?(c2+55):(c2+48);//convert to characters
	lcdDrawC(c2, Pos, ForegroundColour, BackgroundColour, Font, scale);
	lcdDrawC(c1, (point){Pos.X+8,Pos.Y}, ForegroundColour, BackgroundColour, Font, scale);
	return 2;
}
//=================
// LCD DRAW SHAPES
//=================
void lcdDrawPixel(const point Pos, const uint16_t Colour) {
	lcdCanvasSet(Pos, Pos);
	lcdCmdWrite(0x2C);
	lcdWordWrite(Colour);
}
void lcdDrawBox(point A, point B, const uint16_t Colour) {
	point D;
	if (A.X > B.X) {
		D.X = A.X;
		A.X = B.X;
		B.X = D.X;
	}
	if (A.Y > B.Y) {
		D.Y = A.Y;
		A.Y = B.Y;
		B.Y = D.Y;
	}
	lcdCanvasSet(A, B);
	D.X = B.X - A.X + 1;
	D.Y = B.Y - A.Y + 1;
	uint32_t Pixels = (long)D.X * (long)D.Y;	//	TODO: Optimise this
	if (Pixels > 0xFFFF) {
		lcdRepeatedWordWrite(0x2C, Colour, ((uint16_t)Pixels) + 1);
		lcdRepeatedWordWrite(0x3C, Colour, 0xFFFF);
	} else {
		lcdRepeatedWordWrite(0x2C, Colour, (uint16_t)Pixels);
	}
}
void lcdDrawAll(const uint16_t Colour) {
	//lcdDrawBox(ORIGIN, LCD_MAX, Colour);//dosnt work with rotated displys
	if(lcdRot%2){//screen horisontal
		lcdCanvasSet((point){0,0}, (point){319,239});
	}else{//screen vertical
		lcdCanvasSet((point){0,0}, (point){239,319});
	}
	lcdRepeatedWordWrite(0x2C,Colour,40000);
	lcdRepeatedWordWrite(0x3C,Colour,36800);
}
void lcdDrawSprite(const sprite * Sprite, const point Pos) {
	uint16_t Pixel = 0;
	point I = Sprite->Size;
	lcdCanvasSet(Pos, (point){Pos.X+I.X-1,Pos.Y+I.Y-1});
	Pixel = I.X*I.Y;
	lcdWordArrayWrite(0x2C,(uint16_t*)Sprite->RGB,Pixel);
}
void lcdDrawSpriteScaled(const sprite * Sprite, const point Pos, const uint8_t ScaleX, const uint8_t ScaleY) {
	if (ScaleX == 1 && ScaleY == 1) {
		lcdDrawSprite(Sprite, Pos);
		return;
	} else if (!(ScaleX || ScaleY)){
		return;
	}
	uint8_t k = 2*Sprite->Size.X;
	uint8_t* rgb = (uint8_t*)Sprite->RGB;
	point J, I = Sprite->Size;
	lcdCanvasSet(Pos,(point){(Pos.X + (I.X * ScaleX)) - 1, (Pos.Y + (I.Y * ScaleY)) - 1})
	lcdCmdWrite(0x2C);
	while(I.Y--) {
		J.Y = ScaleY;
		while(J.Y--) {
			I.X = Sprite->Size.X;
			while (I.X--) {
				J.X = ScaleX;
				while (J.X--) {
					lcdByteWrite( *(rgb+1) );
					lcdByteWrite( *rgb );
				}
				rgb+=2;
			}
		if (I.X && J.Y) {rgb -= k;}
		}
	}
}
void lcdDrawSpritePartial(const sprite * Sprite, const point Pos, point X1, point X2){
	uint8_t x = X2.X-X1.X;
	uint8_t y = X2.Y-X1.Y;
	uint8_t * rgb;
	uint8_t i;
	lcdCanvasSet(Pos, (point){Pos.X+x,Pos.Y+y});
	if((x+1)>=(uint8_t)Sprite->Size.X){//same width just changed hight
		lcdWordArrayWrite(0x2C,((uint16_t*)Sprite->RGB)+((x+1)*X1.Y),(x+1)*(y+1));
	}else{
		rgb = (uint8_t*)((uint16_t*)((Sprite->RGB)+(Sprite->Size.X*X1.Y)+X1.X));//starting location
		y+=2;
		lcdCmdWrite(0x2C);
		while(--y){//loop through rows
			i=x+2;
			while(--i){//loop through colums
					lcdByteWrite( *(rgb+1) );
					lcdByteWrite( *rgb );
					rgb+=2;
			}
			rgb+=(Sprite->Size.X-(x+1))*2;
		}
	}
}
void lcdDrawSpriteScaledPartial(const sprite * Sprite, const point Pos, const uint8_t ScaleX, const uint8_t ScaleY, point X1, point X2){
	uint8_t x = X2.X-X1.X;
	uint8_t y = X2.Y-X1.Y;
	uint8_t * rgb;
	uint8_t i;
	uint8_t a,b,c=Sprite->Size.X*2,d=2*(x+1);
	lcdCanvasSet(Pos, (point){Pos.X+((x+1)*ScaleX)-1,Pos.Y+((y+1)*ScaleY)-1});
	rgb = (uint8_t*)((uint16_t*)((Sprite->RGB)+(Sprite->Size.X*X1.Y)+X1.X));//starting location
	y+=2;
	lcdCmdWrite(0x2C);
	//for(i=0;i<((x+1)*(y-1)*ScaleX*ScaleY);i++)lcdWordWrite(WHITE);
	while(--y){//loop through rows
		a=ScaleY;
		while(a--){
			i=x+2;
			while(--i){//loop through colums
					b = ScaleX;
					while(b--){
						lcdByteWrite( *(rgb+1) );
						lcdByteWrite( *rgb );
					}
					rgb+=2;
			}
			rgb-=d;
		}
		rgb+=c;
	}
}

void plot4points(const point Centre, const point Offset, const uint16_t Colour) {
	point A = {Center.X + Offset.X, Center.Y + Offset.Y};
	point B = {Centre.X - Offset.X, Centre.Y + Offset.Y};
	point C = {Centre.X + Offset.X, Centre.Y - Offset.Y};
	point D = {Centre.X - Offset.X, Centre.Y - Offset.Y};
	lcdDrawPixel(A, Colour);
	if (Offset.X) {
		lcdDrawPixel(B, Colour);
	}
	if (Offset.Y) {
		lcdDrawPixel(C, Colour);
	}
	if (Offset.X && Offset.Y) {
		lcdDrawPixel(D, Colour);
	}
}
void plot8points(const point Centre, const point Offset, const uint16_t Colour) {
  plot4points(Centre, Offset, Colour);
  if (Offset.X != Offset.Y) plot4points(Centre, (point){Offset.Y,Offset.X}, Colour);
}
void lcdDrawCircle(const point Centre, const uint8_t Radius, const uint16_t Colour) {
	//TODO:	Revise this.
	if (!Radius) {
		lcdDrawPixel(Centre, Colour);
		return;
	}
	int16_t Error = -Radius;
	point Pos = {Radius, 0};

	  // The following while loop may be altered to 'while (x > y)' for a
	  // performance benefit, as long as a call to 'plot4points' follows
	  // the body of the loop. This allows for the elimination of the
	  // '(x != y)' test in 'plot8points', providing a further benefit.
	  //
	  // For the sake of clarity, this is not shown here.
	while (Pos.X >= Pos.Y) {
		plot8points(Centre, Pos, Colour);

	    Error += Pos.Y;
	    Pos.Y++;
	    Error += Pos.Y;

	    // The following test may be implemented in assembly language in
	    // most machines by testing the carry flag after adding 'y' to
	    // the value of 'error' in the previous step, since 'error'
	    // nominally has a negative value.
	    if (Error >= 0) {
	    	Error -= Pos.X;
	    	Pos.X--;
	    	Error -= Pos.X;
	    }
	}
}

//====================
// LCD INTITALISATION
//====================
void lcdInit(const uint8_t Mode) {
	if (Mode) {
		//TODO: add splash and debug modes!
	}
	//	Disable JTAG
	MCUCR |= 0x80;
	MCUCR |= 0x80;
	//	Setup outputs
	CTRLDDR  = 0x7F;
	DATADDR  = 0xFF;
	//	Take all outputs high; keeping Reset, Backlight off and Chip selected.
	CTRLPORT  = 0b0111000;
	_delay_ms(16);
	CTRLPORT |= ~RESET;
	_delay_ms(128);
	lcdCmdWrite(0x28);
	lcdCmdWrite(0x11);
	lcdCmdWrite(0xCB);
	lcdByteWrite(0x01);
	lcdCmdWrite(0xC0);
	lcdByteWrite(0x26);
	lcdByteWrite(0x08);
	lcdCmdWrite(0xC1);
	lcdByteWrite(0x10);
	lcdCmdWrite(0xC5);
	lcdByteWrite(0x35);
	lcdByteWrite(0x3E);
	lcdCmdWrite(0x36); // RGB
	lcdByteWrite(0x48);
	lcdCmdWrite(0xB0);
	lcdByteWrite(0x4A);
	lcdCmdWrite(0xB1);
	lcdByteWrite(0x00);
	lcdByteWrite(0x1B);
	lcdCmdWrite(0xB6);
	lcdByteWrite(0x0A);
	lcdByteWrite(0x82);
	lcdByteWrite(0x27);
	lcdByteWrite(0x00);
	lcdCmdWrite(0xC7);
	lcdByteWrite(0xB5);
	lcdCmdWrite(0xF6);
	lcdByteWrite(0x01);
	lcdByteWrite(0x00);
	lcdByteWrite(0x00);
	lcdCmdWrite(0xF2);
	lcdByteWrite(0x00);
	lcdCmdWrite(0x26);
	lcdByteWrite(0x01);
	lcdCmdWrite(0x3A);
	lcdByteWrite(0x55);
	uint8_t PosGammaCorrections[15] = {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0x87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00};
	lcdByteArrayWrite(0xE0, PosGammaCorrections, 15);
	uint8_t NegGammaCorrections[15] = {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F};
	lcdByteArrayWrite(0xE1, NegGammaCorrections, 15);
	lcdCmdWrite(0x34); // Tearing OFF
	lcdCmdWrite(0xB4); // Inversion OFF
	lcdByteWrite(0x00);
	lcdCmdWrite(0xB7); // Entry Mode Set
	lcdByteWrite(0x07);
	lcdDrawAll(BLACK);
	lcdCmdWrite(0x29);
	_delay_ms(30);
	lcdBacklightState(1);
}
void memDump(){
	set_rotation(0);
	lcdCanvasSet((point){0,0}, (point){15,127});
	lcdWordArrayWrite(0x2C,(uint16_t*)0,2048);//draw the entire of memory as pixels
	uint16_t i, data;
	const uint8_t * ptr;
	lcdCanvasSet((point){32,0}, (point){160,255});
	lcdCmdWrite(0x2C);
	for(i=0,ptr=0;i<32768;i++,ptr+=2){//draw the entire of program memory as pixels
		data = pgm_read_dword(ptr);
		lcdWordWrite(data);
	}
}