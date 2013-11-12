#pragma once
#ifndef __constants_h__
#define __constants_h__

#include <string>

const int ORTHO_MAX = 100;
const float ORTHO_MAX_D = 100.0;

const int MAX_BULLETS = 10;
const float M_PI = 3.14159265358979323846;
const float SIDE_MOVING_ACC = 0.1;

const int PENTAGON_RIBERS_NUMBER = 5;
const float DEFAULT_ASTEROID_RADIUS = 5;

const float PENTAGON_TRIANGLE_ANGLES = 72;

const int TIME_BOMBS_ALIVE = 1500;
const int TIME_INITIAL_SCREEN_ALIVE = 4000;

const int DEFAULT_LIVES_COUNT = 5;

const std::string GAME_NAME = "ASTEROIDS";
const std::string POINTS_LABEL = "Points: ";
const std::string LEVEL_LABEL = "Level: ";
const std::string LIVES_LABEL = " lives";

#endif /* __constants_h__ */