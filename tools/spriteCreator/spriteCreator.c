#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "lodepng.h"

const char InfoString[] = "Sprite generator from PNG for use with pictor v0.3.3 or later.\nCreated by William Barber. Using LodePNG from Lode Vandevenne.\n";

typedef struct {
	uint16_t colour[16];
	uint8_t n;
}Palette;

uint16_t encode656Colour(const uint8_t Red, const uint8_t Green, const uint8_t Blue);
void spriteGen( uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, uint8_t ProgMem);
int paletteGen(uint8_t* data, uint16_t X, uint16_t Y, Palette* palette);
void spritePacked(uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, Palette* palette, uint8_t ProgMem);
void spriteCompressed( uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, Palette* palette, uint8_t ProgMem);
int spriteOne( uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, uint8_t ProgMem);
void imageErr(uint8_t err);

int main(int argc, char *argv[]) {//<program name> <image file> <sprite file> <sprite name> <type[...]>
	printf("%s\n", InfoString);
	if((argc == 2) && (argv[1][0] == '?')) {//help message on "spriteCreator ?"
		printf("Usage: %s <image input file> <sprite output file> <sprite name>\n\t<sprite type[..]>\n",argv[0]);//Usage message
		printf("Image input file\tThe file-name of the PNG image to be converted into the\n\t\t\tsprite.\n");
		printf("Sprite output file\tThe file-name of the header file that the generated\n\t\t\tsprite is appended to.\n");//Parameter list
		printf("Sprite name\t\tThe name allocated to the generated sprite structure,\n\t\t\tused to address the sprite in pictor.\n");
		printf("Sprite type\t\tThe type of sprite to be generated, as a number\n\t\t\treferring to the available sprite types list.\n");
		printf("\t[...]\t\tOptional additional sprite types to be generated,\n\t\t\tstored in the same header file, the name is appended by\n\t\t\tthe type number.\n\n");
		printf("Available sprite types:\n");//Sprite type list
		printf("\t0 - Uncompressed sprite stored in memory\n");
		printf("\t1 - Uncompressed sprite stored in program memory\n");
		printf("\t2 - 1-bit Colour sprite stored in memory (Black and White images only)\n");
		printf("\t3 - 1-bit Colour sprite stored in program memory (B&W images only)\n");
		printf("\t4 - Colour indexed sprite, palette and pixels stored in memory,\n\t\tthe pixel array is runtime encoded\n");
		printf("\t5 - Colour indexed sprite,\n\t\tpalette stored in memory, pixels stored in program memory,\n\t\tthe pixel array is runtime encoded\n");
		printf("\t6 - Colour indexed sprite,\n\t\tpixels stored in memory, palette stored in program memory,\n\t\tthe pixel array is runtime encoded\n");
		printf("\t7 - Colour indexed sprite, palette and pixels stored in program memory,\n\t\tthe pixel array is runtime encoded\n");
		printf("\t8 - Colour indexed sprite, palette and pixels stored in memory,\n\t\tthe pixel array is packed 2 pixels per byte\n");
		printf("\t9 - Colour indexed sprite,\n\t\tpalette stored in memory, pixels stored in program memory,\n\t\tthe pixel array is packed 2 pixels per byte\n");
		printf("\tA - Colour indexed sprite,\n\t\tpixels stored in memory, palette stored in program memory,\n\t\tthe pixel array is packed 2 pixels per byte\n");
		printf("\tB - Colour indexed sprite, palette and pixels stored in program memory,\n\t\tthe pixel array is packed 2 pixels per byte\n");
		return -3;
	} else if(argc != 5) {//Usage message on insufficient arguments
		printf("Usage: %s <image input file> <sprite output file> <sprite name>\n\t<sprite type[...]>\n",argv[0]);
		printf("e.g. %s image.png sprite.h sprite 4567\n",argv[0]);
		return -2;
	}
	FILE* file;
	if((file = fopen(argv[2],"a")) == NULL) {//open output file, append
		fprintf(stderr,"file failed to open\n");
		return -1;
	}
	int err;
	uint8_t* image;
	uint8_t i,j;
	unsigned int W,H;
	char name[50];
	strcpy(name,argv[3]);
	Palette colourIndex;
	if((err = lodepng_decode24_file(&image/*PNG pixel array*/, &W/*PNG X size*/, &H/*PNG Y size*/, argv[1]/*Image file-name*/)) != 0) {//Extract PNG data and handle errors
		imageErr(err);
		fclose(file);
		return -1;
	}
	for(j = 0; j < strlen(argv[4]); j++) {
		i=100;
		strcpy(name,argv[3]);
		switch(toupper(argv[4][j])) {
			case '0':
				if(j != 0)strcat(name,"0");
				spriteGen( image, W, H, file, name,0);
				printf("Successfully created sprite \"%s\" of type 0\n", name);
				break;
			case '1':
				if(j != 0)strcat(name,"1");
				spriteGen( image, W, H, file, name,1);
				printf("Successfully created sprite \"%s\" of type 1\n", name);
				break;
			case '2':
				if(j != 0)strcat(name,"2");
				if(!spriteOne( image, W, H, file, name,0)) {
					printf("Successfully created sprite \"%s\" of type 2\n", name);
				}
				break;
			case '3':
				if(j != 0)strcat(name,"3");
				if(!spriteOne( image, W, H, file, name,1)) {
					printf("Successfully created sprite \"%s\" of type 3\n", name);
				}
				break;
			case '4':
				if(i==100) {
					i=0;
					if(j != 0)strcat(name,"4");
				}
			case '5':
				if(i==100) {
					i=1;
					if(j != 0)strcat(name,"5");
				}
			case '6':
				if(i==100) {
					i=2;
					if(j != 0)strcat(name,"6");
				}
			case '7':
				if(i==100) {
					i=3; 
					if(j != 0)strcat(name,"7");
				}
				colourIndex.n = 0;
				if(paletteGen( image, W, H, &colourIndex) == -1) {
					fprintf(stderr,"Image contains more than 16 unique colours, so cannot be indexed.\n");
				} else {
					spriteCompressed( image, W, H, file, name, &colourIndex,i);
					printf("Successfully created sprite \"%s\" of type %d\n", name, 4+i);
				}
				break;
			case '8':
				if(i==100) {
					i=0;
					if(j != 0)strcat(name,"8");
				}
			case '9':
				if(i==100) {
					i=1;
					if(j != 0)strcat(name,"9");
				}
			case 'A':
				if(i==100) {
					i=2;
					if(j != 0)strcat(name,"A");
				}
			case 'B':
				if(i==100) {
					i=3; 
					if(j != 0)strcat(name,"B");
				}
				colourIndex.n = 0;
				if(paletteGen( image, W, H, &colourIndex) == -1) {
					fprintf(stderr,"Image contains more than 16 unique colours, so cannot be indexed.\n");
				} else {
					spritePacked( image, W, H, file, name, &colourIndex,i);
					printf("Successfully created sprite \"%s\" of type %d\n", name, 8+i);
				}
				break;
			default:
				printf("'%c' is an invalid sprite type\n",argv[4][j]);
				break;
		}
	}
	free(image);
	fclose(file);
	return 0;
}

