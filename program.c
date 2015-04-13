/********************************************************************************
/ Includes
/*******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include "game.h"

/********************************************************************************
/ Interrupt Routines
/*******************************************************************************/
 
int counter;

// timer0 overflow 1024 times per second
ISR(TIMER0_OVF_vect) {
	
	move();
	if(counter++>=256)
	{
		counter = 0;
		PORTA = (PORTA ^ _BV(PA6)) | ~_BV(PA6);
	}	
}
 
/********************************************************************************
/ Main
/*******************************************************************************/
int main( void ) {
    
	// set led as output
	DDRA |= _BV(PA6);
	// enable timer overflow interrupt for Timer0
    TIMSK |= _BV(TOIE0);
    // set Timer0 counter initial value to 0
    TCNT0 = 0x00;
    // start timer0 with /32 prescaler
    TCCR0 = _BV(CS01) | _BV(CS00);
    // enable interrupts
    sei(); 

    while(1)
    {
		_NOP();
    }
}
