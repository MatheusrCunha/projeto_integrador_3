#include <stdbool.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "servo.h"
#include "driver_init.h"
#include "defines.h"

void app_main(void) {
	
	bool bttouch = false;
		
	touch_init();
 
    configure_ledc_timer();
    
    configure_ledc_channel();
 
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    while (1) {
		
		bttouch = read_buttons();
		if (bttouch == 1){
			moveServo(152); 
        	vTaskDelay(1000 / portTICK_PERIOD_MS); 
        	moveServo(57); 
		}
		else{
			moveServo(57);
		}
		
		vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
}