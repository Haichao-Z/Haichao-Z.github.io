/*
 * File:   Buzzer.c
 * Author: 13356
 *
 * Created on 2024?11?19?, ??4:09
 */


#include <xc.h>
#include <util/delay.h>


void buzzer_init(){
    DDRD |=(1<<PD3);
    
    TIMSK2 |= (1 << TOIE2);    
    TCCR2A |= (1 << WGM21); 
    TCCR2A |= (1 << WGM20);
    
    TCCR2B |= (1 << CS22); // 64 prescaler  64*256/16M = 1.024ms
    TCCR2A |= (1<<COM2B1);
    OCR2B =255; //Larger -> Quiter
    TCNT2 = 6;
}

void buzzer_beep(){
    TCCR2A |= (1<<COM2B1);
    OCR2B =200;
    _delay_ms(1000);
    TCCR2A &= ~(1<<COM2B1);
}

//int main(){
//
//    buzzer_init();
//    buzzer_beep();
//    
//}