#include <stdio.h>

#include "GameController.h"
#include "Common.h"
#include <string>
#include <streambuf>
#include <iostream>
#include <sstream>

// Constructor.
GameController::GameController(int windowNumber)
{
    _originalWindow = windowNumber;
    _cursor = 0;
    _player = new Player();

    _enemyAppearInterval = 2000;
    _lastAppearTime = 0;

    _isInitialState = true;
    _isGameState = false;

    _initialAppearInterval = TIME_INITIAL_SCREEN_ALIVE;

    _statistics.LivesCount = DEFAULT_LIVES_COUNT;
    _statistics.CurrentLevel = 1;
    _statistics.PointsEarned = 0;
}

// Clears resources.
GameController::~GameController()
{
}

// Initializes current instance of the game.
void GameController::Initialize()
{
    _InitDisplay();

    glutMainLoop();

    _initialAppearInterval = TIME_INITIAL_SCREEN_ALIVE;
}

// Determines is game initialized.
bool GameController::IsInitialized()
{
    if (_isGameState && !_isInitialState)
    {
        return true;
    }

    return false;
}

// Initializes display to show game.
void GameController::_InitDisplay()
{
    //glViewport(0,0,(GLsizei) ORTHO_MAX, (GLsizei) ORTHO_MAX);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glShadeModel(GL_FLAT);

    ////Initialize OpenGL
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ////Initialize the viewport
    //glViewport(0, 0, 640, 480);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, ORTHO_MAX, 0, ORTHO_MAX, -1.0, 1.0);
    //glOrtho(0.0f, 640.0f, 480.0f, 0.0f, -2.0f, 2.0f);

    glMatrixMode(GL_MODELVIEW);

    glPointSize(3.0);

    _currentWindow = glutGetWindow();
}

// Handles key presses.
void GameController::KeyPressed(unsigned char key, int px, int py)
{
    int time = glutGet(GLUT_ELAPSED_TIME);

    switch (key)
    {
    case 27:
        glutLeaveGameMode();// TODO ?
        exit(0);
        break;
    case 'C':
    case 'c':
        _cursor = !_cursor;
        glutSetCursor(
            _cursor ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
        break;
    case 'z':
    case 'Z':
        _player->RestorePosition();
        break;
    case ' ':
        _player->MakeShot(time);
        break;
    }
}

// Handles key presses of Arrow buttons, F1, F2, etc.
void GameController::SpecialKeyPressed(int key, int x, int y)
{
    int time = glutGet(GLUT_ELAPSED_TIME);
    _player->SpecialKeyPressed(key, time);
}

// Handles key Up presses of Arrow buttons, F1, F2, etc.
void GameController::SpecialKeyUpPressed(int key, int x, int y)
{
    _player->SpecialKeyUpPressed(key);
}

// Draws all parts of the game.
void GameController::Display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (_isGameState)
    {
        _player->Draw();

        int size = _asteroids.size();
        for (int i = 0; i < size; i++)
        {
            _asteroids[i]->Draw();
        }

        size = _dust.size();
        for (int i = 0; i < size; i++)
        {
            _dust[i]->Draw();
        }

        _ShowStatistics();
    }
    else
    {
        if (_isInitialState)
        {
            _ShowInitialScreen();
        }
        // TODO
        //else
        //{
            // Show Menu.
        //}
    }

    // Switch current-original windows and do glFlush for drawing.
    glutSwapBuffers();
}

