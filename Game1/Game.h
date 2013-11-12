#pragma once
#ifndef __game_h__
#define __game_h__

#include "GameController.h"

// 
// 
// 
class Game
{
public:
    // 
    Game(int windowNumber);
    // 
    virtual ~Game();

    // 
    static bool NewGame(int argc, char **argv);
    // 
    static bool IsGameFinished();
    // 
    static bool HaltGame();

    // 
    static void KeyPressed(unsigned char key, int px, int py);
    //
    static void SpecialKeyPressed(int key, int x, int y);
    //
    static void SpecialKeyUpPressed(int key, int x, int y);
    //
    static void Display();
    //
    static void Update();
    //
    static void Visible(int vis);

private:
    //
    static int _InitDisplay(int argc, char **argv);
    //
    static void _InitHandlers();

    //
    static Game* _pInstance;
    //
    GameController* _pController;
};

#endif /* __game_h__ */
