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
// BASE LCD CMDS (now with weird inline style define based functions)
//===============
inline void pictorCmdWrite(const uint8_t Cmd) {
	CTRLPORT &=  COMND;
	DATAPORT  =  Cmd;
	CTRLPORT &=  WRITE;
	CTRLPORT |= ~WRITE;
	CTRLPORT |= ~COMND;
}

inline void pictorByteWrite(const uint8_t Data) {
	DATAPORT  =  Data;
	CTRLPORT &=  WRITE;
	CTRLPORT |= ~WRITE;
}

inline void pictorWordWrite(uint16_t Value) {
	pictorByteWrite( (uint8_t)(Value >> 8) );
	pictorByteWrite( (uint8_t)(Value & 0xFF) );
}

inline uint8_t pictorByteRead() {
	uint8_t Data;
	CTRLPORT &=  READ;
	Data   =  DATAPINS;
	CTRLPORT |= ~READ;
	return Data;
}

inline uint16_t pictorWordRead() {
	return ( ( (uint16_t)pictorByteRead() ) << 8 ) + pictorByteRead();
}

void pictorBacklightState(const int8_t State) {
	if (State > 0) {
		CTRLPORT |=  BACKLIGHT;
	} else if (!State) {
		CTRLPORT &= ~BACKLIGHT;
	} else {
		CTRLPORT ^=  BACKLIGHT;
	}
}

void pictorFrame() {
	CTRLPORT &= ~VSYNC;
	CTRLPORT |= VSYNC;
}

