#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <math.h>
#include "LCD_GFX.h"
#include "TWI.h"

// APDS9960 Register Address Definitions
#define APDS9960_ADDR 0x39
#define APDS9960_ID 0x92
#define APDS9960_STATUS 0x93 
#define APDS9960_GCONF1 0xA2
#define APDS9960_GCONF2 0xA3
#define APDS9960_GCONF3 0xAA
#define APDS9960_GCONF4 0xAB
#define APDS9960_GFLVL 0xAE
#define APDS9960_GSTATUS 0xAF
#define APDS9960_ENABLE 0x80
#define APDS9960_GFIFO_U 0xFC
#define APDS9960_GFIFO_D 0xFD
#define APDS9960_GFIFO_L 0xFE
#define APDS9960_GFIFO_R 0xFF
#define APDS9960_GPULSE  0xA6
#define APDS9960_GPENTH  0xA0
#define APDS9960_GEXTH   0xA1
#define APDS9960_GOFFSET_U  0xA4
#define APDS9960_GOFFSET_D  0xA5
#define APDS9960_GOFFSET_L  0xA6
#define APDS9960_GOFFSET_R  0xA7
#define APDS9960_PDATA 0x9C

// Gesture Direction Definitions
typedef enum {
    DIR_NONE,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
} gesture_dir_t;

// Write to register
void write_reg(uint8_t reg, uint8_t data);

// Read from register
uint8_t read_reg(uint8_t reg);

// Initialize APDS9960
void APDS9960_init(void);

// Read gesture data
gesture_dir_t read_gesture(void);
