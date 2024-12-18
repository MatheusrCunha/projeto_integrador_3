/*
 * driver_init.h
 *
 *  Created on: 1 de dez. de 2024
 *      Author: ueslei
 */

#ifndef MAIN_DRIVER_INIT_H_
#define MAIN_DRIVER_INIT_H_

#include <stdbool.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include <unistd.h>

#define TOUCH_PAD_GPIO13_CHANNEL TOUCH_PAD_NUM4

void touch_init();
bool read_buttons();

#endif /* MAIN_DRIVER_INIT_H_ */
