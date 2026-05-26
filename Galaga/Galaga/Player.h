#pragma once
#include "Collision.h"

class Player
{
private:
    float X;
    float Y;
    float MoveSpeed;

public:
    Player();

    void Update(float dt, bool moveLeft, bool moveRight);

    float GetX() const;
    float GetY() const;
    HitBox GetHitBox() const;
};