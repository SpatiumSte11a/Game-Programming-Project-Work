#include "Enemy.h"

Enemy::Enemy()
    : X(0.0f),
    Y(0.70f),
    MoveSpeed(0.35f),
    MoveDirection(1.0f),
    IsAlive(true),
    Type(EnemyType::Type1)
{
}

void Enemy::SetType(EnemyType type)
{
    Type = type;

    switch (Type)
    {
    case EnemyType::Type1:
        MoveSpeed = 0.35f;
        break;

    case EnemyType::Type2:
        MoveSpeed = 0.35f;
        break;

    case EnemyType::Type3:
        MoveSpeed = 0.35f;
        break;
    }
}

void Enemy::SetPosition(float x, float y)
{
    X = x;
    Y = y;
}

void Enemy::Update(float dt)
{
    if (!IsAlive)
        return;

    X += MoveDirection * MoveSpeed * dt;

    if (X > 0.85f)
    {
        X = 0.85f;
        MoveDirection = -1.0f;
    }

    if (X < -0.85f)
    {
        X = -0.85f;
        MoveDirection = 1.0f;
    }
}

float Enemy::GetX() const
{
    return X;
}

float Enemy::GetY() const
{
    return Y;
}

bool Enemy::GetIsAlive() const
{
    return IsAlive;
}

EnemyType Enemy::GetType() const
{
    return Type;
}