/*
 * File:   TWI.c
 * Author: zhaohaichao
 *
 * Created on November 14, 2024, 4:08 PM
 */


#include <xc.h>
#include <util/twi.h>

// Initialize TWI
void twi_init(void);

// Send TWI start signal
void twi_start(void);

// Send TWI stop signal
void twi_stop(void);

// Write one byte via TWI
void twi_write(uint8_t data);

// Read one byte via TWI and send ACK
uint8_t twi_read_ack(void);

// Read one byte via TWI and send NACK
uint8_t twi_read_nack(void);
