#include "timer_utils.h"
#include "esp_log.h"




volatile uint32_t tickCount = 0;


static portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
bool IRAM_ATTR onTimer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    portENTER_CRITICAL_ISR(&timerMux);
    tickCount++;
    portEXIT_CRITICAL_ISR(&timerMux);
    return true; // Reiniciar o alarme
}

uint32_t get_tickCount(void) {
    uint32_t ticks;
    portENTER_CRITICAL(&timerMux);
    ticks = tickCount;
    portEXIT_CRITICAL(&timerMux);
    return ticks;
}



void init_timer_1ms() {
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,
    };
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 1000,
        .flags = {
            .auto_reload_on_alarm = true,
        },
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &(gptimer_event_callbacks_t){
        .on_alarm = onTimer,
    }, NULL));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

uint32_t TickStampDelta(const uint32_t lastTick, const uint32_t tickNow) {
    if (lastTick > tickNow) {
        return (0xFFFFFFFF - lastTick) + tickNow;
    } else {
        return tickNow - lastTick;
    }
}
