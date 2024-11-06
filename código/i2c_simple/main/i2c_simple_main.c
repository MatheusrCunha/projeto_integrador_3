#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "hal/i2c_types.h"
#include "soc/gpio_num.h"
#include "i2c-lcd.h"

static const char *TAG = "i2c-simple-example";

static esp_err_t i2c_master_init(void)  //inicializa o I2C no modo mestre
{
    int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = { 
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = 1,
        .scl_pullup_en = 1,
        .master.clk_speed = 100000,  
    };

    i2c_param_config(I2C_NUM_0, &conf);

    return i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
}

void app_main(void)
{
	
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    
    lcd_init();
    
    lcd_put_cur(0, 0);
    
    lcd_send_string("Hello World");
    

}
