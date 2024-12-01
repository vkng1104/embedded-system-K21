/*
 * software_timer.h
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "tim.h"
#include "led_7seg.h"
#include "type.h"

#define FREQUENCY_OF_TIM	1000    // Hz
#define INCREASE_TIME		200
#define BLINKING_TIME		1000 // blink in 2Hz
#define NUMBER_OF_TIMER 	5
#define READ_BUTTON_TIME	10 //50ms
#define READ_SCREEN_TIME	10
#define SNAKE_MOVE_TIME		500

void set_timer(unsigned i, unsigned int time);
void run_timer(void);
bool flag_timer(unsigned i);
void timer_EnableDelayUs();
void delay_us (uint16_t us);
#endif /* INC_SOFTWARE_TIMER_H_ */
