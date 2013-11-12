#pragma once
#ifndef __asteroidenemy_h__
#define __asteroidenemy_h__

#include <vector>
#include "Vertex.h"
#include "GraphicObject.h"
#include "BaseEnemy.h"
#include "Position.h"

class AsteroidEnemy : public BaseEnemy, public GraphicObject
{
public:
    //
    AsteroidEnemy(float playerX, float playerY);
    //
    virtual ~AsteroidEnemy();
    //
    virtual void Draw();
    // 
    virtual void Update(int time, int delta = 0);
    //
    virtual float GetOuterRadius();

    bool IsVisible();
    void SetVisible(bool visibility);

    bool IsVisibleInViewport();

    std::vector<AsteroidEnemy*> SplitToPieces();

    bool HasEffectiveDamage();

private:
    int _GetRandomCountOfVertexes();
    int _GetTrianglesCount();

    std::vector<Vertex> _vertexes;
    float _radius;
    bool _isVisible;

    bool _isDust;

    bool _isPiece;
    float _deltaX;
    float _deltaY;

    float _pieceAngle;

    int shapeType;
};

#endif /* __asteroidenemy_h__ */
