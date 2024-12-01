/*
 * system.h
 *
 *  Created on: Nov 29, 2024
 *      Author: ASUS
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "lcd.h"
#include "sensor.h"
#include "button.h"
#include "buzzer.h"
#include "software_timer.h"
#include "my_clock.h"
#include "uart.h"
void system_loop(void);
void store_power_data(float power);
int get_digit_count(int number);
void plot_power_chart();

#endif /* INC_SYSTEM_H_ */
