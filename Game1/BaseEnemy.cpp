
#include "BaseEnemy.h"
#include "Common.h"

//
BaseEnemy::BaseEnemy()
{
    _angle = 0;
    _distance = 0;
    _x = 0;
    _y = 0;
    _lastTime = glutGet(GLUT_ELAPSED_TIME);
    _movementType = 0;
}

//
float BaseEnemy::_GetInitialDistance()
{
    return sqrt(2*pow(ORTHO_MAX_D/2, 2));
}

//
float BaseEnemy::_GetRandomAngle()
{
    return _GetRandomNumber(360);
}

//
int BaseEnemy::_GetRandomTypeOfMovement()
{
    return (int)_GetRandomNumber(3); // 3 types of movement
}

//
float BaseEnemy::_GetRandomNumber(int max)
{
    static int inc = 10;
    srand(time(NULL) - inc);

    float result = rand() % max;

    inc -= result * 10;

    return result;
}

//
float BaseEnemy::GetX()
{
    return _x;
}

//
float BaseEnemy::GetY()
{
    return _y;
}