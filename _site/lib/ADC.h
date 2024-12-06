
/**************************************************************************//**
* @file        ASCII_LUT.h
* @ingroup 	   Adafruit358_LCD
* @brief       ASCII LCD LUT
*
* @details     Lookup Table for ASCII characters to be drawn on LCD
*

*
* @copyright
* @author	   Justin Ye
* @date        April 19, 2021
* @version		0.1
*****************************************************************************/

#include <avr/io.h>

#ifndef ADC_H
#define ADC_H

void ADC_init();
uint16_t ADC_read(uint8_t channel);

#endif /* ADC_H_ */