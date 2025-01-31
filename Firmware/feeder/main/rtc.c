/*
 * rtc.c
 *
 *  Created on: Jan 8, 2025
 *      Author: Ueslei
 */


#include "rtc.h"
#include "defines.h"
#include "i2c-lcd.h"


#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>



struct tm data; // Cria a estrutura que contem as informacoes da data.



// Estrutura para armazenar os alarmes
typedef struct {
    bool active;      // Indica se o alarme está ativo
    struct tm time;   // Horário do alarme
} Alarm;

Alarm alarms[MAX_ALARMS]; // Array para armazenar os alarmes

// Função para inicializar os alarmes
void init_alarms(void)
{
    for (int i = 0; i < MAX_ALARMS; i++) {
        alarms[i].active = false; // Marca todos os alarmes como inativos
    }
}

void init_rtc(void)
{
    set_date(0);// Seta data como 01/01/1970 00:00:00
    //struct timeval tv;       // Use 'struct' para definir a estrutura
    //tv.tv_sec = 0; // Atribui um valor à tv_sec
   // settimeofday(&tv, NULL); // Configura o horário
    init_alarms();
}



void set_date(int timeval_sec)
{
    if (timeval_sec < 0) {
        printf("Erro: tempo inválido!\n");
        return;
    }

    struct timeval tv;       // Use 'struct' para definir a estrutura
    tv.tv_sec = timeval_sec; // Atribui um valor à tv_sec
    settimeofday(&tv, NULL); // Configura o horário
    
}



void print_date(void)
{
    time_t tt = time(NULL); // Obtem o tempo atual em segundos.
    data = *gmtime(&tt);    // Converte o tempo atual e atribui na estrutura

    char data_formatada[64];
    strftime(data_formatada, 64, "%d/%m/%Y %H:%M:%S", &data); // Cria uma String formatada da estrutura "data"

    DEBUG_PRINT(("\nUnix Time: %ld\n", (long)tt));         // Mostra na Serial o Unix time.
    DEBUG_PRINT(("Data formatada: %s\n", data_formatada)); // Mostra na Serial a data formatada
    
    //REMOVER - APenas para teste
    strftime(data_formatada, 64, "%H:%M:%S", &data);
    lcd_put_cur(0, 12); // Muda cursor para segunda Linha
    lcd_send_string(data_formatada); //envia uma string
   
}

time_t get_time(void)
{
    return (time(NULL)); // Obtem o tempo atual em segundos.
}

void sort_alarms(void)
{
    for (int i = 0; i < MAX_ALARMS - 1; i++) {
        for (int j = 0; j < MAX_ALARMS - i - 1; j++) {
            if (alarms[j].active && alarms[j + 1].active) { // Comparar apenas alarmes ativos
                // Converte os horários dos alarmes para Unix time para comparar
                time_t time_j = mktime(&alarms[j].time);
                time_t time_next = mktime(&alarms[j + 1].time);

                if (time_j > time_next) {
                    // Troca os alarmes se estiverem fora de ordem
                    Alarm temp = alarms[j];
                    alarms[j] = alarms[j + 1];
                    alarms[j + 1] = temp;
                }
            }
        }
    }
}



// Função para inserir um novo alarme
const char* insert_alarm(time_t tt)
{
    struct tm alarm_time = *localtime(&tt); // Converte o Unix time para struct tm

    // Verifica se o horário já está programado
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarms[i].active &&
            alarms[i].time.tm_hour == alarm_time.tm_hour &&
            alarms[i].time.tm_min == alarm_time.tm_min &&
            alarms[i].time.tm_sec == alarm_time.tm_sec
            ) {
            DEBUG_PRINT(("Erro: Esse alarme já está programado para %02d:%02d:%02d\n",
                   alarm_time.tm_hour, alarm_time.tm_min, alarm_time.tm_sec));
            return "Esse alarme já está programado";
        }
    }

    // Procura um slot livre para inserir o novo alarme
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (!alarms[i].active) { // Procura o primeiro slot livre
            alarms[i].time = alarm_time; // Define o horário do alarme
            alarms[i].active = true;     // Ativa o alarme
            printf("Alarme inserido: %02d:%02d:%02d\n",
                   alarms[i].time.tm_hour, alarms[i].time.tm_min, alarms[i].time.tm_sec);
            sort_alarms(); //Ordena cronologicamente os alarmes 
            return "Alarme inserido com sucesso";
        }
    }

    // Se não houver slots livres, retorna mensagem de erro
    DEBUG_PRINT(("Erro: Limite de alarmes atingido!\n"));
    return "Erro: Limite de alarmes atingido";
}



bool check_alarm(void)
{
    time_t now = time(NULL);
    struct tm current_time = *localtime(&now); // Obtém o horário atual

    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarms[i].active &&
            alarms[i].time.tm_hour == current_time.tm_hour &&
            alarms[i].time.tm_min == current_time.tm_min &&
            alarms[i].time.tm_sec == current_time.tm_sec) {
            printf("Alarme disparado: %02d:%02d:%02d\n",
                   alarms[i].time.tm_hour, alarms[i].time.tm_min, alarms[i].time.tm_sec);
            return true;

            // Desativa o alarme após disparar (opcional)
            //alarms[i].active = false;
        }
    }
    return false;
}



int get_all_alarms(time_t alarm_list[MAX_ALARMS])
{
    int count = 0;

    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarms[i].active) {
            alarm_list[count++] = mktime(&alarms[i].time); // Adiciona o alarme ativo à lista
        }
    }

    return count; // Retorna o número de alarmes ativos
}
