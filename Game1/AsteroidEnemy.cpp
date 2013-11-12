#include "AsteroidEnemy.h"
#include "Common.h"
#include <iostream>
#include <fstream>

AsteroidEnemy::AsteroidEnemy(float playerX, float playerY)
{
    _isPiece = false;

    _isVisible = true;

    _distance = _GetInitialDistance();
    _angle = _GetRandomAngle();
    _movementType = _GetRandomTypeOfMovement();

    if (_movementType == 0)
    {
        _deltaX = ORTHO_MAX_D/2; // center of the screen
        _deltaY = ORTHO_MAX_D/2;
    }
    else if (_movementType == 1)
    {
        _deltaX = ORTHO_MAX_D/3; // not a center of the screen
        _deltaY = ORTHO_MAX_D/3;
    }
    else
    {
        _deltaX = playerX; // player position
        _deltaY = playerX;
    }

    // Initial position of the enemy.
    _x = _deltaX + _distance * cos(_angle * M_PI / 180);
    _y = _deltaY + _distance * sin(_angle * M_PI / 180);
    _pieceAngle = 0;
    _radius = DEFAULT_ASTEROID_RADIUS;

    // Initialize some number of vertexes.
    int vertexCount = _GetRandomCountOfVertexes();
    float angle = 0;
    for (int i = 0; i < vertexCount; i++)
    {
        Vertex v;
        v.x = _x + _radius * cos(angle * M_PI / 180);
        v.y = _y + _radius * sin(angle * M_PI / 180);

        angle += PENTAGON_TRIANGLE_ANGLES;

        _vertexes.push_back(v);
    }

    _isDust = false;
}

AsteroidEnemy::~AsteroidEnemy()
{
}

// 
// Draws pentagon from triangles: full pentagon if 5 triangle could be build from _vertexes array.
// If generates less than 5 vertexes, than we have not a full pentagon.
//
void AsteroidEnemy::Draw()
{
    if (!_isVisible)
    {
        return;
    }

    int size = _vertexes.size();

    // Dust.
    if (size == 1)
    {
        glBegin(GL_POINTS);
        glColor3f(0.0, 1.0, 1.0);
        glVertex2f(_vertexes[0].x, _vertexes[0].y);
        glEnd();

        glBegin(GL_POINTS);
        glColor3f(0.0, 1.0, 1.0);
        glVertex2f(_x, _y);
        glEnd();

        return;
    }

    // Shape
    for (int i = 0; i < size; i++)
    {
        Vertex v1 = _vertexes[i];

        int next = i + 1;
        if (next >= size)
        {
            // Draw last triangle ONLY if this is a full pentagon.
            if (size == PENTAGON_RIBERS_NUMBER)
            {
                next = 0;
            }
            else
            {
                continue;
            }
        }

        Vertex v2 = _vertexes[next];

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glColor3f(1.0, 0.0, 1.0);
        glBegin(GL_POLYGON);

        // Calculate visibility of first riber.
        if (_isPiece || (size != PENTAGON_RIBERS_NUMBER && i == 0))
        {
            // First riber becomes visible if shape is not full.
            glEdgeFlag(GL_TRUE);
        }
        else
        {
            // No one internal riber should be visible in the Full Pentagon.
            glEdgeFlag(GL_FALSE);
        }

        glVertex2f(_x, _y);

        // Middle riber always visible.
        glEdgeFlag(GL_TRUE);

        glVertex2f(v1.x, v1.y);

        // Calculate visibility of last riber.
        if (_isPiece || (size != PENTAGON_RIBERS_NUMBER && i == size - 2))
        {
            // First riber becomes visible if shape is not full.
            glEdgeFlag(GL_TRUE);
        }
        else
        {
            // No one internal riber should be visible in the Full Pentagon.
            glEdgeFlag(GL_FALSE);
        }

        glVertex2f(v2.x, v2.y);
        
        glEnd();
    }
    
    //glBegin(GL_POINTS);
    //glColor3f(0.0, 1.0, 1.0);
    //glVertex2f(_x, _y);
    //glEnd();
}

// 
void AsteroidEnemy::Update(int time, int delta)
{
    if (!_isVisible)
    {
        return;
    }

    delta = time - _lastTime;

    _distance -= delta * 0.01; // Set movement: change distance to the destination a bit.

    // Set rotation: change shape angle a bit.
    if (_isPiece)
    {
        // For pieces rotation a bit faster.
        _pieceAngle += 6 * delta * 0.01;
    }
    else
    {
        _pieceAngle += delta * 0.01;
    }

    _x = _deltaX + _distance * cos(_angle * M_PI / 180);
    _y = _deltaY + _distance * sin(_angle * M_PI / 180);

    int size = _vertexes.size();

    float angle = _pieceAngle;
    for (int i = 0; i < size; i++)
    {
        _vertexes[i].x = _x + _radius * cos(angle * M_PI / 180);
        _vertexes[i].y = _y + _radius * sin(angle * M_PI / 180);

        if (_isDust)
        {
            angle += PENTAGON_TRIANGLE_ANGLES/3;
        }
        else
        {
            angle += PENTAGON_TRIANGLE_ANGLES;
        }
    }

    _lastTime = time;

    // Round up vertexes.
}

