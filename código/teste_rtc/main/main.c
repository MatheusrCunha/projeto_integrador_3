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

struct tm data;


void app_main() {
    struct timeval tv; 
    tv.tv_sec = 1730501148; 
    settimeofday(&tv, NULL); // Configura o horário

	gpio_set_direction(LED, GPIO_MODE_OUTPUT);
	char var = 0;

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));


    time_t tt = time(NULL);
    data = *gmtime(&tt);

    
    char data_formatada[64];
    strftime(data_formatada, 64, "%d/%m/%Y %H:%M:%S", &data);//Cria uma String formatada da estrutura "data"

    //time_t tt = time(NULL); // Obtem o tempo atual em segundos
    printf("\nUnix Time: %ld\n", (long)tt); // Use %ld para time_t

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
	
    if (tt >= 1730501208 && tt < 1730501215)
    {
      printf("Acionando carga durante 3 segundos...\n");
    }
  }

  
}