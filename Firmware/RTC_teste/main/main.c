#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <time.h>
#include <sys/time.h>


#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <driver/gpio.h>
#define LED GPIO_NUM_2

struct tm data;//Cria a estrutura que contem as informacoes da data.


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


    time_t tt = time(NULL);//Obtem o tempo atual em segundos.
    data = *gmtime(&tt);//Converte o tempo atual e atribui na estrutura

    
    char data_formatada[64];
    strftime(data_formatada, 64, "%d/%m/%Y %H:%M:%S", &data);//Cria uma String formatada da estrutura "data"

    printf("\nUnix Time: %ld\n", (long)tt); // Mostra na Serial o Unix time.
    printf("Data formatada: %s\n", data_formatada);//Mostra na Serial a data formatada


	//Fica ligando e desligando o led da placa a cada 1 segundo só para ver que o firmware está rodando
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
	
  }

  
}