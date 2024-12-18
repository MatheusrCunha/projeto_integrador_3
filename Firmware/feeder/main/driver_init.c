/*
 * driver_init.c
 *
 *  Created on: 1 de dez. de 2024
 *      Author: Ueslei
 */

#include "driver_init.h"
#include <stdbool.h>

/**
 * Inicializa os pinos dos botões touch
 *
 */
 
bool btteste = false;
uint16_t val;
uint16_t filtered_value = 0;
uint16_t raw_value_touch = 0;

void touch_init(){
    touch_pad_init();
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    touch_pad_config(TOUCH_PAD_GPIO13_CHANNEL, -1);
    touch_pad_filter_start(10);
}

bool read_buttons(){
	
	
    touch_pad_read_raw_data(TOUCH_PAD_GPIO13_CHANNEL, &raw_value_touch);
    touch_pad_read_filtered(TOUCH_PAD_GPIO13_CHANNEL, &filtered_value);
    touch_pad_read(TOUCH_PAD_GPIO13_CHANNEL, &val);
    
    printf("val_touch_gpio13 = %d raw_value = %d filtered_value = %d\n", val, raw_value_touch, filtered_value);
    if (filtered_value < 500){
		
		btteste = true;
		
	}else {
		btteste = false;
	}
	
	return btteste;
}
