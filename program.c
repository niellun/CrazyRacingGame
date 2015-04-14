/********************************************************************************
/ Includes
/*******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include "lcd.h"
#include "game.h"

char getgraphics(byte x, bool left)
{
	char c = '.';
	if(x & _BV(ROAD_LINE))
		c = left ? '(' : ')';
	if(x & _BV(CAR_FRONT))
		c = x & _BV(CAR_JUMP) ? '8': '1';
	if(x & _BV(CAR_BACK))
		c = x & _BV(CAR_JUMP) ? '9': '2';
	if(x & _BV(OBSTACLE1))
		c = 'x';
	if(x & _BV(OBSTACLE2))
		c = 'x';
	if(x & _BV(OBSTACLE3))
		c = 'x';
	if(x & _BV(OBSTACLE4))
		c = 'x';
	return c;
}

bool dispdraw()
{
	if(draw())
		return true;
	
	// draw on display
	lcd_write_ctrl(LCD_CLEAR);
	for(byte i=ROAD_LENGTH-1; i<ROAD_LENGTH; i--)
	{
		lcd_write_data(getgraphics(road2[i], false));
	}

	lcd_write_ctrl(ENTER);
	
	for(byte i=ROAD_LENGTH-1; i<ROAD_LENGTH; i--)
	{
		lcd_write_data(getgraphics(road1[i], true));
	}

	return false;
}

/********************************************************************************
/ Interrupt Routines
/*******************************************************************************/

int counter;

// timer0 overflow 1024 times per second
ISR(TIMER0_OVF_vect) {

	bool redraw = false;
	if(!(PINA&_BV(PA4)))
		redraw |= left();

	if(!(PINA&_BV(PA0)))
		redraw |= right();

	//if(PINA&_BV(PA2))
	//	redraw |= forward();

	//if(PINA&_BV(PA3))
	//	redraw |= backward();

	if(!(PINA&_BV(PA2)))
		redraw |= jump();

	if(redraw)
	{
		dispdraw();
	}

	if(counter++>=1024)
	{
		counter = 0;


		move();
		dispdraw();
	}	
}
 
/********************************************************************************
/ Main
/*******************************************************************************/
int main( void ) {

	lcd_init();
	lcd_write_ctrl(LCD_ON);
	lcd_write_ctrl(LCD_CLEAR);

	init();
	dispdraw();

	// set led as output
	DDRA |= _BV(PA6);
	
	//set buttons as input
	DDRA &= ~(_BV(PA0)|_BV(PA1)|_BV(PA2)|_BV(PA3)|_BV(PA4));

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
