#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <math.h>
#include "LCD_GFX.h"
#include "TWI.h"
#include "Gesture_sensor.h"
#include "ST7735.h"
#include "uart.h"

int flag = 1;
// Write to register
void write_reg(uint8_t reg, uint8_t data) {
    twi_start();
    twi_write((APDS9960_ADDR << 1) | 0);  // Write address
    twi_write(reg);
    twi_write(data);
    twi_stop();
}

// Read from register
uint8_t read_reg(uint8_t reg) {
    uint8_t data;
    
    twi_start();
    twi_write((APDS9960_ADDR << 1) | 0);  // Write address
    twi_write(reg);
    twi_start();  // Repeated start
    twi_write((APDS9960_ADDR << 1) | 1);  // Read address
    data = twi_read_nack();
    twi_stop();
    
    return data;
}

// Initialize APDS9960
void APDS9960_init(void) {
    // Disable all functions
    write_reg(APDS9960_ENABLE, 0x00);
    _delay_ms(10);

    // Configure gesture settings
    write_reg(APDS9960_GPENTH, 0x28);    // Enter gesture threshold
    write_reg(APDS9960_GEXTH, 0x20);     // Exit gesture threshold
    
    write_reg(APDS9960_GCONF1, 0x40);    // 4 gesture events for valid gesture
    write_reg(APDS9960_GCONF2, 0xC8);    // Gesture gain 8x, LED drive 200mA
    write_reg(APDS9960_GCONF3, 0x00);    // All photodiodes active during gesture
    write_reg(APDS9960_GCONF4, 0x02);    // Gesture interrupt enable
    
    write_reg(APDS9960_GOFFSET_U, 0x00); // No offset for UP direction
    write_reg(APDS9960_GOFFSET_D, 0x00); // No offset for DOWN direction
    write_reg(APDS9960_GOFFSET_L, 0x00); // No offset for LEFT direction
    write_reg(APDS9960_GOFFSET_R, 0x00); // No offset for RIGHT direction
    
    write_reg(APDS9960_GPULSE, 0xC9);    // 8 pulses, 16us
    // Enable gesture engine and power
    write_reg(APDS9960_ENABLE, 0x45);    // Power ON and Enable gesture
    _delay_ms(10);
}

// Read gesture data
gesture_dir_t read_gesture(void) {
    int8_t fifo_level;
    int16_t up_down_diff, left_right_diff, PDATA;
    int8_t up, down, left, right;
    gesture_dir_t gesture = DIR_NONE;
    char buffer[20];
    
    int16_t up_sum = 0;
    int16_t down_sum = 0;
    int16_t left_sum = 0;
    int16_t right_sum = 0;
    int16_t temp = 0;
    fifo_level = read_reg(APDS9960_GFLVL);
    
//    PDATA = read_reg(APDS9960_PDATA);
//    floatToString(1.0*PDATA, buffer, 1);
//    UART_putstring("PDATA::");
//    UART_putstring(buffer); 
    
    if (flag){
        flag = 0;
        temp = read_reg(APDS9960_GFIFO_U);
        
        floatToString(1.0*temp, buffer, 3);
        UART_putstring("up:");
        UART_putstring(buffer);
        
        temp = read_reg(APDS9960_GFIFO_D);
        
        floatToString(1.0*temp, buffer, 3);
        UART_putstring("down:");
        UART_putstring(buffer);
        
        temp = read_reg(APDS9960_GFIFO_L);
        
        floatToString(1.0*temp, buffer, 3);
        UART_putstring("left:");
        UART_putstring(buffer);
        
        temp = read_reg(APDS9960_GFIFO_R);
        
        floatToString(1.0*temp, buffer, 3);
        UART_putstring("right:");
        UART_putstring(buffer);
    }
    // Only read the first dataset
    if(fifo_level == 32) {
        
        // stop reading 4-byte datasets until fifo_level = 0
        for(int i = 0; i < 8; i++){
            up_sum += read_reg(APDS9960_GFIFO_U);
            down_sum += read_reg(APDS9960_GFIFO_D);
            left_sum += read_reg(APDS9960_GFIFO_L);
            right_sum += read_reg(APDS9960_GFIFO_R);
        } 
        
        for(int i=0; i<24; i++){
            temp = read_reg(APDS9960_GFIFO_U);
            temp = read_reg(APDS9960_GFIFO_D);
            temp = read_reg(APDS9960_GFIFO_L);
            temp = read_reg(APDS9960_GFIFO_R);
        }
        
        
        floatToString(1.0*up_sum, buffer, 3);
        UART_putstring("up:");
        UART_putstring(buffer);
        
        floatToString(1.0*down_sum, buffer, 3);
        UART_putstring("down:");
        UART_putstring(buffer);
        
        floatToString(1.0*right_sum, buffer, 3);
        UART_putstring("left:");
        UART_putstring(buffer);
        
        floatToString(1.0*left_sum, buffer, 3);
        UART_putstring("right:");
        UART_putstring(buffer);
        UART_putstring("\n");
        
        up = up_sum >> 4;
        down = down_sum >> 4;
        right = right_sum >> 4;
        left = left_sum >> 4;
        
        
        up_down_diff = up - down;
        left_right_diff = left - right;
        
        
//        floatToString(1.0*up, buffer, 1);
//        UART_putstring("up:");
//        UART_putstring(buffer); 
//        
//        floatToString(1.0*down, buffer, 1);
//        UART_putstring("down:");
//        UART_putstring(buffer); 
//        
//        floatToString(1.0*left, buffer, 1);
//        UART_putstring("left:");
//        UART_putstring(buffer); 
//        
//        floatToString(1.0*right, buffer, 1);
//        UART_putstring("right:");
//        UART_putstring(buffer); 
        
        
        
        if(abs(up_down_diff) > abs(left_right_diff)) {
            if(up > down)
                gesture = DIR_UP;
            else if(up < down)
                gesture = DIR_DOWN;
        }
        else {
            if(left > right)
                gesture = DIR_LEFT;
            else if(left < right)
                gesture = DIR_RIGHT;
        }
    }
    
    return gesture;
}