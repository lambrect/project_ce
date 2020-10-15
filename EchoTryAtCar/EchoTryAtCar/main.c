/*
 * EchoTryAtCar.c
 *
 * Created: 14.10.2020 16:18:30
 * Author : tim-l
 */ 

#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart.h"

volatile bool echo;
uint16_t distance;
uint16_t ticks= 0;

void initPinsAndInterrupts(){
	DDRB |= 1 << DDB1;		//set PORTB1 as output (used to send a trigger signal for ultrasonic sensor)
	
	//The next two lines initialize interrupt code for external interrupt INT1 (see documentation for details)
	EICRA |= (1 << ISC10);		//External interrupt control register set INT1 to trigger ANY logic change
	EIMSK |= (1 << INT1);		//Turn on INT1
	
	TIMSK1 |= (1<<TOIE1);					//enable the interrupt flag
	TCCR1B |= (1<< CS12) | (1<< CS10);		//prescaler of 1024
	TCCR1A = 0b00000000;					// normal mode
	TIFR1 |= (1<<TOV1);						// enable overflow flag
}
	
	

ISR(INT1_vect){	//ISR for when there is a change in logic level (for ultrasonic)
	
	if (PIND & (1 << PIND3))		//if echosignal is 1, the echo signal is high
	{
		TCNT1 = 0;	//set timer data register to 0
	}
	else if(!(PIND & (1 << PIND3)))	//if echosignal is 0, the echo signal is low
	{
		ticks=TCNT1;		//store the length of the signal in lengthSignal
		echo = true;
	}
}

void trigger() {
	PORTB &= ~(1<<PORTB1);	//set EDS pin to 0
	_delay_us(2);			//wait 2 microseconds
	PORTB |= (1<<PORTB1);	//set PORTB1 to high and trigger the signal from the ultrasonic distance sensor
	_delay_us(10);			//wait till the signal is send
	PORTB &= ~(1<<PORTB1);	// set EDS pin to 0 again
	echo = false;
}
	
	
int main(void)
{
	initPinsAndInterrupts();
	
	sei();
	//initialize uart
						//Data-Bits out | Stop-bit      |  Parity-Bit   | which mode
	uart_set_FrameFormat(USART_8BIT_DATA|USART_1STOP_BIT|USART_NO_PARITY|USART_ASYNC_MODE);
	// calculate the Baudrate in double speed mode
	uart_init(DOUBLE_BAUD_CALC(115200));
	
    /* Replace with your application code */
    while (1) 
    {
		trigger();
		//while(!echo);
		if(ticks < 368)
		{
			distance = ticks * 1.088 ;
			
		}
		else
		{
			distance= 11111; // for measure out of range
		}
		
		char buffer[50]; // for the output on the monitor
		snprintf(buffer, 50, "Distance: %d\n\r", distance); //saves the String in the buffer
		uart_puts(buffer);	//send it with uart
		_delay_ms(100);
    }
}

