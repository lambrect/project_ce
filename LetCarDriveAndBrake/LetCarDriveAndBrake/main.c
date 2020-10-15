/*
 * LetCarDrive.c
 *
 * Created: 14.10.2020 12:21:22
 * Author : tim-l
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
//#include "uart.h"
//#define F_CPU 16000000UL
#define BaudRate 9600
#define MYUBRR ( F_CPU / 16 / BaudRate ) -  1

//define the Motor Control Pins
#define RIGHT1 PORTD2	// Datasheet L_CTRL_1
#define RIGHT2 PORTD4	// Datasheet L_CTRL_2
#define LEFT1 PORTD7	// Datasheet R_CTRL_1
#define LEFT2 PORTB0	// Datasheet R_CTRL_2


void initPWM()
{
	DDRD |= (1<< PORTD5); //set PORTD pin5 as an output (left motor) speed
	DDRD |= (1 << PORTD6);// set PORTD pin6 as an output (right motor)speed
	TCCR0A |= (1 << COM0A1);// set none-inverting mode for OCR0A
	TCCR0A |= (1 << COM0B1);// set none-inverting mode for OCR0B
	TCCR0A |= (1 << WGM01) | (1 << WGM00);// set fast PWM Mode
	TCCR0B |= (1 << CS01);// sets prescaler to 8 and starts PWM
}

// sets the direction for the car
void motorDirection(uint8_t direction)
{
	if(direction) // if it's not 0 drive forward
	{
		PORTD |= (1<<RIGHT1);		//set D2 to high
		PORTD &= ~(1<<RIGHT2);		//set D4 to low 
		PORTD |= (1<<LEFT1);		//set D7 to high 
		PORTB &= ~(1<<LEFT2);		//set B0 to low	
	}
	else{
		//drive backwards
		PORTD &= ~(1<<RIGHT1);	//set D2 to low 
		PORTD |= (1<<RIGHT2);	//set D4 to high
		PORTD &= ~(1<<LEFT1);	//set D7 to low
		PORTB |= (1<<LEFT2);	//set B0 to high
	}
}


void brake(){
	//set all to high to brake
	PORTD |= (1<<RIGHT1);	//set D2 to high
	PORTD |= (1<<RIGHT2);	//set D4 to high
	PORTD |= (1<<LEFT1);	//set B0 to high
	PORTB |= (1<<LEFT2);	//set D7 to high
}

// Initialize the Button on the Robot Car
void initButton(){
	DDRB &= ~(1<< PORTB4);	// set PORTB pin4 as an input
	PORTB |= 1<< PORTB4;	// default state is high
}

int main(void)
{
	initButton();	//initialize Button
	initPWM();		//initialize PWM
	while (1) 
    {
			while((PINB & (1<<PINB4))); // wait till Button pressed and PINB4 is grounded, then leave the while and move on to the next step
			motorDirection(1); //direction forwards
			OCR0A=128;	// right motor power
			OCR0B=128; // left motor power
			_delay_ms(2000);  // 
			while((PINB & (1<<PINB4))); // press Button again for hitting the brakes
			brake();
			_delay_ms(1000); 
    }
}

