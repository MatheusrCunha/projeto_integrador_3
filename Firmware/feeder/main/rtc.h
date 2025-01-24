/*
 * rtc.h
 *
 *  Created on: Jan 8, 2025
 *      Author: Ueslei
 */
 #ifndef _RTC
 #define _RTC

 #include <time.h>

 #define MAX_ALARMS 24 // Número máximo de alarmes diários


void init_alarms(void);
void init_rtc(void);
void set_date(int timeval_sec);
void print_date(void);
time_t get_time(void);
void sort_alarms(void);
const char* insert_alarm(time_t tt);
void check_alarm(void);
int get_all_alarms(time_t alarm_list[MAX_ALARMS]);


 #endif /* _RTC */