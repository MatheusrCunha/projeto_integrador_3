/*
 * rtc.h
 *
 *  Created on: Jan 8, 2025
 *      Author: Ueslei
 */
 #ifndef _RTC
 #define _RTC

 #include <time.h>
 #include <stdbool.h>

 #define MAX_ALARMS 24 // Número máximo de alarmes diários


void init_alarms(void);
void init_rtc(void);
long int set_date(int timeval_sec);
void print_date(void);
time_t get_time(void);
void sort_alarms(void);
const char* insert_alarm(time_t tt, int _number_doses);
const char* disable_alarm(int _indice);
bool check_alarm(void);
//Retorna quantidade de alarmes
int count_alarms(void);
int get_all_alarms(time_t alarm_list[MAX_ALARMS]);
char* get_alarm_format(int _pos);


 #endif /* _RTC */