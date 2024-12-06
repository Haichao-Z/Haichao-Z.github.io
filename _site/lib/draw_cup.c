#include <avr/io.h>
#include "ST7735.h"
#include "LCD_GFX.h"

#define LCD_WIDTH 160
#define LCD_HEIGHT 128

/**************************************************************************//**
* @fn			void draw_trapezoid_cup(uint16_t x, uint16_t y, uint16_t top_width, uint16_t bottom_width, uint16_t height, uint16_t cup_color, uint16_t water_color)
* @brief		Draw a cup of water
* @note         draw_trapezoid_cup(40, 20, 80, 40, 100, BLACK, BLUE);
*****************************************************************************/
void draw_trapezoid_cup(uint16_t x, uint16_t y, uint16_t top_width, uint16_t bottom_width, uint16_t height, uint16_t cup_color, uint16_t water_color) {
    uint16_t top_x = x;
    uint16_t bottom_x = x + (top_width - bottom_width) / 2;

    LCD_drawLine(top_x, y, top_x + top_width, y, cup_color);
    LCD_drawLine(top_x, y, bottom_x, y + height, cup_color);
    LCD_drawLine(top_x + top_width, y, bottom_x + bottom_width, y + height, cup_color);
    LCD_drawLine(bottom_x, y + height, bottom_x + bottom_width, y + height, cup_color);
    

    uint16_t water_height = height / 2;
    uint16_t water_y = y + (height - water_height);

    for (uint16_t i = water_y + water_height - 1; i > water_y; i--) {
        uint16_t current_width = top_width - ((top_width - bottom_width) * (i - y)) / height;
        uint16_t fill_left = top_x + (top_width - current_width) / 2 + 1;
        uint16_t fill_right = fill_left + current_width - 1;
        LCD_drawLine(fill_left, i, fill_right, i, water_color);
    }

}