uint16_t encode656Colour(const uint8_t Red, const uint8_t Green, const uint8_t Blue) {
	uint16_t Colour = Red & 0xF8;	//	00000000RRRRR000
	Colour <<= 5;					//	000RRRRR00000000
	Colour |= Green & 0xFC;			//	000RRRRRGGGGGG00
	Colour <<= 3;					//	RRRRRGGGGGG00000
	Colour |= Blue >> 3;			//	RRRRRGGGGGGBBBBB
	return Colour;
}

void spriteGen( uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, uint8_t ProgMem) {//sprite type 0 & 1
	uint32_t Size = (uint32_t)X * (uint32_t)Y * 3;
	uint32_t i;
	uint16_t pixel;
	if(ProgMem) {
		fprintf(fd,"static const uint16_t %sRGB[] PROGMEM = {\n", name);//write opening line
	} else {
		fprintf(fd,"uint16_t %sRGB[] = {\n", name);//write opening line
	}
	for(i=0;i<Size;i+=3) {
		pixel = encode656Colour(data[i], data[i+1], data[i+2]);//RGB encode
		fprintf(fd,"0x%04X",pixel);//write pixel to file
		if(i<(Size-3))fputc(',',fd);
		if(((i+3)%(X*3)) == 0)fputc('\n',fd);//scanline wrapping to output
	}
	fprintf(fd,"};\n");
	fprintf(fd,"sprite %s = {{%i, %i}, (uint16_t*) %sRGB};\n", name, X, Y, name);//write structure
}