//
float AsteroidEnemy::GetOuterRadius()
{
    return _radius;
}

//
int AsteroidEnemy::_GetRandomCountOfVertexes()
{
    int var = (int)_GetRandomNumber(4); // 0..3 - totally 4 variants of rectangles.

    if (var == 0)
    {
        return PENTAGON_RIBERS_NUMBER - 3;
    }
    else if (var == 1)
    {
        return PENTAGON_RIBERS_NUMBER - 2;
    }
    else if (var == 2)
    {
        return PENTAGON_RIBERS_NUMBER - 1;
    }
    else
    {
        return PENTAGON_RIBERS_NUMBER;
    }
}

//
bool AsteroidEnemy::IsVisible()
{
    return _isVisible;
}

//
void AsteroidEnemy::SetVisible(bool visibility)
{
    _isVisible = visibility;
}

//
bool AsteroidEnemy::IsVisibleInViewport()
{
    if (_x >= -30.0 && _x <= ORTHO_MAX_D + 30.0 &&
        _y >= -30.0 && _y <= ORTHO_MAX_D + 30.0)
    {
        return true;
    }

    return false;
}

//
int AsteroidEnemy::_GetTrianglesCount()
{
    int size = _vertexes.size();

    switch (size)
    {
    case 2:
        return 1;
    case 3:
        return 2;
    case 4:
        return 3;
    case 5:
        return 5;
    default:
        return 0;
    };
}

// Creates number of a triangle pieces from a figure shape.
std::vector<AsteroidEnemy*> AsteroidEnemy::SplitToPieces()
{
    std::vector<AsteroidEnemy*> pieces;

    int size = _vertexes.size();

    // Triangle is minimal figure.
    if (size <= 2 && !_isDust) // 2 vertexes + 1 center vertex.
    {
        float angle = 12;
        for (int i = 0; i < 16; i++)
        {
            AsteroidEnemy* enemy = new AsteroidEnemy(0, 0);

            enemy->_lastTime = _lastTime;
            enemy->_angle = angle + 180;

            enemy->_deltaX = _x + 150 * cos(angle * M_PI / 180);
            enemy->_deltaY = _y + 150 * sin(angle * M_PI / 180);

            float distance = sqrt(pow(abs(_x - enemy->_deltaX), 2) + pow(abs(_y - enemy->_deltaY), 2));

            enemy->_distance = distance;
            enemy->_x = _x;
            enemy->_y = _y;

            enemy->_radius = _radius;

            enemy->_isPiece = false;
            enemy->_isDust = true;
            enemy->_isVisible = true;
            enemy->_vertexes.clear();
            enemy->_pieceAngle = angle;

            Vertex v1;
            v1.x = _x + _radius * cos(angle * M_PI / 180);
            v1.y = _y + _radius * sin(angle * M_PI / 180);
            enemy->_vertexes.push_back(v1);

            pieces.push_back(enemy);

            angle += PENTAGON_TRIANGLE_ANGLES / 3;
        }

        return pieces;
    }

    float angle = 36;
    int count = _GetTrianglesCount();
    for (int i = 0; i < count; i++)
    {
        AsteroidEnemy* enemy = new AsteroidEnemy(0, 0);

        enemy->_lastTime = _lastTime;
        enemy->_angle = angle + 180;

        enemy->_deltaX = _x + 150 * cos(angle * M_PI / 180);
        enemy->_deltaY = _y + 150 * sin(angle * M_PI / 180);

        float distance = sqrt(pow(abs(_x - enemy->_deltaX), 2) + pow(abs(_y - enemy->_deltaY), 2));

        enemy->_distance = distance;
        enemy->_x = _x;
        enemy->_y = _y;

        enemy->_radius = _radius;

        enemy->_isPiece = true;
        enemy->_isVisible = true;
        enemy->_vertexes.clear();
        enemy->_pieceAngle = angle;

        Vertex v1;
        v1.x = _vertexes[i].x;
        v1.y = _vertexes[i].y;
        enemy->_vertexes.push_back(v1);

        int next = i + 1;
        if (next == size)
        {
            next = 0;
        }

        Vertex v2;
        v2.x = _vertexes[next].x;
        v2.y = _vertexes[next].y;
        enemy->_vertexes.push_back(v2);

        pieces.push_back(enemy);

        angle += PENTAGON_TRIANGLE_ANGLES;
    }

    return pieces;
}

///
bool AsteroidEnemy::HasEffectiveDamage()
{
    return !_isDust;
}