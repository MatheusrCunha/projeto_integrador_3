#include "driver_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "timer_utils.h"
#include "nvs_flash.h"

//#define DEBUG 0

#include "servo.h"
#include "defines.h"
#include "rtc.h"
#include "wifi.h"
#include "i2c-lcd.h"
#include "stateMachine.h"

void test_state_machine(void);
static const char* TAG = "GPTIMER";

// Função para varredura dos botões
void task_sweep_buttons(void *pvParameters) {
    while (1) {
        sweep_buttons();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// Função para a máquina de estados
void task_sweep_stateMachine(void *pvParameters) {
    while (1) {
        sweep_stateMachine();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// Função para a checagem do alarme
void task_check_alarm(void *pvParameters) {
    int num_doses=0; 

    while (1) {
        num_doses = check_alarm();
        DEBUG_PRINT(("Num doses: %d\n",num_doses));
        alimenta_gato(num_doses); // Precisa chamar a função para alimentar mais que uma dose 
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
        ESP_LOGI(TAG, "HighWaterMark: %d", highWaterMark);

    }
}



void app_main(void) {
    esp_log_level_set(TAG, ESP_LOG_INFO);
    init_timer_1ms();
    init_touch();
    init_servo();

    init_rtc();
    print_date();
    wifi_init();

    ESP_ERROR_CHECK(i2c_master_init());
    print_date();

    lcd_init();

    //test_state_machine();

    // Criação das tarefas FreeRTOS
    xTaskCreate(&task_sweep_buttons, "task_sweep_buttons", 2048, NULL, 5, NULL);
    xTaskCreate(&task_sweep_stateMachine, "task_sweep_stateMachine", 4096, NULL, 5, NULL);
    xTaskCreate(&task_check_alarm, "task_check_alarm", 4096, NULL, 5, NULL);

}

void test_state_machine(void) {
    // init_rtc();
    // print_date();
    // wifi_init();

    // ESP_ERROR_CHECK(i2c_master_init());
    // print_date();

    // lcd_init();


    DEBUG_PRINT(("MODO DEBUG está ativo!!!.\n"));

    for (size_t i = 0; i < 8; i++) {
        DEBUG_PRINT(("Retorno de inserção alarme: %s\n", insert_alarm( get_time() + (20*i),2)));
    }

    time_t alarm_list[MAX_ALARMS];
    int alarm_count = get_all_alarms(alarm_list);

    if (alarm_count == 0) {
        printf("Nenhum alarme programado.\n");
    } else {
        printf("Lista de alarmes programados:\n");
        for (int i = 0; i < alarm_count; i++) {
            struct tm *alarm_time = localtime(&alarm_list[i]);
            printf("Alarme %d: %02d:%02d:%02d\n",
                   i + 1,
                   alarm_time->tm_hour,
                   alarm_time->tm_min,
                   alarm_time->tm_sec);
        }
    }
}
