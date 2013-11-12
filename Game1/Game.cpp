
#include <string.h>
#include "Game.h"
#include "Common.h"

/// 
/// 
/// 
Game* Game::_pInstance =  0;

/// 
/// 
/// 
Game::Game(int windowNumber)
{
    _pController = new GameController(windowNumber);
}

/// 
/// 
/// 
Game::~Game()
{
    if (_pController != 0)
    {
        delete _pController;
        _pController = 0;
    }
}

/// 
/// 
/// 
bool Game::NewGame(int argc, char **argv)
{
    // Don't let halt current Game. Only if user knows what he does by hald_Game().
    if (_pInstance == 0 && IsGameFinished())
    {
        _pInstance = new Game(_InitDisplay(argc,argv));

        bool result = _pInstance != 0;

        if (result)
        {
            _InitHandlers();

            _pInstance->_pController->Initialize();
        }

        return result;
    }
    else
    {
        return false;
    }
}

/// 
/// 
/// 
int Game::_InitDisplay(int argc, char **argv)
{
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

    if (argc > 1 && !strcmp(argv[1], "-fullscreen"))
    {
        // Simply create window in fullscreen mode.
        glutGameModeString("640x480:16@60");
        return glutEnterGameMode(); // Need return? Or just 0?
    }
    else
    {
        glutInitWindowSize(640,480);

        return glutCreateWindow("Asteroids");
    }

}

//
// Links handlers.
//
void Game::_InitHandlers()
{
    glutIgnoreKeyRepeat(1);

    glutKeyboardFunc(&Game::KeyPressed);
    glutSpecialFunc(&Game::SpecialKeyPressed);
    glutSpecialUpFunc(&Game::SpecialKeyUpPressed);
    glutDisplayFunc(&Game::Display);
    glutVisibilityFunc(&Game::Visible);
}

/// 
/// 
/// 
bool Game::IsGameFinished()
{
    if (_pInstance != 0 && _pInstance->_pController != 0)
    {
        return !_pInstance->_pController->IsInitialized();
    }

    return true;
}

/// 
/// 
/// 
bool Game::HaltGame()
{
    bool result = _pInstance != 0;

    if (result)
    {
        delete _pInstance;
        _pInstance = 0;
    }

    return result;
}

/// 
/// 
/// 
void Game::KeyPressed(unsigned char key, int px, int py)
{
    _pInstance->_pController->KeyPressed(key, px, py);
}

/// 
/// 
/// 
void Game::SpecialKeyPressed(int key, int x, int y)
{
    _pInstance->_pController->SpecialKeyPressed(key, x, y);
}

/// 
/// 
/// 
void Game::SpecialKeyUpPressed(int key, int x, int y)
{
    _pInstance->_pController->SpecialKeyUpPressed(key, x, y);
}

/// 
/// 
/// 
void Game::Display()
{
    _pInstance->_pController->Display();
}

/// 
/// 
/// 
void Game::Update()
{
    _pInstance->_pController->Update();
}

/// 
/// 
/// 
void Game::Visible(int vis)
{
    if (vis == GLUT_VISIBLE)
    {
        //if (!paused) {
            glutIdleFunc(&Game::Update);
        //}
    }
    else
    {
        glutIdleFunc(NULL);
    }
}
