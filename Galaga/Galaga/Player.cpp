#include "Player.h"

Player::Player()
    : X(0.0f),
    Y(-0.80f),
    MoveSpeed(0.9f)
{
}

void Player::Update(float dt, bool moveLeft, bool moveRight)
{
    if (moveLeft)
        X -= MoveSpeed * dt;

    if (moveRight)
        X += MoveSpeed * dt;

    if (X < -0.9f)
        X = -0.9f;

    if (X > 0.9f)
        X = 0.9f;
}

float Player::GetX() const
{
    return X;
}

float Player::GetY() const
{
    return Y;
}

HitBox Player::GetHitBox() const
{
    HitBox box = {};
    box.X = X;
    box.Y = Y;
    box.HalfWidth = 0.04f;
    box.HalfHeight = 0.05f;
    return box;
}