//===============
// LCD BULK CMDS
//===============
void pictorArrayWrite(const uint8_t* Data, const uint16_t Count){
#ifndef PICTOR_FASTMODE
	uint16_t I = 0;
	while (I < Count) {
		pictorByteWrite( Data[I++] );
	}
#else
	uint8_t HighCount = (uint8_t)(Count >> 8);
	uint8_t LowCount = (uint8_t)Count & 0xFF; //std name
	uint8_t I;
	uint16_t Index = 0;
	while (HighCount--) { // moved from for loops to while loops
		I = 17;
		while (--I) {//moved from post-decrement to pre-decrement (because somehow it's faster)
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

void pictorWordArrayWrite(const uint8_t Cmd, const uint16_t* Data, const uint16_t Count) {
#ifndef PICTOR_FASTMODE	
	uint8_t I = 0;
	pictorCmdWrite(Cmd);
	while (I < Count) {
		pictorWordWrite( Data[I++] );
	}
#else
	const uint8_t* Byte = (uint8_t*)Data;
	pictorCmdWrite(Cmd);
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
#endif
}

void pictorRepeatedByteWrite(const uint8_t Cmd, const uint8_t Data, uint16_t Count) { // NEW
#ifndef PICTOR_FASTMODE
	pictorCmdWrite(Cmd);
	Count++;
	while (--Count) { // moved from for loops to while loops(using pre-decrements rather than post-decrements)
		pictorByteWrite(Data);
	}
#else
	uint8_t HighCount = (uint8_t)(Count >> 8);
	uint8_t LowCount = (uint8_t)Count & 0xFF; //std name
	uint8_t I;
	pictorCmdWrite(Cmd);
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
#endif
}

void pictorRepeatedWordWrite(const uint8_t Cmd, const uint16_t Data, uint16_t Count) { // NEW
#ifndef PICTOR_FASTMODE
	uint8_t HighByte = (uint8_t)(Data >> 8);
	uint8_t LowByte  = (uint8_t)(Data & 0xFF);
	pictorCmdWrite(Cmd);
	while (Count--) {
		pictorByteWrite(HighByte);
		pictorByteWrite(LowByte);
	}
#else
	uint8_t I; // std name
	uint8_t HighCount = (uint8_t)(Count >> 8),			LowCount = (uint8_t)Count & 0xFF;//std name
	uint8_t HighByte = (uint8_t)(Data >> 8),	LowByte = (uint8_t)(Data) & 0xFF;//added for opti, optimised brackets & std name
	pictorCmdWrite(Cmd);
	while (HighCount--) { // moved from for loops to while loops
		I = 17;
		while (--I) { // 16x16 is using a fair bit of memory(pre-decrement rather than post-decrement)
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
#endif
}

void pictorWrite(const uint8_t Cmd, const uint8_t* Buffer, const uint8_t Count){
	pictorCmdWrite(Cmd);
	pictorArrayWrite(Buffer,Count);
}

void pictorRead(const uint8_t Cmd, uint8_t* Buffer, const uint8_t Count) {
	uint8_t I = 0;
	pictorCmdWrite(Cmd);
	DATAPORT  =  0x00;
	DATADDR   =  0x00;
	CTRLPORT &=  READ;
	CTRLPORT |= ~READ;
	while (I < Count) {
		Buffer[I++] = pictorByteRead();
	}
	DATADDR   =  0xFF;
}

//====================
// LCD COMPOSITE CMDS
//====================
void pictorCanvasSet(const point A, const point B) {
	pictorCmdWrite(0x2A); // Column Address Set
	pictorWordWrite(A.X);
	pictorWordWrite(B.X);
	pictorCmdWrite(0x2B); // Page Address Set
	pictorWordWrite(A.Y);
	pictorWordWrite(B.Y);
}

void pictorSetRotation(const uint8_t rotation){
	pictorCmdWrite(0x36);
	switch(rotation){
		case 0:
			pictorByteWrite(0x48);
			break;
		case 1:
			pictorByteWrite(0x28);
			break;
		case 2:
			pictorByteWrite(0x88);
			break;
		case 3:
			pictorByteWrite(0xE8);
			break;
	}
	pictorRot = rotation;
}

//=========================
// LCD DRAW CHAR FUNCTIONS
//=========================
void pictorDrawC(const unsigned char Char, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const uint8_t* Font, uint8_t scale){
	if (Char < PICTOR_FONTSTART || Char > PICTOR_FONTEND) return;
	const uint8_t* fdata;
	uint8_t bits, mask, I, J;
	uint8_t k, l;
	uint8_t HighFCol = (uint8_t)(ForegroundColour >> 8), LowFCol = (uint8_t)(ForegroundColour & 0xFF);//added for opti, standardised name
	uint8_t HighBCol = (uint8_t)(BackgroundColour >> 8), LowBCol = (uint8_t)(BackgroundColour & 0xFF);//added for opti, standardised name
	point Pos2 = {Pos.X + (8*scale) - 1, Pos.Y + (8*scale) - 1};
	pictorCanvasSet(Pos, Pos2);
	fdata = (Char - PICTOR_FONTSTART)*8 + Font;
	pictorCmdWrite(0x2C); // Memory Write
	for (I = 0; I < 8; I++) {//draw horizontal line
		bits = pgm_read_byte(fdata++);
		for(k = 0;k<scale;k++){//draw repeated horizontal line for scale factor
			for (J = 0, mask=0x80; J < 8; J++, mask>>=1) {//draw each pixel on line
				for(l = 0;l<scale;l++){//draw repeats for scale
					if(bits & mask){
						pictorByteWrite(HighFCol);//added for opti
						pictorByteWrite(LowFCol);//added for opti
					}else{
						pictorByteWrite(HighBCol);//added for opti
						pictorByteWrite(LowBCol);//added for opti
					}
				}
			}
		}
	}
}

uint8_t pictorDrawS(const unsigned char* Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale) {
	uint8_t i = 0;
	point X1 = Pos;
	while (Buffer[i]) {
		if(Buffer[i] == '\n'){
			X1.Y += 8*scale;
			X1.X = Pos.X;
			i++;
		}else{
			pictorDrawC( Buffer[i++], X1, ForegroundColour, BackgroundColour, Font, scale);
			X1.X += 8*scale;
		}
	}
	return i;
}


uint8_t pictorDrawSP(const unsigned char* Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale) {
	uint8_t i = 0;
	unsigned char dat;
	point X1 = Pos;
	do{
		dat = pgm_read_byte(Buffer++);
		if(dat == '\n'){
			X1.Y += 8*scale;
			X1.X = Pos.X;
		}else{
			pictorDrawC( dat, X1, ForegroundColour, BackgroundColour, Font, scale);
			X1.X += 8*scale;
		}
	}while(dat);
	return i;
}

//=========================
// LCD DRAW Number FUNCTIONS
//=========================
void pictorDrawD(const int16_t Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale, uint8_t len) {
	char NumberString[7];
	itoa(Number, NumberString, 10);
	point Pos2 = Pos;
	if(len) {
		uint8_t i = 0;
		while(NumberString[i])i++;
		while(i<len){
			pictorDrawC(32, Pos2, ForegroundColour, BackgroundColour, Font, scale);
			Pos2.X += scale*8;
			i++;
		}
	}
	pictorDrawS((unsigned char*)NumberString, Pos2, ForegroundColour, BackgroundColour, Font, scale);
}

uint8_t pictorDrawX(const uint8_t Value, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale) {
	uint8_t c1=Value&0x0F,c2=(Value&0xF0)>>4;//load initial two values(0-15)
	c1=(c1>=10)?(c1+55):(c1+48);
	c2=(c2>=10)?(c2+55):(c2+48);//convert to characters
	pictorDrawC(c2, Pos, ForegroundColour, BackgroundColour, Font, scale);
	pictorDrawC(c1, (point){Pos.X+8*scale,Pos.Y}, ForegroundColour, BackgroundColour, Font, scale);
	return 2;
}

//=================
// LCD DRAW SHAPES
//=================
void pictorDrawPixel(const point Pos, const uint16_t Colour) {
	pictorCanvasSet(Pos, Pos);
	pictorCmdWrite(0x2C);
	pictorWordWrite(Colour);
}

void pictorDrawLine(point A, point B, const uint16_t Colour) {
	//Nested function declarations are forbidden in standard C but the avr-gcc compiler allows them.
	//The swap function swaps the values of int1 and int2; it is only accessible in this block.
	void swap(uint16_t *int1 , uint16_t *int2) {
		uint16_t temp;
		 temp = *int1;
		*int1 = *int2;
		*int2 =  temp;
	}
	//if the slope is higher than 1
	//mirror across the diagonal
	//e.g. increment Y instead of X in the main for loop
	uint8_t steep = 0;
	if(abs(A.X - B.X) < abs(A.Y - B.Y)) {
		swap(&A.X,&A.Y);
		swap(&B.X,&B.Y);
		steep = 1;
	}
	//if the first point is to the right of the second
	//swap them
	if(A.X>B.X) {
		swap(&A.X,&B.X);
		swap(&A.Y,&B.Y);
	}
	//if the second point is above the first
	//decrement y, as opposed to increment, in the main for loop
	int8_t y_inc;
	if(B.Y>A.Y)
		y_inc = 1;
	else
		y_inc = -1;
	point Pos; //position of pixel which is drawn each loop
	int16_t offsetX = abs(B.X - A.X);
	int16_t offsetY = abs(B.Y - A.Y);
	int16_t err = 0;
	for (Pos.X = A.X,Pos.Y = A.Y; Pos.X <= B.X; Pos.X++) {
		//line is transposed back before being drawn
		if(steep)
			pictorDrawPixel( (point) {Pos.Y, Pos.X}, Colour);
		else
			pictorDrawPixel(Pos, Colour);

		err += offsetY;
		if( (err<<1) >= offsetX) {
			Pos.Y += y_inc;
			err -= offsetX;
		}
	}
}

void pictorDrawBox(point A, point B, const uint16_t Colour) {
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
	pictorCanvasSet(A, B);
	D.X = B.X - A.X + 1;
	D.Y = B.Y - A.Y + 1;
	uint32_t Pixels = (uint32_t)D.X * (uint32_t)D.Y;	//	TODO: Optimise this
	if (Pixels > 0xFFFF) {
		pictorRepeatedWordWrite(0x2C, Colour, ((uint16_t)Pixels) + 1);
		pictorRepeatedWordWrite(0x3C, Colour, 0xFFFF);
	} else {
		pictorRepeatedWordWrite(0x2C, Colour, (uint16_t)Pixels);
	}
}

void pictorDrawAll(const uint16_t Colour) {
	if(pictorRot%2){//screen horizontal
		pictorCanvasSet((point){0,0}, (point){319,239});
	}else{//screen vertical
		pictorCanvasSet((point){0,0}, (point){239,319});
	}
	pictorRepeatedWordWrite(0x2C,Colour,32768);
	pictorRepeatedWordWrite(0x3C,Colour,44032);
}

void pictorDrawSpritePartial_(const sprite* Sprite, const point Pos, point X1, point X2){
	uint8_t x = X2.X-X1.X;
	uint8_t y = X2.Y-X1.Y;
	uint8_t * rgb;
	uint8_t i;
	pictorCanvasSet(Pos, (point){Pos.X+x,Pos.Y+y});
	if((x+1)>=(uint8_t)Sprite->Size.X){//same width just changed height
		pictorWordArrayWrite(0x2C,((uint16_t*)Sprite->RGB)+((x+1)*X1.Y),(x+1)*(y+1));
	}else{
		rgb = (uint8_t*)((uint16_t*)((Sprite->RGB)+(Sprite->Size.X*X1.Y)+X1.X));//starting location
		y+=2;
		pictorCmdWrite(0x2C);
		while(--y){//loop through rows
			i=x+2;
			while(--i){//loop through columns
					pictorByteWrite( *(rgb+1) );
					pictorByteWrite( *rgb );
					rgb+=2;
			}
			rgb+=(Sprite->Size.X-(x+1))*2;
		}
	}
}

void pictorDrawSpritePartial(const sprite* Sprite, const point Pos, const uint8_t Scale, point X1, point X2){
	if (Scale == 1) {
		pictorDrawSpritePartial_(Sprite, Pos, X1, X2);
		return;
	}
	uint8_t x = X2.X-X1.X;
	uint8_t y = X2.Y-X1.Y;
	uint8_t* rgb;
	uint8_t i;
	uint8_t a,b,c=Sprite->Size.X*2,d=2*(x+1);
	pictorCanvasSet(Pos, (point){Pos.X+((x+1)*Scale)-1,Pos.Y+((y+1)*Scale)-1});
	rgb = (uint8_t*)((uint16_t*)((Sprite->RGB)+(Sprite->Size.X*X1.Y)+X1.X));//starting location
	y+=2;
	pictorCmdWrite(0x2C);
	//for(i=0;i<((x+1)*(y-1)*ScaleX*ScaleY);i++)lcdWordWrite(WHITE);
	while(--y){//loop through rows
		a=Scale;
		while(a--){
			i=x+2;
			while(--i){//loop through columns
					b = Scale;
					while(b--){
						pictorByteWrite( *(rgb+1) );
						pictorByteWrite( *rgb );
					}
					rgb+=2;
			}
			rgb-=d;
		}
		rgb+=c;
	}
}

void pictorDrawSpriteType_(const void* Sprite, const point Pos, const uint8_t type) {
	uint8_t* Buffer;
	uint8_t* byteCur;
	uint8_t datH, datL;//only used for types 1,3,5,7
	uint8_t number=0, colour=0;//only used for types 4-7
	uint8_t mask = 0x80;//only used for types 2-3
	uint8_t HighFCol, LowFCol, HighBCol, LowBCol;//only used for types 2-3
	uint8_t ProgMem = (type & _BV(0)), k = 0;
	uint16_t j = 0, j2;
	point I;
	uint16_t _palette[16];
	uint16_t* Palette =_palette;
	
	//void pointer typecasting needed 
	//3 pointers added for the sprite type to avoid continual casting
	sprite* SpriteReg = NULL;
	spriteMono* SpriteMono = NULL;
	spriteEncoded* SpriteIndex = NULL;
	spritePalette* SpritePacked = NULL;
	
	switch(type) {
		case 0x0B:
		case 0x0A:
		case 9:
		case 8:
			SpritePacked = (spritePalette*)Sprite;
			I = SpritePacked->Size;
			byteCur = (uint8_t*)(SpritePacked->data);
			break;
		case 7:
		case 6:
		case 5:
		case 4:
			SpriteIndex = (spriteEncoded*)Sprite;
			I = SpriteIndex->Size;
			byteCur = (uint8_t*)(SpriteIndex->data);
			break;
		case 3:
		case 2:
			SpriteMono = (spriteMono*)Sprite;
			I = SpriteMono->Size;
			byteCur = (uint8_t*)(SpriteMono->data);
			HighFCol = (uint8_t)(SpriteMono->FGcolour >> 8);//added for opti, standardised name
			LowFCol = (uint8_t)(SpriteMono->FGcolour & 0xFF);
			HighBCol = (uint8_t)(SpriteMono->BGcolour >> 8);
			LowBCol = (uint8_t)(SpriteMono->BGcolour & 0xFF);
			break;
		case 1:
		case 0:
			SpriteReg = (sprite*)Sprite;
			I = SpriteReg->Size;
			byteCur = (uint8_t*)(SpriteReg->RGB);
			j2 = I.Y * I.X;
			break;
	}
	
	pictorCanvasSet(Pos,(point){(Pos.X + I.X) - 1, (Pos.Y + I.Y) - 1});
	pictorCmdWrite(0x2C);
	
	switch(type) {
		case 0x0B:
			Buffer = (uint8_t*) SpritePacked->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(Buffer++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(Buffer++))<<8;
			}
			j = (I.Y * I.X)>>1;
			while(j--) {
				datL = pgm_read_byte(byteCur);
				byteCur++;
				datH = datL>>4;
				datL &= 0x0F;
				pictorWordWrite(Palette[datH]);//write upper pixel
				pictorWordWrite(Palette[datL]);//write lower pixel
			}
			if((I.Y * I.X) & 0x01) {
				datH = pgm_read_byte(byteCur)>>4;
				pictorWordWrite(Palette[datH]);//write lower pixel
			}
			break;
		case 0x0A:
			Buffer = (uint8_t*) SpritePacked->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(Buffer++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(Buffer++))<<8;
			}
			j = (I.Y * I.X)>>1;
			while(j--) {
				datL = (*byteCur);
				byteCur++;
				datH = datL>>4;
				datL &= 0x0F;
				pictorWordWrite(Palette[datH]);//write upper pixel
				pictorWordWrite(Palette[datL]);//write lower pixel
			}
			if((I.Y * I.X) & 0x01) {
				datH = (*byteCur)>>4;
				pictorWordWrite(Palette[datH]);//write lower pixel
			}
			break;
		case 9:
			Palette = SpritePacked->palette;//use palette in sprite
			j = (I.Y * I.X)>>1;
			while(j--) {
				datL = pgm_read_byte(byteCur);
				byteCur++;
				datH = datL>>4;
				datL &= 0x0F;
				pictorWordWrite(Palette[datH]);//write upper pixel
				pictorWordWrite(Palette[datL]);//write lower pixel
			}
			if((I.Y * I.X) & 0x01) {
				datH = pgm_read_byte(byteCur)>>4;
				pictorWordWrite(Palette[datH]);//write lower pixel
			}
			break;
		case 8:
			Palette = SpritePacked->palette;//use palette in sprite
			j = (I.Y * I.X)>>1;
			while(j--) {
				datL = (*byteCur);
				byteCur++;
				datH = datL>>4;
				datL &= 0x0F;
				pictorWordWrite(Palette[datH]);//write upper pixel
				pictorWordWrite(Palette[datL]);//write lower pixel
			}
			if((I.Y * I.X) & 0x01) {
				datH = (*byteCur)>>4;
				pictorWordWrite(Palette[datH]);//write lower pixel
			}
			break;
		case 7:
			Buffer = (uint8_t*) SpriteIndex->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(Buffer++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(Buffer++))<<8;
			}
			j = I.Y * I.X;
			while(j--) {
				datL = pgm_read_byte(byteCur++);
				colour = (uint8_t) ((datL>>4) & 0x0F);//fetch and break bytes into colour and number
				number = (uint8_t) (datL & 0x0F);
				
				if (number == 0) {//single pixel of colour
					pictorWordWrite(Palette[colour]);//write colour
				} else if (number > j) {//overflow avoidance
					k = (j + 1);
					while(k--) pictorWordWrite(Palette[colour]);//write colour
					j = 0;
				} else {//multiple pixels of colour inside the sprite
					k = (number + 1);
					while (k--) pictorWordWrite(Palette[colour]);//write colour
					j -= number;
				}
			}
			break;
		case 6:
			Buffer = (uint8_t*) SpriteIndex->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(Buffer++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(Buffer++))<<8;
			}
			j = I.Y * I.X;
			while(j--) {
				colour = (uint8_t) ((*byteCur>>4) & 0x0F);//fetch and break bytes into colour and number
				number = (uint8_t) (*byteCur & 0x0F);
				byteCur++;
				
				if (number == 0) {//single pixel of colour
					pictorWordWrite(Palette[colour]);//write colour
				} else if (number > j) {//overflow avoidance
					k = (j + 1);
					while(k--) pictorWordWrite(Palette[colour]);//write colour
					j = 0;
				} else {//multiple pixels of colour inside the sprite
					k = (number + 1);
					while (k--) pictorWordWrite(Palette[colour]);//write colour
					j -= number;
				}
			}
			break;
		case 5:
			Palette = SpriteIndex->palette;//use palette in sprite
			j = I.Y * I.X;
			while(j--) {
				datL = pgm_read_byte(byteCur);
				byteCur++;
				colour = (uint8_t) ((datL>>4) & 0x0F);//fetch and break bytes into colour and number
				number = (uint8_t) (datL & 0x0F);
				
				if (number == 0) {//single pixel of colour
					pictorWordWrite(Palette[colour]);//write colour
				} else if (number > j) {//overflow avoidance
					k = (j + 1);
					while(k--) pictorWordWrite(Palette[colour]);//write colour
					j = 0;
				} else {//multiple pixels of colour inside the sprite
					k = (number + 1);
					while (k--) pictorWordWrite(Palette[colour]);//write colour
					j -= number;
				}
			}
			break;
		case 4:
			Palette = SpriteIndex->palette;//use palette in sprite
			j = I.Y * I.X;
			while(j--) {
				colour = (uint8_t) ((*byteCur>>4) & 0x0F);//fetch and break bytes into colour and number
				number = (uint8_t) (*byteCur & 0x0F);
				byteCur++;
				
				if (number == 0) {//single pixel of colour
					pictorWordWrite(Palette[colour]);//write colour
				} else if (number > j) {//overflow avoidance
					k = (j + 1);
					while(k--) pictorWordWrite(Palette[colour]);//write colour
					j = 0;
				} else {//multiple pixels of colour inside the sprite
					k = (number + 1);
					while (k--) pictorWordWrite(Palette[colour]);//write colour
					j -= number;
				}
			}
			break;
		case 3:
			for(j=0;j<j2;j+=8){
				datL = pgm_read_byte(byteCur++);//fetch sequential bytes
				if(datL & 0x80){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(datL & 0x40){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(datL & 0x20){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(datL & 0x10){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(datL & 0x08){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(datL & 0x04){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(datL & 0x02){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(datL & 0x01){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
			}
			break;
		case 2:
			for(j=0;j<j2;j+=8){
				if(*byteCur & 0x80){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(*byteCur & 0x40){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(*byteCur & 0x20){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(*byteCur & 0x10){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(*byteCur & 0x08){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(*byteCur & 0x04){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(*byteCur & 0x02){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				if(*byteCur & 0x01){pictorByteWrite( HighFCol );pictorByteWrite( LowFCol );}else{pictorByteWrite( HighBCol );pictorByteWrite( LowBCol );}
				byteCur++;//fetch sequential bytes
			}
			break;
		case 1:
			j = I.X * I.Y;
			while(j--) {
				datH = pgm_read_byte(byteCur+1);
				datL = pgm_read_byte(byteCur);
				pictorByteWrite( datH );
				pictorByteWrite( datL );
				byteCur+=2;
			}
			break;
		case 0:
			j = I.X * I.Y;
			pictorWordArrayWrite(0x2C,(uint16_t*)SpriteReg->RGB,j);
			break;
	}
}

void pictorDrawSpriteType(const void* Sprite, const point Pos, const uint8_t type, const uint8_t Scale) {
	if (Scale == 1) {
		pictorDrawSpriteType_(Sprite, Pos, type);
		return;
	} else if (!Scale) {
		return;//if scale is 0 then nothing to draw
	}
	
	uint8_t* byteStart;
	uint8_t* byteCur;
	uint8_t datH, datL;//only used for types 1,3,5,7
	uint8_t number=0, lastCol, lastNum, endNum=0xFF, colour=0;//only used for types 4-7
	uint8_t maskStart, mask = 0x80;//only used for types 2-3
	uint8_t ProgMem = (type & _BV(0));
	uint8_t HighFCol, LowFCol, HighBCol, LowBCol;//only used for types 2-3
	point J, I;
	uint16_t _palette[16];
	uint16_t* Palette = _palette;
	
	//void pointer typecasting needed 
	//4 pointers added for the sprite type to avoid continual casting
	sprite* SpriteReg = NULL;
	spriteMono* SpriteMono = NULL;
	spriteEncoded* SpriteIndex = NULL;
	spritePalette* SpritePacked = NULL;
	
	switch(type) {
		case 0x0B:
		case 0x0A:
		case 9:
		case 8:
			SpritePacked = (spritePalette*)Sprite;
			I = SpritePacked->Size;
			byteCur = (uint8_t*)(SpritePacked->data);
			break;
		case 7:
		case 6:
		case 5:
		case 4:
			SpriteIndex = (spriteEncoded*)Sprite;
			I = SpriteIndex->Size;
			byteCur = (uint8_t*)(SpriteIndex->data);
			break;
		case 3:
		case 2:
			SpriteMono = (spriteMono*)Sprite;
			I = SpriteMono->Size;
			byteCur = (uint8_t*)(SpriteMono->data);
			HighFCol = (uint8_t)(SpriteMono->FGcolour >> 8);//added for opti, standardised name
			LowFCol = (uint8_t)(SpriteMono->FGcolour & 0xFF);
			HighBCol = (uint8_t)(SpriteMono->BGcolour >> 8);
			LowBCol = (uint8_t)(SpriteMono->BGcolour & 0xFF);
			break;
		case 1:
		case 0:
			SpriteReg = (sprite*)Sprite;
			I = SpriteReg->Size;
			byteCur = (uint8_t*)(SpriteReg->RGB);
			break;
	}
	
	pictorCanvasSet(Pos,(point){(Pos.X + (I.X * Scale)) - 1, (Pos.Y + (I.Y * Scale)) - 1});
	pictorCmdWrite(0x2C);
	
	switch(type) {
		case 0x0B:
			byteStart = (uint8_t*) SpritePacked->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(byteStart++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(byteStart++))<<8;
			}
			endNum = 0;
			while(I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;
				lastNum = endNum;
				number = datL;
				while(J.Y--) {
					I.X = SpritePacked->Size.X;
					endNum = 0;
					if(lastNum) {//clause for starting in the middle of a byte
						byteCur++;
						I.X--;
						J.X = Scale;
						while (J.X--) pictorWordWrite(Palette[number]);//write lower pixel
					}
					while (I.X--) {
						if(I.X) {
							datL = pgm_read_byte(byteCur);
							byteCur++;
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datH]);//write upper pixel
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datL]);//write lower pixel
							I.X--;
						} else {//when ending in the middle of a byte
							endNum = 1;
							datL = pgm_read_byte(byteCur);
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) {
								pictorWordWrite(Palette[datH]);//write lower pixel
							}
						}
					}
				if (J.Y) {byteCur = byteStart;}
				}
			}
			break;
		case 0x0A:
			byteStart = (uint8_t*) SpritePacked->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(byteStart++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(byteStart++))<<8;
			}
			endNum = 0;
			while(I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;
				lastNum = endNum;
				number = datL;
				while(J.Y--) {
					I.X = SpritePacked->Size.X;
					endNum = 0;
					if(lastNum) {//clause for starting in the middle of a byte
						byteCur++;
						I.X--;
						J.X = Scale;
						while (J.X--) pictorWordWrite(Palette[number]);//write lower pixel
					}
					while (I.X--) {
						if(I.X) {
							datL = *byteCur;
							byteCur++;
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datH]);//write upper pixel
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datL]);//write lower pixel
							I.X--;
						} else {//when ending in the middle of a byte
							endNum = 1;
							datL = *byteCur;
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) {
								pictorWordWrite(Palette[datH]);//write lower pixel
							}
						}
					}
				if (J.Y) {byteCur = byteStart;}
				}
			}
			break;
		case 9:
			Palette = SpritePacked->palette;//use palette in sprite
			endNum = 0;
			while(I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;
				lastNum = endNum;
				number = datL;
				while(J.Y--) {
					I.X = SpritePacked->Size.X;
					endNum = 0;
					if(lastNum) {//clause for starting in the middle of a byte
						byteCur++;
						I.X--;
						J.X = Scale;
						while(J.X--)pictorWordWrite(Palette[number]);//write lower pixel
					}
					while (I.X--) {
						if(I.X) {
							datL = pgm_read_byte(byteCur);
							byteCur++;
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datH]);//write upper pixel
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datL]);//write lower pixel
							I.X--;
						} else {//when ending in the middle of a byte
							endNum = 1;
							datL = pgm_read_byte(byteCur);
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) {
								pictorWordWrite(Palette[datH]);//write lower pixel
							}
						}
					}
				if (J.Y) {byteCur = byteStart;}
				}
			}
			break;
		case 8:
			Palette = SpritePacked->palette;//use palette in sprite
			endNum = 0;
			while(I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;
				lastNum = endNum;
				number = datL;
				while(J.Y--) {
					I.X = SpritePacked->Size.X;
					endNum = 0;
					if(lastNum) {//clause for starting in the middle of a byte
						byteCur++;
						I.X--;
						J.X = Scale;
						while(J.X--)pictorWordWrite(Palette[number]);//write lower pixel
					}
					while (I.X--) {
						if(I.X) {
							datL = *byteCur;
							byteCur++;
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datH]);//write upper pixel
							J.X = Scale;
							while (J.X--) pictorWordWrite(Palette[datL]);//write lower pixel
							I.X--;
						} else {//when ending in the middle of a byte
							endNum = 1;
							datL = *byteCur;
							datH = datL>>4;
							datL &= 0x0F;
							J.X = Scale;
							while (J.X--) {
								pictorWordWrite(Palette[datH]);//write lower pixel
							}
						}
					}
				if (J.Y) {byteCur = byteStart;}
				}
			}
			break;
		case 7:
			byteStart = (uint8_t*) SpriteIndex->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(byteStart++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(byteStart++))<<8;
			}
			while(I.Y--) {
				J.Y = Scale;
				//handle number counting round onto new line
				lastCol = colour;//Store colour and remaining number for start of line
				lastNum = endNum;
				number = lastNum;
				endNum = 0xFF;
				byteStart = byteCur;//Store address of first new byte of the line
				while(J.Y--) {
					I.X = SpriteIndex->Size.X;
					while (I.X--) {
						J.X = Scale;
						if(number > 20) {//process new line byte before reading new byte
							datL = pgm_read_byte(byteCur);
							byteCur++;
							colour = (uint8_t) ((datL>>4) & 0x0F);//fetch and break bytes into colour and number
							number = (uint8_t) (datL & 0x0F);
						}
						if (number == 0) {//single pixel of colour
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
						} else if (number <= I.X) {
							J.X *= (number + 1);
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
							I.X -= number;
						} else if (number > I.X) {
							J.X *= (I.X + 1);
							while(J.X--) pictorWordWrite(Palette[colour]);//write colour
							endNum = (number - I.X)-1;//take this value for flowing onto the next line
							I.X = 0;
						}
						number = 0xFF;
					}
					if(J.Y) {
						colour = lastCol;//load start of line values into colour and number
						number = lastNum;
						byteCur = byteStart;
					}
				}
			}
			break;
		case 6:
			byteStart = (uint8_t*) SpriteIndex->palette;
			for(datH=0;datH<16;datH++){//copy sprite palette to Palette
				Palette[datH] = ((uint8_t) pgm_read_byte(byteStart++));
				Palette[datH] |= ((uint8_t) pgm_read_byte(byteStart++))<<8;
			}
			while(I.Y--) {
				J.Y = Scale;
				//handle number counting round onto new line
				lastCol = colour;//Store colour and remaining number for start of line
				lastNum = endNum;
				number = lastNum;
				endNum = 0xFF;
				byteStart = byteCur;//Store address of first new byte of the line
				while(J.Y--) {
					I.X = SpriteIndex->Size.X;
					while (I.X--) {
						J.X = Scale;
						if(number > 20) {//process new line byte before reading new byte
							colour = (uint8_t) ((*byteCur>>4) & 0x0F);//fetch and break bytes into colour and number
							number = (uint8_t) (*byteCur & 0x0F);
							byteCur++;
						}
						if (number == 0) {//single pixel of colour
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
						} else if (number <= I.X) {
							J.X *= (number + 1);
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
							I.X -= number;
						} else if (number > I.X) {
							J.X *= (I.X + 1);
							while(J.X--) pictorWordWrite(Palette[colour]);//write colour
							endNum = (number - I.X)-1;//take this value for flowing onto the next line
							I.X = 0;
						}
						number = 0xFF;
					}
					if(J.Y) {
						colour = lastCol;//load start of line values into colour and number
						number = lastNum;
						byteCur = byteStart;
					}
				}
			}
			break;
		case 5:
			Palette = SpriteIndex->palette;//use palette in sprite
			while(I.Y--) {
				J.Y = Scale;
				//handle number counting round onto new line
				lastCol = colour;//Store colour and remaining number for start of line
				lastNum = endNum;
				number = lastNum;
				endNum = 0xFF;
				byteStart = byteCur;//Store address of first new byte of the line
				while(J.Y--) {
					I.X = SpriteIndex->Size.X;
					while (I.X--) {
						J.X = Scale;
						if(number > 20) {//process new line byte before reading new byte
							datL = pgm_read_byte(byteCur);
							byteCur++;
							colour = (uint8_t) ((datL>>4) & 0x0F);//fetch and break bytes into colour and number
							number = (uint8_t) (datL & 0x0F);
						}
						if (number == 0) {//single pixel of colour
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
						} else if (number <= I.X) {
							J.X *= (number + 1);
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
							I.X -= number;
						} else if (number > I.X) {
							J.X *= (I.X + 1);
							while(J.X--) pictorWordWrite(Palette[colour]);//write colour
							endNum = (number - I.X)-1;//take this value for flowing onto the next line
							I.X = 0;
						}
						number = 0xFF;
					}
					if(J.Y) {
						colour = lastCol;//load start of line values into colour and number
						number = lastNum;
						byteCur = byteStart;
					}
				}
			}
			break;
		case 4:
			Palette = SpriteIndex->palette;//use palette in sprite
			while(I.Y--) {
				J.Y = Scale;
				//handle number counting round onto new line
				lastCol = colour;//Store colour and remaining number for start of line
				lastNum = endNum;
				number = lastNum;
				endNum = 0xFF;
				byteStart = byteCur;//Store address of first new byte of the line
				while(J.Y--) {
					I.X = SpriteIndex->Size.X;
					while (I.X--) {
						J.X = Scale;
						if(number > 20) {//process new line byte before reading new byte
							colour = (uint8_t) ((*byteCur>>4) & 0x0F);//fetch and break bytes into colour and number
							number = (uint8_t) (*byteCur & 0x0F);
							byteCur++;
						}
						if (number == 0) {//single pixel of colour
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
						} else if (number <= I.X) {
							J.X *= (number + 1);
							while (J.X--) pictorWordWrite(Palette[colour]);//write colour
							I.X -= number;
						} else if (number > I.X) {
							J.X *= (I.X + 1);
							while(J.X--) pictorWordWrite(Palette[colour]);//write colour
							endNum = (number - I.X)-1;//take this value for flowing onto the next line
							I.X = 0;
						}
						number = 0xFF;
					}
					if(J.Y) {
						colour = lastCol;//load start of line values into colour and number
						number = lastNum;
						byteCur = byteStart;
					}
				}
			}
			break;
		case 3:
			while (I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;//record the bit location of the start of the line
				maskStart = mask;//record the read address of the start of the line
				while (J.Y--) {
					I.X = SpriteMono->Size.X;
					datL = pgm_read_byte(byteCur);//fetch byte
					while (I.X--) {
						J.X = Scale;
						while (J.X--) {
							if (datL & mask) {//bit is a 1
								pictorByteWrite( HighFCol );
								pictorByteWrite( LowFCol );
							} else {
								pictorByteWrite( HighBCol );
								pictorByteWrite( LowBCol );
							}
						}
						if(!(mask>>=1)){
							mask = 0x80;//extract sequential bits from bytes
							datL = ((ProgMem)?pgm_read_byte(++byteCur):(*(++byteCur)));//fetch sequential bytes
						}
					}
					if (J.Y) {
						byteCur = byteStart;//reset read address to start of line
						mask = maskStart;//reset mask to start of line
					}
				}
			}
			break;
		case 2:
			while (I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;//record the bit location of the start of the line
				maskStart = mask;//record the read address of the start of the line
				while (J.Y--) {
					I.X = SpriteMono->Size.X;
					while (I.X--) {
						J.X = Scale;
						while (J.X--) {
							if (*byteCur & mask) {//bit is a 1
								pictorByteWrite( HighFCol );
								pictorByteWrite( LowFCol );
							} else {
								pictorByteWrite( HighBCol );
								pictorByteWrite( LowBCol );
							}
						}
						if(!(mask>>=1)){
							mask = 0x80;//extract sequential bits from bytes
							++byteCur;//fetch sequential bytes
						}
					}
					if (J.Y) {
						byteCur = byteStart;//reset read address to start of line
						mask = maskStart;//reset mask to start of line
					}
				}
			}
			break;
		case 1:
			while(I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;
				while(J.Y--) {
					I.X = SpriteReg->Size.X;
					while (I.X--) {
						J.X = Scale;
						while (J.X--) {
							datH = pgm_read_byte(byteCur+1);
							datL = pgm_read_byte(byteCur);
							pictorByteWrite( datH );
							pictorByteWrite( datL );
						}
						byteCur+=2;
					}
				if (J.Y) {byteCur = byteStart;}
				}
			}
			break;
		case 0:
			while(I.Y--) {
				J.Y = Scale;
				byteStart = byteCur;
				while(J.Y--) {
					I.X = SpriteReg->Size.X;
					while (I.X--) {
						J.X = Scale;
						while (J.X--) {
							pictorByteWrite( *(byteCur+1) );
							pictorByteWrite( *byteCur );
						}
						byteCur+=2;
					}
				if (J.Y) {byteCur = byteStart;}
				}
			}
			break;
	}
}