// Solves all collisions between player, asteroids and rockets.
void GameController::_SolveCollisions()
{
    int size = _asteroids.size();
    for (int i = 0; i < size; i++)
    {
        AsteroidEnemy* enemy = _asteroids[i];

        if (!enemy->HasEffectiveDamage())
        {
            continue;
        }

        // Bullets with asteroids
        bool isHit = _player->IsHit(enemy);
        // Player with asteroids
        bool isPlayerDamaged = _player->IsDamagedByEnemy(enemy);
        // Asteroids with asteroids
        // It was not implemented in the actual Asteroids. So I don't know if need this.

        // Split enemy to pieces.
        if (isHit || isPlayerDamaged)
        {
            std::vector<AsteroidEnemy*> result = enemy->SplitToPieces();

            _asteroids[i]->SetVisible(false); // This asteroid will be removed in Update.

            int addedCount = result.size();
            for (int j = 0; j < addedCount; j++)
            {
                _asteroids.push_back(result[j]);
            }
        }

        if (isPlayerDamaged)
        {
            // Set player killed.
            if (_statistics.LivesCount == 0)
            {
            }
            else
            {
                --_statistics.LivesCount;
            }
        }

        if (isHit)
        {
            _statistics.PointsEarned += 1;
        }
    }
}

// Shows initial screen.
void GameController::_ShowInitialScreen()
{
    glColor3f(0.0, 1.0, 1.0);
    //const char* text = "ASTEROIDS";
    glRasterPos2i(ORTHO_MAX / 2 - 8, ORTHO_MAX / 2);
    for(size_t i=0; i<GAME_NAME.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,(int)GAME_NAME[i]);
    }
}

// Updates logic related to asteroids.
void GameController::_UpdateAsteroids(int time)
{
    // Update, Delete, Add asteroids
    int size = _asteroids.size();
    for (int i = 0; i < size; i++)
    {
        _asteroids[i]->Update(time);
    }

    for (int i = size - 1; i >= 0; i--)
    {
        if (!_asteroids[i]->IsVisibleInViewport() || !_asteroids[i]->IsVisible())
        {
            std::vector<AsteroidEnemy*>::iterator p = _asteroids.begin();
            p += i;

            delete _asteroids[i];
            _asteroids.erase(p);
        }
    }

    if ((time - _lastAppearTime) > _enemyAppearInterval)
    {
        AsteroidEnemy* a = new AsteroidEnemy(_player->GetPosition()->x, _player->GetPosition()->y);
        _asteroids.push_back(a);
        _lastAppearTime = time;
    }

    size = _dust.size();
    for (int i = 0; i < size; i++)
    {
        _dust[i]->Update(time);
    }
}

// Updates all the game logic during OpenGL Idle time.
void GameController::Update()
{
    int time = glutGet(GLUT_ELAPSED_TIME);

    if (_isGameState)
    {
        _player->Update(time);

        _UpdateAsteroids(time);
    }
    else
    {
        if (_isInitialState)
        {
            // Show initial screen with game title during some time...
            if (time > _initialAppearInterval)
            {
                _isInitialState = false;
            }
        }
        else
        {
            // TODO Update menu.

            // Start game.
            _isGameState = true;
        }
    }

    glutPostWindowRedisplay(_currentWindow);

    _SolveCollisions();
}

// Shows current statistic information.
void GameController::_ShowStatistics()
{
    glColor3f(0.0, 1.0, 1.0);

    std::ostringstream stringStream;
    stringStream << POINTS_LABEL << _statistics.PointsEarned;
    std::string pointsValue = stringStream.str();

    glRasterPos2i(1, ORTHO_MAX - 3);
    for(size_t i=0; i<pointsValue.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)pointsValue[i]);
    }

    stringStream.str(std::string());
    stringStream << LEVEL_LABEL << _statistics.CurrentLevel;
    std::string levelValue = stringStream.str();

    glRasterPos2i(1, ORTHO_MAX - 6);
    for(size_t i=0; i<levelValue.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)levelValue[i]);
    }

    stringStream.str(std::string());
    stringStream << _statistics.LivesCount << LIVES_LABEL;
    std::string livesValue = stringStream.str();

    glRasterPos2i(ORTHO_MAX - 2 * livesValue.length(), ORTHO_MAX - 3);
    for(size_t i=0; i<livesValue.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)livesValue[i]);
    }
}