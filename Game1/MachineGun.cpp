
#include "MachineGun.h"
#include "Common.h"

MachineGun::MachineGun()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        Bullet b;
        _bullets.push_back(b);
    }
}

// 
void MachineGun::Draw()
{
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 1.0);

    size_t count = _bullets.size();
    for (size_t i=0; i<count; i++)
    {
        if (_bullets[i].inuse)
        {
            glVertex2f(_bullets[i].x, _bullets[i].y);
        }
    }

    glEnd();
}

// 
void MachineGun::Update(int time, int delta)
{
    size_t count = _bullets.size();
    for (size_t i=0; i<count; i++)
    {
        if (_bullets[i].inuse)
        {
            if (time > _bullets[i].expire)
            {
                _bullets[i].inuse = 0;
                continue;
            }
            float x = _bullets[i].x + _bullets[i].xv * delta;
            float y = _bullets[i].y + _bullets[i].yv * delta;
            x = x / ORTHO_MAX_D;
            _bullets[i].x = (x - floor(x))*ORTHO_MAX_D;
            y = y / ORTHO_MAX_D;
            _bullets[i].y = (y - floor(y))*ORTHO_MAX_D;
        }
    }
}

//
void MachineGun::MakeShot(int time, Position playerPosition)
{
    int bulletId = _GetBulletToShow();
    if (bulletId >= 0)
    {
        _InitBullet(bulletId, time, playerPosition);
    }
}

//
int MachineGun::_GetBulletToShow()
{
    size_t count = _bullets.size();
    for (size_t i=0; i<count; i++)
    {
        if (!_bullets[i].inuse)
        {
            return i;
        }
    }

    return -1;
}

//
void MachineGun::_InitBullet(int i, int time, Position playerPosition)
{
    float c = cos(playerPosition.angle*M_PI/180.0);
    float s = sin(playerPosition.angle*M_PI/180.0);

    _bullets[i].inuse = 1;
    _bullets[i].x = playerPosition.x + 2 * c;
    _bullets[i].y = playerPosition.y + 2 * s;
    _bullets[i].v = 0.025;
    _bullets[i].xv = playerPosition.xv + c * _bullets[i].v;
    _bullets[i].yv = playerPosition.yv + s * _bullets[i].v;
    _bullets[i].expire = time + TIME_BOMBS_ALIVE;
}

//
bool MachineGun::IsDamagedByBullets(AsteroidEnemy* enemy)
{
    if (enemy != 0)
    {
        size_t count = _bullets.size();
        for (size_t i=0; i<count; i++)
        {
            if (_bullets[i].inuse == 1)
            {
                float enemyX = enemy->GetX();
                float enemyY = enemy->GetY();

                // Find intersection
                float r = sqrt(pow(abs(_bullets[i].x - enemyX), 2) + pow(abs(_bullets[i].y - enemyY), 2));

                if (r <= enemy->GetOuterRadius())
                {
                    // TODO
                    _bullets[i].inuse = 0;

                     return true;
                }
            }
        }
    }

    return false;
}