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
#include <string.h>


struct tm data; // Cria a estrutura que contem as informacoes da data.

// Estrutura para armazenar os alarmes
typedef struct {
    bool active;      // Indica se o alarme está ativo
    struct tm time;   // Horário do alarme
    int doses;        // Quantidade de doses
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
    init_alarms();
}


//Seta a data no RTC pelo valor no formato UNIX e retorna o valor em UNIX ja implementado no RTC.
long int set_date(int timeval_sec)
{
    if (timeval_sec < 0) {
        printf("Erro: tempo inválido!\n");
        return -1; // Retorna um valor de erro
    }
    
    DEBUG_PRINT(("set_date=%d\n",timeval_sec));
    
    struct timeval tv;       // Use 'struct' para definir a estrutura
    tv.tv_sec = timeval_sec; // Atribui um valor à tv_sec
    tv.tv_usec = 0;   // Define tv_usec como 0 para não adicionar microssegundos
    settimeofday(&tv, NULL); // Configura o horário
    time_t current_time = get_time();

    return (long int) current_time;

}

void print_date(void)
{
    time_t tt = time(NULL); // Obtem o tempo atual em segundos.
    data = *gmtime(&tt);    // Converte o tempo atual e atribui na estrutura

    char data_formatada[64];
    strftime(data_formatada, 64, "%d/%m/%Y %H:%M:%S", &data); // Cria uma String formatada da estrutura "data"

    DEBUG_PRINT(("\nUnix Time: %ld\n", (long)tt));         // Mostra na Serial o Unix time.
    DEBUG_PRINT(("Data formatada: %s\n", data_formatada)); // Mostra na Serial a data formatada

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
const char* insert_alarm(time_t tt, int _number_doses)
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
            alarms[i].doses = _number_doses;
            printf("Alarme inserido: %02d:%02d:%02d Doses:%02d\n",
                   alarms[i].time.tm_hour, alarms[i].time.tm_min, alarms[i].time.tm_sec, alarms[i].doses);
            sort_alarms(); //Ordena cronologicamente os alarmes
            return "Alarme inserido com sucesso";
        }
    }

    // Se não houver slots livres, retorna mensagem de erro
    DEBUG_PRINT(("Erro: Limite de alarmes atingido!\n"));
    return "Erro: Limite de alarmes atingido";
}


// desabilita alarme e move para ultima posição da fila.
const char* disable_alarm(int _indice)
{
    if (_indice >= MAX_ALARMS) {
        printf("Indice acima do limite\n");
        return ("Indice acima do limite"); // 
    }

    if (count_alarms()==0) { //Confere se ainte tem 
        printf("Não possui nenhum alarm para excluir\n");
        return ("Não possui nenhum alarm para excluir"); 
    }

    int last_alarm = (count_alarms()-1);
    alarms[_indice]= alarms[last_alarm]; // Define o horário do alarme

    alarms[last_alarm].active = false; // Desabilita o alarme de acordo com o _indice
    static char mensagem[32]; 
    sprintf(mensagem, "Alarme %d desabilitado", _indice); // Corrigir o uso do sprintf
    sort_alarms(); //Ordena cronologicamente os alarmes 

    return mensagem;
}


//Confere se alarm disparou e retorna quantidade de doses
int check_alarm(void)
{
    time_t now = time(NULL);
    struct tm current_time = *localtime(&now); // Obtém o horário atual

    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarms[i].active &&
            alarms[i].time.tm_hour == current_time.tm_hour &&
            alarms[i].time.tm_min == current_time.tm_min &&
            alarms[i].time.tm_sec == current_time.tm_sec) {
            DEBUG_PRINT(("Alarme disparado: %02d:%02d:%02d\n",
                   alarms[i].time.tm_hour, alarms[i].time.tm_min, alarms[i].time.tm_sec));
            return alarms[i].doses;

            // Desativa o alarme após disparar (opcional)
            //alarms[i].active = false;
        }
    }
    return false;
}


int count_alarms(void)
{
    int count = 0;

    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarms[i].active) {
            count++;
        }
    }

    return count; // Retorna o número de alarmes ativos
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


// Retona o alarme na forma de string "_pos-HH:MM" 
char* get_alarm_format(int _pos)
{
    static char alarm_str[20]; // Buffer estático para armazenar o formato HH:MM

    if(_pos >= MAX_ALARMS){
        DEBUG_PRINT(("Posição de alarme acima do limite"));
        return "";
    }
    else { 
        snprintf(alarm_str, sizeof(alarm_str), "%d-%02d:%02d", _pos,alarms[_pos].time.tm_hour, alarms[_pos].time.tm_min);
        return alarm_str;
    }
}


char* get_all_alarm_format_app(void)
{
    static char alarm_str[1024]; // Buffer estático para armazenar o formato HH:MM
    alarm_str[0] = '\0'; // Inicializa a string vazia

    char temp[40]; // Buffer temporário para cada entrada

    for (size_t i = 0; i < MAX_ALARMS; i++)
    {
        if(alarms[i].active){
            snprintf(temp, sizeof(temp), "%d-%02d:%02d - %d doses,", i, alarms[i].time.tm_hour, alarms[i].time.tm_min, alarms[i].doses);
            strncat(alarm_str, temp, sizeof(alarm_str) - strlen(alarm_str) - 1); // Concatena o buffer temporário ao buffer principal
        }

    }
    DEBUG_PRINT(("%s\n",alarm_str));

    return alarm_str;
}