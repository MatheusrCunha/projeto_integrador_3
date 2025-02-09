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


int POS_TIMER[][2] = {
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

void sweep_stateMachine(void)
{
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
						state = pag_timers_1;
						break;
			
					default:
						break;
				}
			} 
            if (GetButton(btn_D14)) {printf("Botão D14 pressionado\n");}
			break;
			
		case update_date:

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
			}
	

			state=standby;
			last_state=pag_timers_1;

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

			state=pag_timers_1;
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

			state=pag_timers_1;
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

			state=pag_timers_1;
			last_state=standby;

			break;

		case working:
			
			break;
			

		default:
			state = standby;
	}
}

