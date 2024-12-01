/*
 * button.h
 *
 *  Created on: Sep 25, 2023
 *      Author: HaHuyen
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "spi.h"
#include "gpio.h"
#include "software_timer.h"
#include "type.h"

#define LONG_PRESSED_TIME   1500
#define READ_BUTTON_TIME    10 // 10ms

extern uint16_t button_count[16];

void button_init();
void button_Scan();
bool is_button_pressed(uint8_t id);
bool is_button_long_pressed(uint8_t id);

#endif /* INC_BUTTON_H_ */
