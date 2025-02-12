/*
 * stateMachine.c
 *
 * Created: 02/02/2025
 *  Author: Ueslei
 */ 

#include "stateMachine.h"
#include "i2c-lcd.h"
#include "rtc.h"
#include "defines.h"


int POS_TIMER[][2] = {   //Posição linha e coluna dos timers para mostrar no display
    {1, 0},
    {2, 0},
    {3, 0},
    {1, 12},
    {2, 12},
    {3, 12}
};

#define SET_CURSOR_TIMER(x) lcd_put_cur(POS_TIMER[x][0], POS_TIMER[x][1])  // Chama de mácro de acordo com a posição pré definida

#define OFFSET_PAG2 6
#define OFFSET_PAG3 12
#define OFFSET_PAG4 18

int	state = 0, last_state = 0, adc;

int hora = 0;	 //Apenas para configuraçao de alarme e relógio
int minuto = 0;	 //Apenas para configuraçao de alarme e relógio
int doses = MIN_DOSES;	 //Apenas para configuraçao de alarme e relógio
static char alarm_str[25];//Apenas paramensagem no display do alarme e relógio
uint32_t lastUpdate = 0; // Variável para controlar o tempo da última atualização do display


void atualiza_hour_minute_lcd(void){
	lcd_put_cur(2, 0);
	snprintf(alarm_str, sizeof(alarm_str), "%02d:%02d               ",hora,minuto);
	lcd_send_string(alarm_str);
}

