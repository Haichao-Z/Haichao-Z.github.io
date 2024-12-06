/*
 * File:   TWI.c
 * Author: zhaohaichao
 *
 * Created on November 14, 2024, 4:08 PM
 */


#include <xc.h>
#include <util/twi.h>

// Initialize TWI
void twi_init(void) {
    DDRC &= ~((1<<PC4)|(1<<PC5));
    PORTC |= (1<<PC4)|(1<<PC5);      // Enable pull-ups
    
    // Set SCL frequency to 100kHz
    TWBR0 = 72;  // 16MHz/(16+2*TWBR*1) = 100kHz
    TWSR0 = 0;   // Set prescaler to 1
    
    TWCR0 = (1<<TWEN);               // Enable TWI
}

// Send TWI start signal
void twi_start(void) {
    TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while(!(TWCR0 & (1<<TWINT)));
}

// Send TWI stop signal
void twi_stop(void) {
    TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    while(TWCR0 & (1<<TWSTO));
}

// Write one byte via TWI
void twi_write(uint8_t data) {
    TWDR0 = data;
    TWCR0 = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR0 & (1<<TWINT)));
}

// Read one byte via TWI and send ACK
uint8_t twi_read_ack(void) {
    TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while(!(TWCR0 & (1<<TWINT)));
    return TWDR0;
}

// Read one byte via TWI and send NACK
uint8_t twi_read_nack(void) {
    TWCR0 = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR0 & (1<<TWINT)));
    return TWDR0;
}
