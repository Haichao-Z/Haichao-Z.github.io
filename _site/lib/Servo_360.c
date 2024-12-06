/*
 * File:   Servo_360.c
 * Author: zhaohaichao
 *
 * Created on November 15, 2024, 11:02 AM
 */


#include <xc.h>
#include <util/delay.h>
#include "Servo_360.h"

//#define PWM_500us   32
//#define PWM_1500us   91
//#define PWM_2500us   156
//#define timePerDegree   250.0/360   // 500ms For 360º

void delay_1ms(void) {
   for(uint16_t i=0; i<5333; i++) {
       __asm__ __volatile__ ("nop");
       __asm__ __volatile__ ("nop");
       __asm__ __volatile__ ("nop");
   }
}

void Timer3_Timer4_init(){

    DDRD |= (1<<PD0);   // Set PD0 (OC3A) As Output Pin
    DDRD |= (1<<PD1);   // Set PD1 (OC4A) As Output Pin
    
    TCCR3A |= (1<<COM3A1); // Set PWM Channel A As Non-inverting Mode
    TCCR4A |= (1<<COM4A1); // Set PWM Channel B As Non-inverting Mode
    
    TCCR3A |= (1<<WGM31);
    TCCR3B |= ((1<<WGM33)|(1<<WGM32));  // Set Top As ICR3
    
    TCCR4A |= (1<<WGM41);
    TCCR4B |= ((1<<WGM43)|(1<<WGM42));  // Set Top As ICR4
    
    TCCR3B |= (1<<CS32);  // 256 Prescaler
    TCCR4B |= (1<<CS42);  // 256 Prescaler
    
    
    ICR3 = 1250; // Set Frequency As 50Hz
    ICR4 = 1250; // Set Frequency As 50Hz
    
    OCR3A = PWM_1500us; // Set Duty Cycle For CW
    OCR4A = PWM_1500us; // Set Duty Cycle For CW

}

void rotate_certain_degree_T3(uint16_t degree, int dir){
    
    double delay_time;
    
    delay_time = degree * timePerDegree;
    
    if(dir==0)
        OCR3A = PWM_2500us;
    else
        OCR3A = PWM_500us;
    
    for(int i=0; i<delay_time; i++)
        delay_1ms();
    
    OCR3A = PWM_1500us; // Set Duty Cycle For STOP
}

void rotate_certain_degree_T4(uint16_t degree, int dir){
    
    double delay_time;
    
    delay_time = degree * timePerDegree;
    
    if(dir==0)
        OCR4A = PWM_2500us;
    else
        OCR4A = PWM_500us;
    
    for(int i=0; i<delay_time; i++)
        delay_1ms();
    
    OCR4A = 1250; // Set Duty Cycle For STOP
}

void rotate_both_T3_T4(uint16_t degree, int dir){
    
    double delay_time;
    
    delay_time = degree * timePerDegree;
    
    if(dir==0){
        OCR3A = PWM_2500us;
        OCR4A = PWM_500us;
    }else{
        OCR3A = PWM_500us;
        OCR4A = PWM_2500us;
    }
    
    for(int i=0; i<delay_time; i++)
        delay_1ms();
        
    OCR3A = 1250;
    OCR4A = 1250;
}

//int main(){
//    Timer3_Timer4_init();
//    
////    rotate_certain_degree_T4(90,1);
////    _delay_ms(1000);
////    rotate_certain_degree_T4(90,0);
//    
//    //rotate_certain_degree_T4(270,0);
//   
//}