void sweep_stateMachine(void)
{
	uint32_t currentTime = tickCount;
    if (TickStampDelta(lastUpdate, currentTime) >= 1000) {
        print_date_lcd(); // Atualiza a data no display a cada 1 segundo
        lastUpdate = currentTime;
    }

	switch (state)
	{
		case standby:
            if (GetButton(btn_D13)) { //"Botão D13 pressionado\n"
				switch (last_state)
				{
					case standby:
						state = pag_timers_1;
						break;

					case pag_timers_1:
						state = pag_timers_2;
						break;

					case pag_timers_2:
						state = pag_timers_3;
						break;

					case pag_timers_3:
						state = pag_timers_4;
						break;
					
					case pag_timers_4:
						state = pag_adj_hours;
						break;
					
					case pag_adj_hours:
						state = pag_config_timer;
						break;
						
					case pag_config_timer:
						state = pag_timers_1;
						break;
			
					default:
						break;
				}
			} 
            if (GetButton(btn_D14)) {
				
				printf("last_state: %d", last_state);
				switch (last_state)
				{
					case pag_config_timer:
						
						lcd_clear();
						print_date_lcd(); //Atualiza hora no display
						lcd_put_cur(1, 0);
						lcd_send_string("Defina a hora!");
						lcd_put_cur(3, 0);
						lcd_send_string("--    ");
						atualiza_hour_minute_lcd();

						state = pag_config_hours;

						break;

					case pag_adj_hours:

						lcd_clear();
						print_date_lcd(); //Atualiza hora no display
						lcd_put_cur(1, 0);
						lcd_send_string("Defina a hora!");
						lcd_put_cur(3, 0);
						lcd_send_string("--    ");
						atualiza_hour_minute_lcd();
						state = pag_config_hours;

						break;

					default:
						break;
				}
			}
			break;			

		case pag_timers_1:
			lcd_clear();

			lcd_put_cur(0, 0);
			lcd_send_string("Feeders");
			print_date_lcd(); //Atualiza hora no display
			
			int cont = count_alarms();

			if(cont > 0){
				for (size_t i = 0; i < MIN(OFFSET_PAG2, cont); i++)
				{
					SET_CURSOR_TIMER(i);
					lcd_send_string(get_alarm_format(i));
				}

				state=standby;
				last_state=pag_timers_1;
	
				break;
			}

			state=pag_adj_hours;
			last_state=standby;

			break;

		case pag_timers_2:
			lcd_clear();

			lcd_put_cur(0, 0);
			lcd_send_string("Feeders");
			print_date_lcd(); //Atualiza hora no display
		
			cont = count_alarms();

			if(cont>=OFFSET_PAG2){
				for (size_t i = OFFSET_PAG2; i < MIN(OFFSET_PAG3, cont); i++)
				{
					SET_CURSOR_TIMER(i-OFFSET_PAG2);
					lcd_send_string(get_alarm_format(i));
				}
				state=standby;
				last_state=pag_timers_2;
				break;
			}

			state=pag_adj_hours;
			last_state=standby;

			break;

		case pag_timers_3:
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string("Feeders");
			print_date_lcd(); //Atualiza hora no display

			cont = count_alarms();

			if(cont>=OFFSET_PAG3){
				for (size_t i = OFFSET_PAG3; i < MIN(OFFSET_PAG4, cont); i++)
				{
					SET_CURSOR_TIMER(i-OFFSET_PAG3);
					lcd_send_string(get_alarm_format(i));
				}
			
				state=standby;
				last_state=pag_timers_3;
				break;
			}

			state=pag_adj_hours;
			last_state=standby;

			break;

		case pag_timers_4:
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string("Feeders");
			print_date_lcd(); //Atualiza hora no display
			
			cont = count_alarms();

			if(cont>=OFFSET_PAG4){
				for (size_t i = OFFSET_PAG4; i < MIN(MAX_ALARMS, cont); i++)
				{
					SET_CURSOR_TIMER(i-OFFSET_PAG4);
					lcd_send_string(get_alarm_format(i));
				}

				state=standby;
				last_state=pag_timers_4;
				break;
			}

			state=pag_adj_hours;
			last_state=standby;

			break;

		case pag_adj_hours:

			lcd_clear();
			print_date_lcd(); //Atualiza hora no display
			lcd_put_cur(1, 0);
			lcd_send_string("Configurar hora?");

			state=standby;
			last_state=pag_adj_hours;
			
			break;

		case pag_config_timer:
			
			hora = 0;
			minuto = 0;
			doses = MIN_DOSES;

			lcd_clear();
			print_date_lcd(); //Atualiza hora no display
			lcd_put_cur(1, 0);
			lcd_send_string("Configurar alarme?");

			state=standby;
			last_state=pag_config_timer;
			
			break;


		case pag_config_hours:
			
			if (GetButton(btn_D13)) { //"Botão D13 pressionado\n"
				
				if(hora <= 22){
					DEBUG_PRINT(("hora:%d\n", hora));

					hora++;
					atualiza_hour_minute_lcd();

				}else{
					hora=0;
					atualiza_hour_minute_lcd();

				}
			}
			if (GetButton(btn_D27)) { //"Botão D27 pressionado\n"
			
				if(hora >= 1){
					DEBUG_PRINT(("hora:%d\n", hora));

					hora--;
					atualiza_hour_minute_lcd();

				}else{
					hora=23;
					atualiza_hour_minute_lcd();

				}
				
			}
			

			if (GetButton(btn_D14)) { //"Botão D14 pressionado\n"
				
				lcd_put_cur(1, 0);
				lcd_send_string("Defina os minutos!  ");
				lcd_put_cur(3, 0);
				lcd_send_string("   --");
				state=pag_config_min;

			}
	
			break;

		case pag_config_min:
			
			if (GetButton(btn_D13)) { //"Botão D13 pressionado\n"
				
				if(minuto <59){
					minuto++;
					atualiza_hour_minute_lcd();

				}else{
					minuto = 0;
					atualiza_hour_minute_lcd();

				}
				
			}
			if (GetButton(btn_D27)) { //"Botão D13 pressionado\n"
				
				if(minuto >=1){
					minuto--;
					atualiza_hour_minute_lcd();

				}else{
					minuto = 59;
					atualiza_hour_minute_lcd();

				}
				
			}

			if (GetButton(btn_D14)) {
				
				if(last_state == pag_adj_hours)  { //"Botão D14 pressionado\n"e
					set_date((hora * 3600) + (minuto*60));
					hora=minuto=0;
					state=pag_timers_1;
					last_state = standby;
				} else {
					lcd_put_cur(2, 0);
					snprintf(alarm_str, sizeof(alarm_str), "%02d:%02d  doses:%02d",hora,minuto,doses);
					lcd_send_string(alarm_str);
					lcd_put_cur(3, 0);
					lcd_send_string("             --"); //Apenas para apagar a marcação dos minutos da tela anterior
					state=pag_config_doses;
				}
			}
	
			break;

		case pag_config_doses:
			
			if (GetButton(btn_D13)) { //"Botão D13 pressionado\n"
				if(doses < MAX_DOSES){
					doses++;
					lcd_put_cur(2, 0);
					snprintf(alarm_str, sizeof(alarm_str), "%02d:%02d  doses:%02d     ",hora,minuto,doses);
					lcd_send_string(alarm_str);
				}else{
					doses = MIN_DOSES;
					lcd_put_cur(2, 0);
					snprintf(alarm_str, sizeof(alarm_str), "%02d:%02d  doses:%02d     ",hora,minuto,doses);
					lcd_send_string(alarm_str);
				}
			}

			if (GetButton(btn_D27)) { //"Botão D13 pressionado\n"
				if(doses>MIN_DOSES){
					doses--;
					lcd_put_cur(2, 0);
					snprintf(alarm_str, sizeof(alarm_str), "%02d:%02d  doses:%02d     ",hora,minuto,doses);
					lcd_send_string(alarm_str);
				}else{
					doses = MAX_DOSES;
					lcd_put_cur(2, 0);
					snprintf(alarm_str, sizeof(alarm_str), "%02d:%02d  doses:%02d     ",hora,minuto,doses);
					lcd_send_string(alarm_str);
				}
			}
			

			if (GetButton(btn_D14)) { //"Botão D14 pressionado\n"e
				insert_alarm(((hora*3600) + (minuto*60)), doses);
				hora=minuto=doses=1;
				state=pag_timers_1;
				last_state = standby;
			} 
	
			break;
			

		default:
			state = standby;
	}
}

