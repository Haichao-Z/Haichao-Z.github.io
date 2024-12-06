/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 

#include "LCD_GFX.h"
#include "ST7735.h"
#include <math.h>

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++){
				if (((pixels>>j)&1)==1){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
**********************************	*******************************************/
void Bre_Circle(uint8_t x0, uint8_t y0, int x, int y, uint16_t color) {
	LCD_drawPixel(x0 + x, y0 + y, color);
	LCD_drawPixel(x0 - x, y0 + y, color);
    LCD_drawPixel(x0 + x, y0 - y, color);
    LCD_drawPixel(x0 - x, y0 - y, color);
    LCD_drawPixel(x0 + y, y0 + x, color);
    LCD_drawPixel(x0 - y, y0 + x, color);
    LCD_drawPixel(x0 + y, y0 - x, color);
    LCD_drawPixel(x0 - y, y0 - x, color);

}

void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
	//use Bresenham's circle algorithm

	int x = 0; 
	int y = radius;
	int d = (3 - 2 * radius);
	Bre_Circle(x0, y0, x, y, color);
	
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        
        Bre_Circle(x0, y0, x, y, color);
    }
}

void LCD_drawFilledCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color) {
//    int x = 0;
//    int y = radius;
//    int d = 3 - 2 * radius;
//    
//    while(y >= x) {
//        // ????
//        for(int i = x0-x; i <= x0+x; i++) {
//            LCD_drawPixel(i, y0+y, color); // ??
//            LCD_drawPixel(i, y0-y, color); // ??
//        }
//        for(int i = x0-y; i <= x0+y; i++) {
//            LCD_drawPixel(i, y0+x, color); // ???
//            LCD_drawPixel(i, y0-x, color); // ???
//        }
//        
//        x++;
//        if(d > 0) {
//            y--;
//            d = d + 4 * (x - y) + 10;
//        } else {
//            d = d + 4 * x + 6;
//        }
//    }
    
    int16_t x, y, i;
    int32_t r2 = radius * radius;
    int32_t x2;
    
    int32_t num_pixels = 0;
    
    for (x = -radius; x < radius; x++) {
        x2 = x * x;
        y = round(sqrt((float)r2 - (float)x2));
        LCD_setAddr(x0 + x, y0 - y, x0 + x, y0 + y);
        SPI_ControllerTx_16bit_S(color, 2*y);
        LCD_setAddr(x0 - x, y0 - y, x0 - x, y0 + y);
        SPI_ControllerTx_16bit_S(color, 2*y);
    }
}

//void LCD_fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color) {
//    int16_t x, y;
//    int32_t rx2 = rx * rx;
//    
//    for (x = 0; x <= rx; x++) {
//        y = round(ry * sqrt(1 - ((float)x*x/rx2)));
//        LCD_drawLine(x0 + x, y0 - y, x0 + x, y0 + y, color);
//        LCD_drawLine(x0 - x, y0 - y, x0 - x, y0 + y, color);
//    }
//}

void LCD_fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color) {
    int16_t x, y, i;
    int32_t rx2 = rx * rx;
    int32_t num_pixels = 0;
    
    for (x = -rx; x < rx; x++) {
        y = round(ry * sqrt(1 - ((float)x*x/rx2)));
        LCD_setAddr(x0 + x, y0 - y, x0 + x, y0 + y);
        SPI_ControllerTx_16bit_S(color, 2*y);
        LCD_setAddr(x0 - x, y0 - y, x0 - x, y0 + y);
        SPI_ControllerTx_16bit_S(color, 2*y);
    }
}
/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
{
    short dx = abs(x1 - x0);  
    short dy = abs(y1 - y0);  
    short direction_x = (x0 < x1) ? 1 : -1;  
    short direction_y = (y0 < y1) ? 1 : -1; 
    short err_1 = dx - dy;  
    
    while (1) {
        SPI_ControllerTx_16bit_NoBE(x0,y0,c);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        short err_2 = 2 * err_1;
        if (err_2 > -dy) {  
            err_1 -= dy;
            x0 += direction_x;
        }
        if (err_2 < dx) { 
            err_1 += dx;
            y0 += direction_y;
        }
    }
        set(LCD_PORT, LCD_TFT_CS); //set CS to high
}



