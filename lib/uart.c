#include "uart.h"
#include <avr/io.h>



void UART_init(int BAUD_PRESCALER)
{

    /*Set baud rate */
    UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
    UBRR0L = (unsigned char)BAUD_PRESCALER;
    //Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 2 stop bits, 8 data bits */
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8 data bits
    UCSR0C |= (1<<USBS0); // 2 stop bits
}

void UART_send(unsigned char data)
{
    // Wait for empty transmit buffer
    while(!(UCSR0A & (1<<UDRE0)));
    // Put data into buffer and send data
    UDR0 = data;

}

void UART_putstring(char* StringPtr)
{
    while(*StringPtr != 0x00)
    {
        UART_send(*StringPtr);
        StringPtr++;
    }
}

void floatToString(float number, char *buffer, int decimalPlaces) { //25.6
    int intPart = (int)number;  // intPart=25
    float fracPart = number - intPart;  // fracPart=0.6
    
    int i = 0;
    do {
        buffer[i++] = intPart % 10 + '0'; // buffer[0] = 5  buffer[1] = 2
        intPart /= 10;  // intPart = 2  inPart = 0
    } while (intPart > 0);
    
    int start = 0;  // start = 0
    int end = i - 1; // end = 1
    while (start < end) {
        char temp = buffer[start];  // temp  =  buffer[0]
        buffer[start] = buffer[end]; // buffer[0] =  buffer[1]
        buffer[end] = temp; // buffer[1] =  temp
        start++; // start = 1
        end--; // end =  0
    }
    
    buffer[i++] = '.'; // buffer[2] = '.'
    
    for (int j = 0; j < decimalPlaces; j++) {
        fracPart *= 10;
        int digit = (int)fracPart;
        buffer[i++] = digit + '0';
        fracPart -= digit;
    }
    
    buffer[i] = '\n';
}