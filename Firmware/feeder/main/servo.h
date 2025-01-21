/*
 * servo.h
 *
 *  Created on: Dec 2, 2024
 *      Author: mathe
 */

#ifndef MAIN_SERVO_H_
#define MAIN_SERVO_H_

#define SERVO_PIN GPIO_NUM_12 //define o pino GPIO12
#define BT GPIO_NUM_4 //Define o pino GPIO4
#define LEDC_CHANNEL LEDC_CHANNEL_0 //Canal 0 do controlador
#define LEDC_TIMER LEDC_TIMER_0 //define o timer 0
#define LEDC_MODE LEDC_LOW_SPEED_MODE //modo de baixa velocidade
#define LEDC_FREQUENCY 50 // Frequência PWM

#define START_POSITION 57 //Angulo em que o servo ENCHE o recipiente
#define END_POSITION 152 //Angulo em que o servo ESVAZIA o recipiente


int ServoAngle(int angle);

void moveServo(int angle);

void configure_ledc_timer(void);

void configure_ledc_channel(void);
void init_servo(void);


void initServo();
void validaServo();

//void touchbt();

#endif /* MAIN_SERVO_H_ */