int paletteGen(uint8_t* data, uint16_t X, uint16_t Y, Palette* palette) {
	uint32_t Size = (uint32_t)X * (uint32_t)Y * 3;
	uint32_t i,j;
	uint16_t pixel;
	for(i=0;i<Size;i+=3) {
		pixel = encode656Colour(data[i], data[i+1], data[i+2]);//RGB encode
		for(j=0;j<(palette->n);j++) {
			if(pixel == (palette->colour[j]))break;
		}
		if(j == (palette->n)) {
			if(j == 16)return -1;//if palette is full return -1
			palette->colour[j] = pixel;
			(palette->n)++;
		}
	}
	return 0;
}

void spritePacked(uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, Palette* palette, uint8_t ProgMem) {//sprite type 8, 9, A & B
	uint32_t Size = (uint32_t)X * (uint32_t)Y * 3;
	uint32_t i;
	uint8_t j,k=0,out=0,R,G,B;
	uint16_t pixel;
	j=16;
	if(ProgMem & 2) {
		fprintf(fd,"static const uint16_t %sPalette[] PROGMEM = {",name);//write opening of palette
	} else {
		fprintf(fd,"uint16_t %sPalette[] = {",name);//write opening of palette
	}
	for(i=0;i<(palette->n);i++) {
		fprintf(fd,"0x%04X",palette->colour[i]);
		if(i<((palette->n)-1))fputc(',',fd);
		j--;
	}
	if(j>1)fputc(',',fd);
	if(j>0) {
		while(--j) {
			fprintf(fd,"0x0000");//print unused palette fields
			if(j>1)fputc(',',fd);
		}
	}
	fprintf(fd,"};\n");
	if(ProgMem & 1){
		fprintf(fd,"static const uint8_t %sData[] PROGMEM = {",name);//write opening of data
	} else {
		fprintf(fd,"uint8_t %sData[] = {",name);//write opening of data
	}
	for(i=0;i<Size;i+=3) {
		R = data[i];
		G = data[i+1];
		B = data[i+2];
		pixel = encode656Colour(data[i], data[i+1], data[i+2]);//RGB encode
		for(j=0;j<(palette->n);j++) {//test against palette
			if(pixel == (palette->colour[j]))break;
		}
		if(k) {//pack output byte
			out |= (j & 0x0F);//put the palette index in the lower half of the byte
			fprintf(fd,"0x%02X",out);//write to file
			if(i<(Size-3))fputc(',',fd);
			out = 0;
		} else {
			out = (j<<4);//put the palette index in the upper half of the byte
		}
		k = !k;
	}
	if((Size & 0x01)) {//odd number of pixels
		fprintf(fd,"0x%02X",out);//write the final byte to file
	}
	fprintf(fd,"};\n");
	fprintf(fd,"spritePalette %s = {{%i, %i}, (uint8_t*) %sData, (uint16_t*) %sPalette};\n", name, X, Y, name, name);//write struct
}

