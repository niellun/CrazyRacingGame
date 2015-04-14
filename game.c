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

bool draw()
{
	for(byte i = 0; i<ROAD_LENGTH; i++)
	{
		byte k = (last+1+i)%ROAD_LENGTH;
		road1[i] = map1[k];
		road2[i] = map2[k];
		if(i == car_position)
		{
			byte jump = car_jump==0 ? _BV(CAR_JUMP) : 0;

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
		}
	}
	if(car_left)
	{
		return road1[car_position]>obstacle || road1[car_position-1]>obstacle;
	}
	else
	{
		return road2[car_position]>obstacle || road2[car_position-1]>obstacle;
	}
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
	if(car_jump == 0 && no_jump == 0)
	{
		car_jump = JUMP_LENGTH;
		no_jump = JUMP_LENGTH + NO_JUMP;
		return true;
	}
	return false;
}

void generateobstacles()
{
	if(random()/3>random())
	{
		map1[last]|=_BV(OBSTACLE1);
		return;
	}

	if(random()/4>random())
	{
		map2[last]|=_BV(OBSTACLE1);
		return;
	}

	if(random()/4>random())
	{
		map1[last]|=_BV(OBSTACLE2);
		return;
	}

	if(random()/3>random())
	{
		map2[last]|=_BV(OBSTACLE2);
		return;
	}
}

void move ()
{
	// move pointer
	last = (last+1)%ROAD_LENGTH;

	if(car_jump>0)
		car_jump--;
	if(no_jump>0)
		no_jump--;

	// generate lines
	map1[last] = lines_next ? _BV(ROAD_LINE) : 0;
	map2[last] = lines_next ? _BV(ROAD_LINE) : 0;

	// generate obstacles
	if(!lines_next)
	{	
		generateobstacles();
	}

	// change lines generation status
	lines_next = !lines_next;
	
}
