#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "soc/gpio_num.h"
#include "i2c-lcd.h"

static const char *TAG = "i2c-simple-example"; // tag para função main

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

void app_main(void)
{
   
    ESP_ERROR_CHECK(i2c_master_init()); //Macro que verifica algum erro na chamada da função
    ESP_LOGI(TAG, "I2C initialized successfully");
    
	lcd_init(); //inicia lcd

	lcd_send_string("Prog:"); //envia uma string
	lcd_put_cur(1, 0); // Muda cursor para segunda Linha
    lcd_send_string("1 - 12:00"); //envia uma string
    lcd_put_cur(2, 0); // Muda cursor para terceira Linha
    lcd_send_string("2 - 13:50"); //envia uma string
    lcd_put_cur(3, 0); /// Muda cursor para quarta   Linha
    lcd_send_string("3 - 19:00"); //envia uma string
}
