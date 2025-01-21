/*
 * driver_init.h
 *
 *  Created on: 1 de dez. de 2024
 *      Author: ueslei
 */

#ifndef MAIN_DRIVER_INIT_H_
#define MAIN_DRIVER_INIT_H_

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include <unistd.h>

#include "defines.h"
#include "timer_utils.h"

//GPIO recomendadas para o touch
#define TOUCH_1 TOUCH_PAD_NUM1 //GPIO0
#define TOUCH_2 TOUCH_PAD_NUM2 //GPIO2
#define TOUCH_3 TOUCH_PAD_NUM3 //GPIO15
#define TOUCH_4 TOUCH_PAD_NUM4 //GPIO13



#define DebounceTicks	5
#define NUM_BTNS		2

void touch_init();

/**
 * Lê o estado do botão touch.
 * 
 * @param __numTecla Índice do botão no array Num_touch (0 a NUM_BTNS-1).
 * @return true se o botão estiver pressionado, false caso contrário.
 */
bool read_buttons(unsigned char __numTecla);


void sweep_buttons(void);
bool GetButton(unsigned char __tecla);



#endif /* MAIN_DRIVER_INIT_H_ */
