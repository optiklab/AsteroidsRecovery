#pragma once
#ifndef __enemydust_h__
#define __enemydust_h__

#include "GraphicObject.h"
#include "BaseEnemy.h"

// 
// 
// 
class EnemyDust : public BaseEnemy, public GraphicObject
{
public:
    // 
    EnemyDust(void);
    // 
    virtual ~EnemyDust();
    // 
    virtual void Draw();
    // 
    virtual void Update(int time, int delta = 0);
    //
    virtual float GetOuterRadius();
};

#endif /* __enemydust_h__ */