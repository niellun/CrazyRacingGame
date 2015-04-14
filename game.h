#if !defined(GAME_H)
#define GAME_H

/********************************************************************************
/ Includes
/*******************************************************************************/
#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>

/********************************************************************************
/ Types
/*******************************************************************************/
typedef unsigned char byte;

/********************************************************************************
/ Defines
/*******************************************************************************/
#define ROAD_LENGTH 16

#define ROAD_LINE 0
#define CAR_FRONT 1
#define CAR_BACK 2
#define OBSTACLE1 3
#define OBSTACLE2 4
#define OBSTACLE3 5
#define OBSTACLE4 6
#define CAR_JUMP 7

#define JUMP_LENGTH 4
#define NO_JUMP 2

/********************************************************************************
/ Variables
/*******************************************************************************/
byte road1[ROAD_LENGTH];
byte road2[ROAD_LENGTH];

byte map1[ROAD_LENGTH];
byte map2[ROAD_LENGTH];

bool lines_next;
byte last;

bool car_left;
byte car_position;
byte car_jump;
byte no_jump;


static byte obstacle = _BV(OBSTACLE1)+_BV(CAR_JUMP);

/********************************************************************************
/ Functions
/*******************************************************************************/
void init();
bool draw();
bool right();
bool left();
bool forward();
bool backward();
bool jump();
void move ();

#endif
