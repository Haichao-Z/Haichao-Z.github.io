/*
 * File:   ADC.c
 * Author: 13356
 *
 * Created on 2024?10?26?, ??9:02
 */


#include <xc.h>
#include "ADC.h"

void ADC_init()
{
    ADMUX = (1<<REFS0);  
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);  
}

uint16_t ADC_read(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);  
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));
    
    return ADCW;
}