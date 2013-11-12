
#include "EnemyDust.h"
#include "Common.h"

EnemyDust::EnemyDust()
{
    _distance = _GetInitialDistance();
    _angle = _GetRandomAngle();
    _movementType = _GetRandomTypeOfMovement();

    _x = ORTHO_MAX_D/2 + _distance * cos(_angle * M_PI / 180);
    _y = ORTHO_MAX_D/2 + _distance * sin(_angle * M_PI / 180);

    _lastTime = 0;
}

EnemyDust::~EnemyDust()
{
}


// 
void EnemyDust::Draw()
{
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 1.0);

    glVertex2f(_x, _y);

    glEnd();
}

// 
void EnemyDust::Update(int time, int delta)
{
    delta = time - _lastTime;

    _distance -= delta * 0.01;

    float deltaX, deltaY;

    if (_movementType == 0)
    {
        deltaX = ORTHO_MAX_D/2;
        deltaY = ORTHO_MAX_D/2;
    }
    else if (_movementType == 1)
    {
        deltaX = ORTHO_MAX_D/3;
        deltaY = ORTHO_MAX_D/3;
    }
    else
    {
        deltaX = ORTHO_MAX_D/3;
        deltaY = ORTHO_MAX_D/3;
    }

    _x = deltaX + _distance * cos(_angle * M_PI / 180);
    _y = deltaY + _distance * sin(_angle * M_PI / 180);

    _lastTime = time;
}

//
float EnemyDust::GetOuterRadius()
{
    return 1;
}