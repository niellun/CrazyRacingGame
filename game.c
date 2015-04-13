#include "game.h"

void init()
{
	lines_next = false;
	// fill road with empty space and lines
	for(byte i=0; i<ROAD_LENGTH; i++)
	{
		road1[i] = lines_next ? _BV(ROAD_LINE) : 0;
		road2[i] = lines_next ? _BV(ROAD_LINE) : 0;
	}
}

bool check_colision(byte i)
{
	if(road1[i]&(_BV(CAR_FRONT)|_BV(CAR_BACK))>0 && road1[i]>(_BV(OBSTACLE1)|_BV(CAR_JUMP)))
		return true;

	if(road2[i]&(_BV(CAR_FRONT)|_BV(CAR_BACK))>0 && road2[i]>(_BV(OBSTACLE1)|_BV(CAR_JUMP)))
		return true;

	return false;
}


bool right()
{
	bool result = false;
	cli();
	for(byte i=0; i<ROAD_LENGTH; i++)
	{
		if(road1[i]&_BV(CAR_FRONT))
		{
			road1[i] &= ~_BV(CAR_FRONT);
			road2[i] |= _BV(CAR_FRONT);
			result |= check_colision(i);
		}
		if(road1[i]&_BV(CAR_BACK))
		{
			road1[i] &= ~_BV(CAR_BACK);
			road2[i] |= _BV(CAR_BACK);
			result |= check_colision(i);
		}
	}
	sei();
	return result;
}

bool left()
{
	bool result = false;
	cli();
	for(byte i=0; i<ROAD_LENGTH; i++)
	{
		if(road2[i]&_BV(CAR_FRONT))
		{
			road2[i] &= ~_BV(CAR_FRONT);
			road1[i] |= _BV(CAR_FRONT);
			result |= check_colision(i);
		}
		if(road2[i]&_BV(CAR_BACK))
		{
			road2[i] &= ~_BV(CAR_BACK);
			road1[i] |= _BV(CAR_BACK);
			result |= check_colision(i);
		}
	}
	sei();
	return result;
}

bool forward()
{
	bool result = false;
	cli();
	for(byte i=1; i<ROAD_LENGTH-1; i++)
	{
		if(road1[i]&_BV(CAR_FRONT))
		{
			road1[i] &= ~_BV(CAR_FRONT);
			road1[i-1] &= ~_BV(CAR_BACK);
			road1[i+1] |= _BV(CAR_FRONT);
			road1[i] |= _BV(CAR_BACK);
			result |= check_colision(i);
		}
		if(road2[i]&_BV(CAR_FRONT))
		{
			road2[i] &= ~_BV(CAR_FRONT);
			road2[i-1] &= ~_BV(CAR_BACK);
			road2[i+1] |= _BV(CAR_FRONT);
			road2[i] |= _BV(CAR_BACK);
			result |= check_colision(i);
		}
	}
	sei();
	return result;
}

bool backward()
{
	bool result = false;
	cli();
	for(byte i=2; i<ROAD_LENGTH; i++)
	{
		if(road1[i]&_BV(CAR_FRONT))
		{
			road1[i] &= ~_BV(CAR_FRONT);
			road1[i-1] &= ~_BV(CAR_BACK);
			road1[i-1] |= _BV(CAR_FRONT);
			road1[i-2] |= _BV(CAR_BACK);
			result |= check_colision(i);
		}
		if(road2[i]&_BV(CAR_FRONT))
		{
			road2[i] &= ~_BV(CAR_FRONT);
			road1[i-1] &= ~_BV(CAR_BACK);
			road2[i-1] |= _BV(CAR_FRONT);
			road2[i-2] |= _BV(CAR_BACK);
			result |= check_colision(i);
		}
	}
	sei();
	return result;
}

void jump()
{
	
}

bool move ()
{
}
