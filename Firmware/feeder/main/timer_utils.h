#ifndef TIMER_UTILS_H
#define TIMER_UTILS_H

#include <stdint.h>
#include "driver/gptimer.h"
#include "esp_attr.h" // Certifica-se que IRAM_ATTR é definido

#include "freertos/portmacro.h"

extern volatile uint32_t tickCount;

void init_timer_1ms();
uint32_t TickStampDelta(const uint32_t lastTick, const uint32_t tickNow);
bool IRAM_ATTR onTimer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
uint32_t get_tickCount(void); // Nova função para acessar tickCount com segurança

#endif // TIMER_UTILS_H
