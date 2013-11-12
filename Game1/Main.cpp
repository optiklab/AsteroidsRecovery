#include "Game.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// GraphicObject: Draw, Show...
// EnemyBase, Triangle, Rectangle, ...
//MachineGun, Laser, Rocket

// Factory: random generating of enemies.

/////////////////////////////////////////////////////////////////////////////////////////////////


// Game Regime: menu, Game

// Keys handler
// LevelGenerator... ?

// GraphicObject: Draw, Show...
// EnemyBase, Triangle, Rectangle, ...
// WeaponBase, MachineGun, Laser, Rocket

int main(int argc, char **argv)
{
    if (!Game::NewGame(argc, argv))
    {
        return -1;
    }

    if (!Game::HaltGame())
    {
        return -1;
    }

    return 0;
}