#pragma once
#ifndef __machinegun_h__
#define __machinegun_h__

#include <vector>
#include "Bullet.h"
#include "GraphicObject.h"
#include "Position.h"
#include "AsteroidEnemy.h"

/// 
/// 
/// 
class MachineGun : public GraphicObject
{
public:
    MachineGun();
    virtual void Draw();
    virtual void Update(int time, int delta);
    void MakeShot(int time, Position playerPosition);
    bool IsDamagedByBullets(AsteroidEnemy* enemy);

private:
    int _GetBulletToShow(void);
    void _InitBullet(int i, int time, Position playerPosition);

    std::vector<Bullet> _bullets;
};

#endif /* __machinegun_h__ */