void plot4points(const point Centre, const point Offset, const uint16_t Colour) {
	point A = {Centre.X + Offset.X, Centre.Y + Offset.Y};
	point B = {Centre.X - Offset.X, Centre.Y + Offset.Y};
	point C = {Centre.X + Offset.X, Centre.Y - Offset.Y};
	point D = {Centre.X - Offset.X, Centre.Y - Offset.Y};
	pictorDrawPixel(A, Colour);
	if (Offset.X) {
		pictorDrawPixel(B, Colour);
	}
	if (Offset.Y) {
		pictorDrawPixel(C, Colour);
	}
	if (Offset.X && Offset.Y) {
		pictorDrawPixel(D, Colour);
	}
}

void plot8points(const point Centre, const point Offset, const uint16_t Colour) {
  plot4points(Centre, Offset, Colour);
  if (Offset.X != Offset.Y) plot4points(Centre, (point){Offset.Y,Offset.X}, Colour);
}

void pictorDrawCircle(const point Centre, const uint8_t Radius, const uint16_t Colour) {
	//TODO:	Revise this.
	if (!Radius) {
		pictorDrawPixel(Centre, Colour);
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
void pictorInit(const uint8_t Mode) {
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
	pictorCmdWrite(0x28);
	pictorCmdWrite(0x11);
	pictorCmdWrite(0xCB);
	pictorByteWrite(0x01);
	pictorCmdWrite(0xC0);
	pictorByteWrite(0x26);
	pictorByteWrite(0x08);
	pictorCmdWrite(0xC1);
	pictorByteWrite(0x10);
	pictorCmdWrite(0xC5);
	pictorByteWrite(0x35);
	pictorByteWrite(0x3E);
	pictorCmdWrite(0x36); // RGB
	pictorByteWrite(0x48);
	pictorCmdWrite(0xB0);
	pictorByteWrite(0x4A);
	pictorCmdWrite(0xB1);
	pictorByteWrite(0x00);
	pictorByteWrite(0x1B);
	pictorCmdWrite(0xB6);
	pictorByteWrite(0x0A);
	pictorByteWrite(0x82);
	pictorByteWrite(0x27);
	pictorByteWrite(0x00);
	pictorCmdWrite(0xC7);
	pictorByteWrite(0xB5);
	pictorCmdWrite(0xF6);
	pictorByteWrite(0x01);
	pictorByteWrite(0x00);
	if(Mode & _BV(0)) {//modes with _BV(0) are VSYNC modes
		pictorByteWrite(0x08);
	} else {
		pictorByteWrite(0x00);
	}
	pictorCmdWrite(0xF2);
	pictorByteWrite(0x00);
	pictorCmdWrite(0x26);
	pictorByteWrite(0x01);
	pictorCmdWrite(0x3A);
	pictorByteWrite(0x55);
	uint8_t PosGammaCorrections[15] = {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0x87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00};
	pictorWrite(0xE0, PosGammaCorrections, 15);
	uint8_t NegGammaCorrections[15] = {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F};
	pictorWrite(0xE1, NegGammaCorrections, 15);
	pictorCmdWrite(0x34); // Tearing OFF
	pictorCmdWrite(0xB4); // Inversion OFF
	pictorByteWrite(0x00);
	pictorCmdWrite(0xB7); // Entry Mode Set
	pictorByteWrite(0x07);
	pictorDrawAll(BLACK);
	pictorCmdWrite(0x29);
	_delay_ms(30);
	pictorBacklightState(1);
}

void pictorMemDump(){
	pictorSetRotation(0);
	pictorCanvasSet((point){0,0}, (point){15,127});
	pictorWordArrayWrite(0x2C,(uint16_t*)0,2048);//draw the entire of memory as pixels
	uint16_t i, data;
	const uint8_t* ptr;
	pictorCanvasSet((point){32,0}, (point){160,255});
	pictorCmdWrite(0x2C);
	for(i=0,ptr=0;i<32768;i++,ptr+=2){//draw the entire of program memory as pixels
		data = pgm_read_dword(ptr);
		pictorWordWrite(data);
	}
}