/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
    uint16_t i, numPixels;
    
    
    uint8_t old_x0 = x0;
    uint8_t old_x1 = x1;
    uint8_t old_y0 = y0;
    uint8_t old_y1 = y1;
    
    
    if (x0 > x1) {
        x1 = old_x0;
        x0 = old_x1;
    }
    
    if (y0 > y1) {
        y1 = old_y0;
        y0 = old_y1;
    }
    
    LCD_setAddr(x0, y0, x1, y1);
    
    numPixels = (x1 - x0 + 1) * (y1 - y0 + 1);
    SPI_ControllerTx_16bit_S(color, numPixels);
}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{
	// Fill this out
    
    
    
    
    
    LCD_drawBlock(0, 0, LCD_WIDTH, LCD_HEIGHT, color);


}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
	// Fill this out
    while (*str) {
        if (x + 5 <= LCD_WIDTH && y + 8 <= LCD_HEIGHT) {
            LCD_drawChar(x, y, *str, fg, bg);
            x += 6;  // 5 pixels for the character width + 1 pixel space
            if (x + 5 > LCD_WIDTH) {
                x = 0;  // Move to the start of the next line
                y += 9;  // 8 pixels for the character height + 1 pixel space
            }
        } else {
            break;  // Stop if we've reached the edge of the screen
        }
        str++;
    }
}

/**************************************************************************//**
* @fn			void LCD_drawRightGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw the Right direction of gesture
* @note         LCD_drawRightGesture(29,48,99,79,BLACK);
*****************************************************************************/
void LCD_drawRightGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
    LCD_drawBlock(x0,y0,x1,y1,color);
    for(int i=0;i<30;i++){
    LCD_setAddr(x1+i,y0-15+i,x1+i,y1+15-i);
        for(int j = 0; j< (y1-y0+30-2*i);j++){
            clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
            uint8_t temp = color >> 8;
            SPDR0 = temp;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission
            SPDR0 = color;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission           
        }
    }
    set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}

/**************************************************************************//**
* @fn			void LCD_drawLeftGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw the Left direction of gesture
* @note         LCD_drawLeftGesture(59,48,129,79,BLACK);
*****************************************************************************/
void LCD_drawLeftGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
    LCD_drawBlock(x0,y0,x1,y1,color);
    for(int i=0;i<30;i++){
    LCD_setAddr(x0-i,y0-15+i,x0-i,y1+15-i);
        for(int j = 0; j< (y1-y0+30-2*i);j++){
            clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
            uint8_t temp = color >> 8;
            SPDR0 = temp;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission
            SPDR0 = color;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission           
        }
    }
    set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}

/**************************************************************************//**
* @fn			void LCD_drawUpGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw the Up direction of gesture
* @note         LCD_drawUpGesture(64,43,95,113,BLACK);
*****************************************************************************/
void LCD_drawUpGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
    LCD_drawBlock(x0,y0,x1,y1,color);
    for(int i=0;i<30;i++){
    LCD_setAddr(x0-15+i,y0-i,x1+15-i,y0-i);
        for(int j = 0; j< (x1-x0+30-2*i);j++){
            clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
            uint8_t temp = color >> 8;
            SPDR0 = temp;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission
            SPDR0 = color;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission           
        }
    }
    set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}

/**************************************************************************//**
* @fn			void LCD_drawDownGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw the Down direction of gesture
* @note         LCD_drawDownGesture(64,14,95,84,BLACK);
*****************************************************************************/
void LCD_drawDownGesture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
    LCD_drawBlock(x0,y0,x1,y1,color);
    for(int i=0;i<30;i++){
    LCD_setAddr(x0-15+i,y1+i,x1+15-i,y1+i);
        for(int j = 0; j< (x1-x0+30-2*i);j++){
            clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
            uint8_t temp = color >> 8;
            SPDR0 = temp;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission
            SPDR0 = color;		//Place data to be sent on registers
            while(!(SPSR0 & (1<<SPIF)));	//wait for end of transmission           
        }
    }
    set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}
