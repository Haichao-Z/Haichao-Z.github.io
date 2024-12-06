/*
 * File:   Servo_360.c
 * Author: zhaohaichao
 *
 * Created on November 15, 2024, 11:02 AM
 */


#include <xc.h>
#include <util/delay.h>

#define PWM_500us   32
#define PWM_1500us   91
#define PWM_2500us   156
#define timePerDegree   250/360   // 500ms For 360º

void Timer3_Timer4_init(void);
void rotate_certain_degree_T3(uint16_t degree, int dir);
void rotate_certain_degree_T4(uint16_t degree, int dir);
void delay_1ms(void);