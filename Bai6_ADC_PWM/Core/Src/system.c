/*
 * system.c
 *
 *  Created on: Nov 29, 2024
 *      Author: ASUS
 */

#include "system.h"

int volval = 99; //duty cycle of volume
uint8_t isnotify = 0; //to notify to computer
float power_samples[100] = { 0 }; // Array of power
uint8_t grid_split = 10;
uint16_t time_range = 100;      // OX (100 unit time)
float max_power = 100.0;         // OY (10 mW)

void LCD_show_sensor();

/*
 * @brief:	update sensor value
 * @para:	none
 * @retval:	none
 * */
void sensor_diplay(int is_show_graph) {
	if (!flag_timer(0)) {
		set_timer(0, READ_BUTTON_TIME);
		ds3231_ReadTime();
		button_Scan();
		sensor_Read();
		//Check Potentiometer value and send notify
		if (sensor_GetPotentiometer() >= 4095 * 0.7) {
			if (isnotify == 1) {
				buzzer_SetVolume(volval);
				if (!flag_timer(4)) {
					set_timer(4, ONE_SECOND);
					isnotify = 0;
					uart_Rs232SendString("Potentiometer > 70%\n");
					uart_Rs232SendString("Please reduce Potentiometer\n\n");
				}
			} else if (isnotify == 0) {
				buzzer_SetVolume(0);
				if (!flag_timer(4)) {
					set_timer(4, ONE_SECOND);
					isnotify = 1;
				}
			}
		} else {
			buzzer_SetVolume(0);
		}

		//change volume value
		if (button_count[11] == 1) {
			volval += 10;
			if (volval > 99) {
				volval = 0;
			}
			lcd_ShowIntNum(10, 300, volval, 2, WHITE, BLACK, 16);
		}
		if (button_count[15] == 1) {
			volval -= 10;
			if (volval < 0) {
				volval = 99;
			}
			lcd_ShowIntNum(10, 300, volval, 2, WHITE, BLACK, 16);
		}
	}
	//show sensor value to LCD
	if (!flag_timer(3)) {
		if (is_show_graph == 1) {
			store_power_data(sensor_GetPotentiometer() * 100 / 4095);
			lcd_ShowStr(10, 280, "Power(mW):", WHITE, BLACK, 16, 0);
			lcd_ShowFloatNum(130, 280, sensor_GetCurrent() * sensor_GetVoltage(), 4, WHITE, BLACK, 16);
			plot_power_chart();
		}

		else
			LCD_show_sensor();

		set_timer(3, LCD_SENSOR_TIME);
	}
}

/*
 * @brief:	show sensor value to screen
 * @para:	none
 * @retval:	none
 * */
void LCD_show_sensor() {
	lcd_ShowStr(10, 100, "Voltage(V):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 100, sensor_GetVoltage(), 4, WHITE, BLACK, 16);

	lcd_ShowStr(10, 120, "Current(mA):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 120, sensor_GetCurrent(), 4, WHITE, BLACK, 16);

	lcd_ShowStr(10, 140, "Power(mW):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 140, sensor_GetCurrent() * sensor_GetVoltage(), 4,
	WHITE, BLACK, 16);

	lcd_ShowStr(10, 160, "Light:", WHITE, BLACK, 16, 0);
	if (sensor_GetLight() <= 4095 * 0.75) {
		lcd_ShowStr(60, 160, "(Strong)", WHITE, BLACK, 16, 0);
	} else {
		lcd_ShowStr(60, 160, "(Weak)  ", WHITE, BLACK, 16, 0);
	}
	lcd_ShowIntNum(130, 160, sensor_GetLight(), 4, WHITE, BLACK, 16);

	lcd_ShowStr(10, 180, "Poten(Ohm):", WHITE, BLACK, 16, 0);
	lcd_ShowIntNum(130, 180, sensor_GetPotentiometer() * 100 / 4095, 2, WHITE,
	BLACK, 16);
	lcd_ShowStr(180, 180, "%", WHITE, BLACK, 16, 0);

	lcd_ShowStr(10, 200, "Temp(C):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 200, sensor_GetTemperature(), 4, WHITE, BLACK, 16);
}

void system_loop(void) {
	sensor_diplay(0);
//	fsm_clock();
}

void store_power_data(float power) {
	// Shift all elements one step to the left
	for (int i = 0; i < time_range - 1; i++) {
		power_samples[i] = power_samples[i + 1];
	}
	// Store the new power value at the last position
	power_samples[time_range - 1] = power;
}

int get_digit_count(int number) {
	int count = 0;
	if (number == 0)
		return 1;
	while (number != 0) {
		number /= 10;
		count++;
	}
	return count;
}

void plot_power_chart() {
	lcd_Fill(10, 10, 210, 210, BLACK);

	// Ox
	for (int i = 0; i <= grid_split; i++) {
		int x = 10 + (200 * i / grid_split);
		lcd_DrawLine(x, 10, x, 210, LIGHTGRAY);
	}

	// Oy
	for (int i = 0; i <= grid_split; i++) {
		int y = 210 - (200 * i / grid_split);
		lcd_DrawLine(10, y, 210, y, LIGHTGRAY);
	}

	// Oy labels
	for (int i = 0; i <= grid_split; i++) {
		int label = (int) (i * max_power / grid_split);
		int y = 210 - (200 * i / grid_split);
		int len = get_digit_count(label);
		lcd_ShowIntNum(0, y, label, len, WHITE, BLACK, 16);
	}

	// Ox labels
	for (int i = 0; i <= grid_split; i++) {
		int label = (int) (time_range - (i * time_range / grid_split));
		int x = 10 + (200 * i / grid_split);
		int len = get_digit_count(label);
		lcd_ShowIntNum(x, 215, label, len, WHITE, BLACK, 16);
	}

	// Graph of power
	for (int i = 0; i < time_range - 1; i++) {
		int x1 = 10 + (200 * i / time_range);
		int y1 = 210 - (int) ((200.0 * power_samples[i]) / 100.0);
		int x2 = 10 + (200 * (i + 1) / time_range);
		int y2 = 210 - (int) ((200.0 * power_samples[i + 1]) / 100.0);

		if (y1 < 10)
			y1 = 10;
		if (y1 > 210)
			y1 = 210;
		if (y2 < 10)
			y2 = 10;
		if (y2 > 210)
			y2 = 210;

		lcd_DrawLine(x1, y1, x2, y2, RED);
	}
}

