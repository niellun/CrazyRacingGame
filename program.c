/********************************************************************************
/ Includes
/*******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include "lcd.h"
#include "game.h"
#include "pcm_sample.h"

#define DEBOUNCE 512
#define START 5
#define END 10
#define WAIT 20

volatile byte instance;
volatile byte music_type;

char getgraphics(byte x, bool left)
{
	char c = ' ';
	if(x & _BV(ROAD_LINE))
		c = left ? instance%2 : (instance+1)%2;
	if(x & _BV(OBSTACLE1))
		c = 7;
	if(x & _BV(OBSTACLE2))
		c = 6;
	if(x & _BV(OBSTACLE3))
		c = 'x';
	if(x & _BV(OBSTACLE4))
		c = 'X';
	if(x & _BV(CAR_FRONT))
		c = x & _BV(CAR_JUMP) ? 2: 4;
	if(x & _BV(CAR_BACK))
		c = x & _BV(CAR_JUMP) ? 3: 5;
	return c;
}

void writeline(char* str)
{
	while (*str!='\0')	
	{
		lcd_write_data(*str);		
		str++;		
	}	
}

bool dispdraw()
{
	// check if we have colision
	if(draw())
		return true;
	
	// draw right sight
	lcd_write_ctrl(LCD_CLEAR);
	for(byte i=ROAD_LENGTH-1; i<ROAD_LENGTH; i--)
	{
		lcd_write_data(getgraphics(road2[i], false));
	}

	// go to next line
	lcd_write_ctrl(ENTER);
	
	// draw left sight
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
int incrementer;
int debounce;
int speed;
int incrementerspeed;
int points;
byte sample_count;
byte draw_timer;
volatile uint16_t sample;
bool music;

volatile byte mode;

// process controls presses
bool processbuttones()
{
	bool result = false;
	
	if(!(PINA&_BV(PA4)))
		result |= left();

	if(!(PINA&_BV(PA0)))
		result |= right();

	if(!(PINA&_BV(PA2)))
		result |= jump();

	if(debounce>0)
	{
		debounce--;
	}
	else
	{
		if(!(PINA&_BV(PA1)))
		{
			debounce = DEBOUNCE;
			result |= forward();
		}

		if(!(PINA&_BV(PA3)))
		{		
			debounce = DEBOUNCE;
			result |= backward();
		}
	}

	return result;
}

void milisecond()
{
	bool redraw = false;

	if(mode>START)
		redraw = processbuttones();

	// move every second
	if(counter++>=speed)
	{
		counter = 0;

		if(mode>START)
		{
			music = true;
			move();
			points++;
			redraw = true;
		}
		else
		{
			mode++;
			PORTA = (PORTA ^ _BV(PA6)) | ~_BV(PA6);
		}
	}
	
	if(mode>START && incrementer++>=incrementerspeed)
	{
		incrementer = 0;
		incrementerspeed++;
		if(speed>256)
			speed--;
	}	

	if(redraw&&dispdraw())
	{
		// end of the game
		mode = END;
		music_type = 1;
		PORTA |= _BV(PA6);
	}
}

// timer0 overflow 1024 times per second
ISR(TIMER0_OVF_vect) {
	
	if(draw_timer-- == 0 && mode<END)
	{
		draw_timer = 32;
		milisecond();
	}
	

	if (sample_count-- == 0) 
    {
     	sample_count = 8; 
		if(!music)
			return;

		if(music_type == 1)
		{
			music_type = 2;
			sample = 0;
		}

		if(music_type == 0)
		{      
     		OCR3B = pgm_read_byte(&pcm_samples[sample++]);
     		if(sample>pcm_length)
				sample=0;
		}

		if(music_type > 1)
		{
     		OCR3B = pgm_read_byte(&pcm_thrombone[sample++]);
     		if(sample>pcm_thromblength)
			{
				music=false;	
			}		
		}
    }

}

void gameinit()
{
	counter = 0;
	incrementer = 0;
	debounce = 0;
	mode = 0;
	incrementerspeed = 256;
	speed = 1024;
	points = 0;
	instance = random()%255;
	music_type = 0;

	init();
	dispdraw();

	sei();
}

 
/********************************************************************************
/ Main
/*******************************************************************************/
int main( void ) {
	lcd_init();
	lcd_write_ctrl(LCD_ON);
	lcd_write_ctrl(LCD_CLEAR);

	writeline("-=Night Coders=-");
	lcd_write_ctrl(ENTER);
	writeline("    presents    ");

	// set led as output
	DDRA |= _BV(PA6);
	
	//set buttons as input
	DDRA &= ~(_BV(PA0)|_BV(PA1)|_BV(PA2)|_BV(PA3)|_BV(PA4));

	/* use OC3B pin as output */
	DDRE = _BV(PE4);
	DDRE |= _BV(PE5);
	PORTE &= ~_BV(PE5);

    /* 
    * clear OC3B on compare match 
    * set OC3B at BOTTOM, non-inverting mode
    * Fast PWM, 8bit
    */
    TCCR3A = _BV(COM3B1) | _BV(WGM30);
    
    /* 
    * Fast PWM, 8bit
    * Prescaler: clk/1 = 8MHz
    * PWM frequency = 8MHz / (255 + 1) = 31.25kHz
    */
    TCCR3B = _BV(WGM32) | _BV(CS30);
    
    /* set initial duty cycle to zero */
    OCR3B = 0;

	/* Setup Timer0 */
    TCCR0|=(1<<CS00);
    TCNT0=0;
    TIMSK|=(1<<TOIE0);
    sample_count = 8;

    // set Timer0 counter initial value to 0
    //TCNT0 = 0x00;
    // start timer0 with /32 prescaler
    //TCCR0 = _BV(CS01) | _BV(CS00);
    
	mode = END;
	points = 0;
	int seed;

    while(1)
    {
		_NOP();

		if(mode == END)
		{
			_delay_ms(30000);
			TIMSK &= ~_BV(TOIE0);
			lcd_write_ctrl(LCD_CLEAR);
			PORTA &= ~_BV(PA6);
			//write score
			writeline("Press up to play");
			// go to next line
			lcd_write_ctrl(ENTER);
			writeline("Score : ");
			char buf[10];
			itoa(points, buf, 10);
			writeline(buf);


			mode = WAIT;
			while(mode==WAIT)
			{
				_NOP();
				seed++;
				if(!(PINA&_BV(PA1)))
					mode = 0;
			}

			srandom(seed);
			gameinit();
			// enable timer overflow interrupt for Timer0
		    TIMSK |= _BV(TOIE0);
		}
    }
}