void spriteCompressed( uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, Palette* palette, uint8_t ProgMem) {//sprite type 4, 5, 6 & 7
	uint32_t Size = (uint32_t)X * (uint32_t)Y * 3;
	uint32_t i;
	uint8_t j,out,R,G,B;
	uint16_t pixel;
	j=16;
	if(ProgMem & 2) {
		fprintf(fd,"static const uint16_t %sPalette[] PROGMEM = {",name);//write opening of palette
	} else {
		fprintf(fd,"uint16_t %sPalette[] = {",name);//write opening of palette
	}
	for(i=0;i<(palette->n);i++) {
		fprintf(fd,"0x%04X",palette->colour[i]);
		if(i<((palette->n)-1))fputc(',',fd);
		j--;
	}
	if(j>1)fputc(',',fd);
	if(j>0) {
		while(--j) {
			fprintf(fd,"0x0000");//print unused palette fields
			if(j>1)fputc(',',fd);
		}
	}
	fprintf(fd,"};\n");
	if(ProgMem & 1){
		fprintf(fd,"static const uint8_t %sData[] PROGMEM = {",name);//write opening of data
	} else {
		fprintf(fd,"uint8_t %sData[] = {",name);//write opening of data
	}
	for(i=0;i<Size;i+=3) {
		out =0;
		R = data[i];
		G = data[i+1];
		B = data[i+2];
		pixel = encode656Colour(data[i], data[i+1], data[i+2]);//RGB encode
		for(j=0;j<(palette->n);j++) {//test against palette
			if(pixel == (palette->colour[j]))break;
		}
		out = j<<4;//index colour
		j=0;
		for(;i<Size;i+=3) {//run-time encode
			if((data[i] != R)||(data[i+1] != G)||(data[i+2] != B)) {
				i -= 3;//move data-stream back 1 pixel
				break;
			}
			j++;//while the next pixel is the same colour 
			if(j > 0x0F)break;
		}
		out |= ((--j)&0x0F);
		fprintf(fd,"0x%02X",out);//write to file
		if(i<(Size-3))fputc(',',fd);
	}
	fprintf(fd,"};\n");
	fprintf(fd,"spriteEncoded %s = {{%i, %i}, (uint8_t*) %sData, (uint16_t*) %sPalette};\n", name, X, Y, name, name);//write struct
}

int spriteOne( uint8_t* data, uint16_t X, uint16_t Y, FILE* fd, char* name, uint8_t ProgMem) {//sprite type 2 & 3
	uint32_t Size = (uint32_t)X * (uint32_t)Y * 3;
	uint32_t i;
	uint8_t pixels;
	for(i=0;i<Size;i+=3) {
		if((data[i] != 0x00)&&(data[i] != 0xFF)) {
			fprintf(stderr,"The image is not black and white.\n");//may add offending pixel info
			return -1;
		}
	}
	if(ProgMem) {
		fprintf(fd,"static const uint8_t %sData[] PROGMEM = {", name, X, Y);//write opening line
	} else {
		fprintf(fd,"uint8_t %sData[] = {", name, X, Y);//write opening line
	}
	for(i=0;i<Size;i+=(3*8)) {///TODO tidy this up + prevent overrun on sprites that have a size that isn't a multiple of 8
		pixels = (((data[i%Size])?(0x01<<7):(0))|((data[(i+(3*1))%Size])?(0x01<<6):(0))
					|((data[(i+(3*2))%Size])?(0x01<<5):(0))|((data[(i+(3*3))%Size])?(0x01<<4):(0))
					|((data[(i+(3*4))%Size])?(0x01<<3):(0))|((data[(i+(3*5))%Size])?(0x01<<2):(0))
					|((data[(i+(3*6))%Size])?(0x01<<1):(0))|((data[(i+(3*7))%Size])?(0x01):(0)));
		fprintf(fd,"0x%02X",pixels);//write pixel to file
		if(i<(Size-(3*8)))fputc(',',fd);
	}
	fprintf(fd,"};\n");
	fprintf(fd,"spriteMono %s = {{%i, %i}, (uint8_t*) %sData, 0xFFFF, 0x0000};\n",name, X, Y, name);//write struct
	return 0;
}	

void imageErr(uint8_t err){
	fprintf(stderr,"decoder error %u: %s\n", err, lodepng_error_text(err));
}