#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <time.h>
#include <sys/time.h>

//Para funcionar a saida SERIAL
#include "driver/uart.h"


#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <driver/gpio.h>
#define LED GPIO_NUM_2

struct tm data;//Cria a estrutura que contem as informacoes da data.


// Configuração da UART
#define BUF_SIZE (1024)

void uart_init() {
    const uart_port_t uart_num = UART_NUM_1;
    const int baud_rate = 115200;

    // Configura a UART
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // Pinos TX e RX
}




void app_main() {
	
	uart_init(); // Inicializa a UART
	
	
    struct timeval tv; // Use 'struct' para definir a estrutura
    tv.tv_sec = 1730501148; // Atribui um valor à tv_sec
    settimeofday(&tv, NULL); // Configura o horário



	gpio_set_direction(LED, GPIO_MODE_OUTPUT); //Habilita pino do LED
	
	
	char var = 0;
	
	int timestamp;
	
  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));//Espera 1 seg


    time_t tt = time(NULL);//Obtem o tempo atual em segundos. Utilize isso sempre que precisar obter o tempo atual
    data = *gmtime(&tt);//Converte o tempo atual e atribui na estrutura

    
    char data_formatada[64];
    strftime(data_formatada, 64, "%d/%m/%Y %H:%M:%S", &data);//Cria uma String formatada da estrutura "data"

    //time_t tt = time(NULL); // Obtem o tempo atual em segundos
    printf("\nUnix Time: %ld\n", (long)tt); // Use %ld para time_t

    //printf("\nUnix Time: %d\n", int32_t(tt));//Mostra na Serial o Unix time
    printf("Data formatada: %s\n", data_formatada);//Mostra na Serial a data formatada


	
	if(var == 0 )
	{
		printf("LED ligado!\n");
	    gpio_set_level(LED, 1);
	    var=1;
	}
	else
	{
		printf("LED Desligado!\n");
		gpio_set_level(LED, 0);
    	var=0;
	}
	
	// Leitura da UART
    uint8_t data[BUF_SIZE];
    int len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 100 / portTICK_PERIOD_MS);
    if (len > 0) {
        // Lê o comando enviado pela serial
        timestamp = (int)data[0]; // Lê o primeiro byte recebido
        tv.tv_sec = timestamp; // Atualiza valor da hora (em Unix) pela serial
        settimeofday(&tv, NULL); // Configura o horário
       // printf("Timestamp atualizado: %ld\n", tv.tv_sec);
    }
	


    /*
      Com o Unix time, podemos facilmente controlar acoes do MCU por data, visto que utilizaremos os segundos
      e sao faceis de usar em IFs

      Voce pode criar uma estrutura com a data desejada e depois converter para segundos (inverso do que foi feito acima)
      caso deseje trabalhar para atuar em certas datas e horarios

      No exemplo abaixo, o MCU ira printar o texto **APENAS** na data e horario (28/02/2019 12:00:05) ate (28/02/2019 12:00:07)
    */
    if (tt >= 1730501208 && tt < 1730501215)//Use sua data atual, em segundos, para testar o acionamento por datas e horarios
    {
      printf("Acionando carga durante 3 segundos...\n");
    }
  }

  
}