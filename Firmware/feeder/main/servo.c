/*
 * servo.c
 *
 *  Created on: Dec 2, 2024
 *      Author: mathe
 */
#include "servo.h"
#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"
#include "defines.h"
#include "rtc.h"
#include "timer_utils.h"




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


extern volatile uint32_t tickCount; // Declaração variável de contagem de ticks (em milissegundos)
uint32_t lastMillis = 0; //Variavel para salvar o ultimo milisegundo
int qtd_doses=0;
bool wait=false;
void alimenta_gato(int _doses){

    qtd_doses= qtd_doses + _doses; //Vai somando caso não termine a dose antes de ser chamada a função novamente
    DEBUG_PRINT(("Funcao alimenta_gato: %dX\n",qtd_doses));
    if(((qtd_doses > 0 &&  wait == false)) ){
        moveServo(END_POSITION);
        DEBUG_PRINT(("Alimentamdo gato: %dX\n",qtd_doses));
        print_date();
        wait = true; // Variavel para aguardar a execução do movimento do servo
        lastMillis = tickCount;
    } else if (TickStampDelta(lastMillis, tickCount) > 1000) {
        moveServo(START_POSITION);
        if(wait==true){
            qtd_doses--; //Decrementa para contar que fez a alimentação de uma dose
            wait = false;
        }
        DEBUG_PRINT(("Recarregando dose \n"));
        print_date();

    }
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



