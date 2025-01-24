#include "driver_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "timer_utils.h"

#include "servo.h"
#include "driver_init.h"
#include "defines.h"
#include "rtc.h"

//Funções apenas para testes de aplicação
uint32_t lastMillis = 0;
bool bttouch = false;
void test_touch_buttons(void);
void test_delay_tick(void);
void test_move_servo(void);
void test_servo_and_rtc(void);
void test_alarme(void);




static const char* TAG = "GPTIMER";

extern volatile uint32_t tickCount; // Declaração variável de contagem de ticks (em milisensgundos)

void app_main(void) {
   
   
    esp_log_level_set(TAG, ESP_LOG_INFO);
    init_timer_1ms(); // Chamada simplificada da função

    


	
	touch_init(); // Inicializa o touch
    init_servo(); // Inicializa o timer e pinos do servo


    //test_servo_and_rtc();
    test_alarme();

    while (1) {
        //Executa função de teste
        test_touch_buttons();
        //test_delay_tick();
        //test_move_servo();

    }
}

void test_alarme(void){

    init_rtc();
    print_date();

    for (size_t i = 0; i < 25; i++)
    {
        printf("Retorno de inserção alarme: %s\n", insert_alarm(get_time() + (60*i)));
    }
    
    time_t alarm_list[MAX_ALARMS];
    int alarm_count = get_all_alarms(alarm_list);


    if (alarm_count == 0) {
        //TODO  //Responde para o aplicativo "Nenhum alarme programado.\n"
        printf("Nenhum alarme programado.\n");
    } else {
        //TODO // //Responde para o aplicativo "Lista de alarmes programados:\n"
        printf("Lista de alarmes programados:\n");
        for (int i = 0; i < alarm_count; i++) {
            struct tm *alarm_time = localtime(&alarm_list[i]);
            printf("Alarme %d: %02d:%02d\n", 
                   i + 1, 
                   alarm_time->tm_hour, 
                   alarm_time->tm_min);
        }
    }
    

    while (1) {
        print_date();
        check_alarm();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


}


void test_servo_and_rtc(void){

    init_rtc();

    while (1) {
        print_date();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
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
