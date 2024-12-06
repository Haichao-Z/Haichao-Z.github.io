#include <xc.h>
#include "LCD_GFX.h"
#include "ST7735.h"
#include <util/delay.h>
// Idle Blinking Parameters Define
#define EYE_X1 30   // x-axis of left eyeball
#define EYE_X2 93   // x-axis of right eyeball
#define EYE_Y  64   // y-axis of eyeball   
#define EYE_R  15   // radius of left eyeball
#define PUPIL_R 5    // radius of pupil
#define BACKGROUND_COLOR BLUE  

// Idle Expression Parameters Define
#define x_LeftCircle    40  // x-axis of left eyeball
#define y_LeftCircle    70  // y-axis of left eyeball
#define x_RightCircle    130    // x-axis of right eyeball
#define y_RightCircle    70 // y-axis of right eyeball
#define Radius  21  // ridus of eyeball



typedef enum {
    OPEN,
    CLOSING,
    CLOSED,
    OPENING
} EyeState;

void clear_eye_area(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

    LCD_drawBlock(x0-EYE_R, y0+EYE_R, x1+EYE_R, y1-EYE_R, BLUE);

}

void draw_eyes(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t height) {

    clear_eye_area(x0, y0, x1, y1);
    
    LCD_fillEllipse(x0, y0, EYE_R, height, WHITE);
    LCD_fillEllipse(x1, y1, EYE_R, height, WHITE);
    
    if(height > PUPIL_R) {  
        LCD_drawFilledCircle(x0, y0, PUPIL_R, BLACK);
        LCD_drawFilledCircle(x1, y1, PUPIL_R, BLACK);
        
    }
}

void blink_animation(void) {
    static EyeState state = OPEN;
    static uint8_t animation_step = EYE_R;  
    static uint16_t delay_counter = 0;
    
    switch(state) {
        case OPEN:
            
            draw_eyes(EYE_X1, EYE_Y, EYE_X2, EYE_Y, EYE_R);
            
            if(++delay_counter > 1) {
                state = CLOSING;
                delay_counter = 0;
            }
            break;
            
        case CLOSING:
           
            if(animation_step > 0) {
                animation_step--;
                draw_eyes(EYE_X1, EYE_Y, EYE_X2, EYE_Y, animation_step);

            } else {
                state = CLOSED;
                animation_step = 0;
            }
            break;
            
        case CLOSED:
            
            draw_eyes(EYE_X1, EYE_Y, EYE_X2, EYE_Y, 0);
            
            if(++delay_counter > 1) {
                state = OPENING;
                delay_counter = 0;
            }
            break;
            
        case OPENING:
           
            if(animation_step < EYE_R) {
                animation_step++;
                draw_eyes(EYE_X1, EYE_Y, EYE_X2, EYE_Y, animation_step);
                
            } else {
                state = OPEN;
                animation_step = EYE_R;
            }
            break;
    }
}


void main(void) {
    
    lcd_init();
    LCD_setScreen(BACKGROUND_COLOR);
    
//    while(1) {
//        blink_animation();
//        _delay_ms(20);  
//    }
    LCD_drawFilledCircle(x_LeftCircle, y_LeftCircle, Radius, BLACK);
    LCD_drawFilledCircle(x_RightCircle, y_RightCircle, Radius, BLACK);
    LCD_drawBlock(x_LeftCircle, y_LeftCircle+3, x_RightCircle,  y_LeftCircle+5, BLACK);

    return;
}