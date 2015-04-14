#include "game.h"

void init()
{
	car_left = true;
	car_position = 1;
  	car_jump = 0;
	lines_next = false;
	// fill road with empty space and lines
	for(byte i=0; i<ROAD_LENGTH; i++)
	{
		map1[i] = lines_next ? _BV(ROAD_LINE) : 0;
		map2[i] = lines_next ? _BV(ROAD_LINE) : 0;
		lines_next = !lines_next;
	}
	last = ROAD_LENGTH-1;
}

bool check_colision(byte i)
{
	if(road1[i]&(_BV(CAR_FRONT)|_BV(CAR_BACK))>0 && road1[i]>(_BV(OBSTACLE1)|_BV(CAR_JUMP)))
		return true;

	if(road2[i]&(_BV(CAR_FRONT)|_BV(CAR_BACK))>0 && road2[i]>(_BV(OBSTACLE1)|_BV(CAR_JUMP)))
		return true;

	return false;
}

bool draw()
{
	bool result = false;
	for(byte i = 0; i<ROAD_LENGTH; i++)
	{
		byte k = (last+1+i)%ROAD_LENGTH;
		road1[i] = map1[k];
		road2[i] = map2[k];
		if(i == car_position)
		{
			byte jump = car_jump==0 ? 0 : _BV(CAR_JUMP);

			if(car_left)
			{
				road1[i] |= _BV(CAR_FRONT) | jump;
				road1[i-1] |= _BV(CAR_BACK) | jump;
			}
			else
			{
				road2[i] |= _BV(CAR_FRONT) | jump;
				road2[i-1] |= _BV(CAR_BACK) | jump;				
			}
			result |= check_colision(i) | check_colision(i-1);	
		}
	}
	return result;
}

bool right()
{
	if(car_left)
	{
		car_left = false;
		return true;
	}
	return false;
}

bool left()
{
	if(!car_left)
	{
		car_left = true;
		return true;
	}
	return false;
}

bool forward()
{
	if(car_position<ROAD_LENGTH-1)
	{
		car_position++;
		return true;
	}
	return false;
}

bool backward()
{
	if(car_position>1)
	{
		car_position--;
		return true;
	}
	return false;
}

bool jump()
{
	if(car_jump == 0)
	{
		car_jump = JUMP_LENGTH;
		return true;
	}
	return false;
}

void move ()
{
	// move pointer
	last = (last+1)%ROAD_LENGTH;

	if(car_jump>0)
		car_jump--;

	// generate lines
	map1[last] = lines_next ? _BV(ROAD_LINE) : 0;
	map2[last] = lines_next ? _BV(ROAD_LINE) : 0;

	// change lines generation status
	lines_next = !lines_next;
	
	// generate obstacles	
}
