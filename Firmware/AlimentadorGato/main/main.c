#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "servo.h"

void app_main(void) {
 
    configure_ledc_timer();
    
    configure_ledc_channel();
    
    validaServo();
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
     
    while (1) {
		if (gpio_get_level(BT) == 1){
			moveServo(90); 
        	vTaskDelay(1000 / portTICK_PERIOD_MS); 
        	moveServo(180); 
        	vTaskDelay(1000 / portTICK_PERIOD_MS); 
		}
		else{
			moveServo(90);
		}
    }
}