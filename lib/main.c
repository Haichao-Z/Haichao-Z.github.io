/*
 * File:   main.c
 * Author: zhaohaichao
 *
 * Created on November 14, 2024, 4:55 PM
 */


#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <math.h>
#include <avr/interrupt.h>
#include "LCD_GFX.h"
#include "TWI.h"
#include "Gesture_sensor.h"
#include "ST7735.h"
#include "uart.h"
#include "Servo_360.h"
#include "Buzzer.h"

unsigned int Time=0;
unsigned int Health_Monitor = 0;

// F_CPU = 16MHz, 16 cycles per nop
// 1ms = 16000 clock cycles



void my_delay(int i){
    for(int j=0;j<i;j++)
        delay_1ms();
}

void Health_Monitor_Detect(){
    if(Health_Monitor == 1){
        
        
        LCD_setScreen(WHITE);
        draw_trapezoid_cup(40, 20, 80, 40, 100, BLACK, BLUE);
        buzzer_beep();
        my_delay(1000);
        
        Health_Monitor = 0;
        LCD_setScreen(WHITE);
    }
}

ISR(TIMER2_OVF_vect){
    Time++;
    TCNT2 = 6;
    if(Time == 40000){ //100s
        Health_Monitor = 1;
        Time = 0;        
    }
}

int main(void) {
    gesture_dir_t gesture;
    cli();
    //UART_init(103);
    Timer3_Timer4_init();
    buzzer_init();
    
    lcd_init();
    LCD_setScreen(WHITE);
    
    // Initialize TWI and sensor
    twi_init();
    APDS9960_init();
    sei();
    
    DDRC |= (1<<PC3);   // Simulate Keyboard
    DDRC |= (1<<PC2);   // Simulate Keyboard
    DDRC |= (1<<PC1);   // Simulate Keyboard
    DDRC |= (1<<PC0);   // Simulate Keyboard
    
    while(1) {
        Health_Monitor_Detect();
        
        gesture = read_gesture();
        
        // Execute corresponding action based on detected gesture
        switch (gesture) {
            case DIR_UP: // task manager
                // Handle upward gesture
                PORTC |= (1 << PC3);


                LCD_setScreen(WHITE);
                LCD_drawDownGesture(64, 14, 95, 84, BLACK);

                TCCR3A |= (1 << COM3A1); // Set PWM Channel A As Non-inverting Mode
                TCCR4A |= (1 << COM4A1); // Set PWM Channel B As Non-inverting Mode

                PORTC &= ~(1 << PC3);
                
                rotate_both_T3_T4(90, 0);

                my_delay(2000);

                rotate_both_T3_T4(90, 1);

                LCD_setScreen(WHITE);

                break;
            case DIR_DOWN: // google chrome
                // Handle downward gesture
                PORTC |= (1 << PC1);

                LCD_setScreen(WHITE);
                LCD_drawUpGesture(64, 43, 95, 113, BLACK);

                TCCR3A |= (1 << COM3A1); // Set PWM Channel A As Non-inverting Mode
                TCCR4A |= (1 << COM4A1); // Set PWM Channel B As Non-inverting Mode

                PORTC &= ~(1 << PC1);
                
                rotate_both_T3_T4(180, 0);

                my_delay(2000);

                rotate_both_T3_T4(180, 1);

                LCD_setScreen(WHITE);

                break;
            case DIR_LEFT: // Switch Screen
                // Handle leftward gesture
                PORTC |= (1 << PC2);

                LCD_setScreen(WHITE);
                LCD_drawRightGesture(29, 48, 99, 79, BLACK);

                TCCR3A |= (1 << COM3A1); // Set PWM Channel A As Non-inverting Mode
                TCCR4A |= (1 << COM4A1); // Set PWM Channel B As Non-inverting Mode
                
                PORTC &= ~(1 << PC2);

                rotate_certain_degree_T4(180, 1);

                my_delay(2000);

                rotate_certain_degree_T4(180, 0);

                LCD_setScreen(WHITE);

                break;
            case DIR_RIGHT: // Screen Shot
                // Handle rightward gesture
                PORTC |= (1 << PC0);

                LCD_setScreen(WHITE);
                LCD_drawLeftGesture(59, 48, 129, 79, BLACK);

                TCCR3A |= (1 << COM3A1); // Set PWM Channel A As Non-inverting Mode
                TCCR4A |= (1 << COM4A1); // Set PWM Channel B As Non-inverting Mode
                
                PORTC &= ~(1 << PC0);
                
                rotate_certain_degree_T3(180, 0);

                my_delay(2000);

                rotate_certain_degree_T3(180, 1);


                LCD_setScreen(WHITE);


                break;
            default:

                LCD_drawFilledCircle(40, 70, 20 + 1, BLACK);
                LCD_drawFilledCircle(130, 70, 20 + 1, BLACK);
                LCD_drawBlock(40, 73, 130, 75, BLACK);

                TCCR3A &= ~(1 << COM3A1); // Set PWM Channel A As Non-inverting Mode
                TCCR4A &= ~(1 << COM4A1); // Set PWM Channel B As Non-inverting Mode

                PORTC &= ~(1 << PC0);
                PORTC &= ~(1 << PC1);
                PORTC &= ~(1 << PC2);
                PORTC &= ~(1 << PC3);

                break;
        }
        for(int i=0;i<2000;i++)
            _delay_ms(1);  // Delay to reduce sampling rate
    }
    
    return 0;
}