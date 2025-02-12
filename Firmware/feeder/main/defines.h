/*
 * defines.h
 *
 *  Created on: 1 de dez. de 2024
 *      Author: ueslei
 */

#ifndef MAIN_DEFINES_H_
#define MAIN_DEFINES_H_

//#define DEBUG

#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) do { printf x; } while (0)
#else
#define DEBUG_PRINT(x) do { } while (0)
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b)) // Define uma macro para obter o valor mínimo

//BUTTONS ENUMERATION   Numeros são usados para acessar a 

#define btn_set						0  
#define btn_init					1
#define btn_D13					    0
#define btn_D14					    1
#define btn_D27					    2

#define MAX_DOSES                   5
#define MIN_DOSES                   1


//States
#define	standby							0
#define	show_timers			        	2
#define	pag_timers_1					3
#define	pag_timers_2			        4
#define	pag_timers_3			        5
#define	pag_timers_4			        6
#define	pag_adj_hours					8       	
#define	pag_config_hours				9   
#define	pag_config_min				    10
#define	pag_config_doses			    11
#define	pag_config_timer		        12


#endif /* MAIN_DEFINES_H_ */
