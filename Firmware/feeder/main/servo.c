/*
 * servo.c
 *
 *  Created on: Dec 2, 2024
 *      Author: mathe
 */
#include "servo.h"
#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"


int ServoAngle(int angle) {
    const int minDuty = (int)(0.023 * 65536);
    const int maxDuty = (int)(0.124 * 65536);
    return minDuty + (angle * (maxDuty - minDuty) / 180);
}

void moveServo(int angle) {
    int dutyCycle = ServoAngle(angle);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, dutyCycle);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void configure_ledc_timer(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&ledc_timer);
}

void configure_ledc_channel(void) {
    ledc_channel_config_t ledc_channel = {
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
}

void init_servo(void){
    configure_ledc_timer();
	configure_ledc_channel();
}







 // Funções não usadas por enquanto
void initServo() {
	gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

}

void validaServo() {

    for (int angle = 0; angle <= 45; angle += 10) { 
        moveServo(angle);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
    for (int angle = 45; angle >= 0; angle -= 10) { 
        moveServo(angle);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }

}



