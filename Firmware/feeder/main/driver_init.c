/*
 * driver_init.c
 *
 *  Created on: 1 de dez. de 2024
 *      Author: Ueslei
 */

#include "driver_init.h"
#include <stdbool.h>
#include "esp_task_wdt.h"

/**
 * Inicializa os pinos dos botões touch
 *
 */

const touch_pad_t Num_touch[] = {
    TOUCH_PAD_NUM4,  // GPIO13
    TOUCH_PAD_NUM6,  // GPIO14
    TOUCH_PAD_NUM7   // GPIO27
};


void init_touch() {
    esp_err_t err;
    // Inicializa o driver dos touch pads
    err = touch_pad_init();
    if (err != ESP_OK) {
        printf("Erro ao inicializar touch_pad: %s\n", esp_err_to_name(err));
        return;
    }

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);

    // Configura a tensão dos touch pads
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

    // Configura os touch pads especificados
    touch_pad_config(TOUCH_PAD_NUM4, -1);
    touch_pad_config(TOUCH_PAD_NUM6, -1);
    touch_pad_config(TOUCH_PAD_NUM7, -1);
    // Habilita interrupções nos touch pads
    touch_pad_intr_enable();

    // Inicia o filtro para suavizar as leituras
    touch_pad_filter_start(10);

}

bool read_buttons(unsigned char __numTecla) {
    if (__numTecla >= sizeof(Num_touch) / sizeof(Num_touch[0])) {
        printf("Erro: índice inválido para __numTecla\n");
        return false;
    }

    bool bt_Pressed = false;
    uint16_t filtered_value = 0;

    // Lê o valor filtrado do touch pad
    touch_pad_read_filtered(Num_touch[__numTecla], &filtered_value);
    //printf("%d = %d\n", __numTecla, filtered_value);

    // Verifica se o botão foi pressionado
    if (filtered_value < 1100) {
        bt_Pressed = true;
    } else {
        bt_Pressed = false;
    }

    return bt_Pressed;
}

bool btnsPressed[NUM_BTNS] = { false };
bool debouncing[NUM_BTNS] = { false };
bool holdingBtn[NUM_BTNS] = { false };

extern volatile uint32_t tickCount; // Contagem de ticks em milissegundos
uint32_t TickDebounce[NUM_BTNS] = {0};

void sweep_buttons(void) {
    for (int i = 0; i < NUM_BTNS; i++) {
        // Verifica se o botão foi pressionado
        if (read_buttons(i)) {
            if (!debouncing[i]) {
                // Inicia debounce para o botão
                TickDebounce[i] = tickCount;
                debouncing[i] = true;
                holdingBtn[i] = true;
            }
            // Verifica o tempo de debounce
            else if (TickStampDelta(TickDebounce[i], tickCount) > DebounceTicks && holdingBtn[i]) {
                btnsPressed[i] = true;
                holdingBtn[i] = false; // Permite apenas uma contagem por pressão
            }
        } else {
            // Reseta o estado de debounce se o botão não estiver pressionado
            debouncing[i] = false;
        }
    }
}

/**
 * Retorna se um botão foi pressionado
 * __tecla -> índice do botão a ser verificado
 */
bool GetButton(unsigned char __tecla) { 
    if (btnsPressed[__tecla]) {
        btnsPressed[__tecla] = false;
        return true;
    } else {
        return false;
    }
}


