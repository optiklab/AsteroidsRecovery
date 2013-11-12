#pragma once
#ifndef __baseenemy_h__
#define __baseenemy_h__

// 
// Base enemy object with necessary common functions and fields.
// 
class BaseEnemy
{
public:
    BaseEnemy();

    float GetX();
    float GetY();

    //
    virtual float GetOuterRadius() = 0;

protected:
    // 
    float _GetInitialDistance();
    // 
    float _GetRandomAngle();
    // 
    int _GetRandomTypeOfMovement();
    //
    float _GetRandomNumber(int max);
    // 
    float _angle;
    float _distance;
    float _x;
    float _y;
    int _lastTime;
    int _movementType;
};

#endif /* __baseenemy_h__ */