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
//low level special case for unpadded number draw
uint8_t pictorDrawD_(const int16_t Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale) {
	char NumberString[7];
	itoa(Number, NumberString, 10);
	return pictorDrawS((unsigned char*)NumberString, Pos, ForegroundColour, BackgroundColour, Font, scale);
}

void pictorDrawD(const int16_t Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale, uint8_t len) {
	if(!len){
		pictorDrawD_(Number, Pos, ForegroundColour, BackgroundColour, Font, scale);
	}
	point Pos2 = Pos;
	char NumberString[7];
	itoa(Number, NumberString, 10);
	uint8_t i = 0;
	while(NumberString[i])i++;
	while(i<len){
	
		pictorDrawC(32, Pos2, ForegroundColour, BackgroundColour, Font, scale);
		Pos2.X += scale*8;
		i++;
	}
	pictorDrawS((unsigned char*)NumberString, Pos2, ForegroundColour, BackgroundColour, Font, scale);
}

uint8_t pictorDrawX(const uint8_t Value, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale) {
	uint8_t c1=Value&0x0F,c2=(Value&0xF0)>>4;//load initial two values(0-15)
	c1=(c1>=10)?(c1+55):(c1+48);
	c2=(c2>=10)?(c2+55):(c2+48);//convert to characters
	pictorDrawC(c2, Pos, ForegroundColour, BackgroundColour, Font, scale);
	pictorDrawC(c1, (point){Pos.X+8,Pos.Y}, ForegroundColour, BackgroundColour, Font, scale);
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

void pictorDrawSprite_(const sprite* Sprite, const point Pos) {
	uint16_t Pixel = 0;
	point I = Sprite->Size;
	pictorCanvasSet(Pos, (point){Pos.X+I.X-1,Pos.Y+I.Y-1});
	Pixel = I.X*I.Y;
	pictorWordArrayWrite(0x2C,(uint16_t*)Sprite->RGB,Pixel);
}

void pictorDrawSprite(const sprite* Sprite, const point Pos, const uint8_t Scale) {
	if (Scale == 1) {
		pictorDrawSprite_(Sprite, Pos);
		return;
	} else if (!Scale){
		return;//if scale is 0 then nothing to draw
	}
	uint8_t k = 2*Sprite->Size.X;
	uint8_t* rgb = (uint8_t*)Sprite->RGB;
	point J, I = Sprite->Size;
	pictorCanvasSet(Pos,(point){(Pos.X + (I.X * Scale)) - 1, (Pos.Y + (I.Y * Scale)) - 1});
	pictorCmdWrite(0x2C);
	while(I.Y--) {
		J.Y = Scale;
		while(J.Y--) {
			I.X = Sprite->Size.X;
			while (I.X--) {
				J.X = Scale;
				while (J.X--) {
					pictorByteWrite( *(rgb+1) );
					pictorByteWrite( *rgb );
				}
				rgb+=2;
			}
		if (J.Y) {rgb -= k;}
		}
	}
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
	pictorByteWrite(0x00);
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