#include "driver_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "timer_utils.h"

#include "servo.h"
#include "driver_init.h"
#include "defines.h"

//Funções apenas para testes de aplicação
uint32_t lastMillis = 0;
bool bttouch = false;
void test_touch_buttons(void);
void test_delay_tick(void);
void test_move_servo(void);




static const char* TAG = "GPTIMER";

extern volatile uint32_t tickCount; // Declaração variável de contagem de ticks (em milisensgundos)

void app_main(void) {
   
   
    esp_log_level_set(TAG, ESP_LOG_INFO);
    init_timer_1ms(); // Chamada simplificada da função

    


	
	touch_init(); // Inicializa o touch
    init_servo(); // Inicializa o timer e pinos do servo


    while (1) {
        //Executa função de teste
        test_touch_buttons();
        //test_delay_tick();
        //test_move_servo();

    }
}


void test_touch_buttons(void) {
    sweep_buttons();
    vTaskDelay(10 / portTICK_PERIOD_MS);

    if (GetButton(btn_D13)) {
        printf("Botão D13 pressionado\n");
    }
    if (GetButton(btn_D14)) {
        printf("Botão D14 pressionado\n");
    }
}

void test_delay_tick(void) {

    if (TickStampDelta(lastMillis, tickCount) > 1000) {
        lastMillis = tickCount;
        ESP_LOGI(TAG, "Executando a cada 1000 ms");
        
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

void test_move_servo(void) {  // Teste de leitura do botão para movimentar o servo

        sweep_buttons();
        bttouch = GetButton(btn_init);
		if (bttouch == 1)
		{
			moveServo(END_POSITION);
            printf("Alimenta gato\n");
            lastMillis = tickCount;
    
		}else if (TickStampDelta(lastMillis, tickCount) > 1000)
        {
            moveServo(START_POSITION);
            printf("Recarrega ração \n");
        }
        
		vTaskDelay(10 / portTICK_PERIOD_MS);

}
