#pragma once

enum class EnemyType
{
    Type1,
    Type2,
    Type3
};

class Enemy
{
private:
    float X;
    float Y;
    float MoveSpeed;
    float MoveDirection;
    bool IsAlive;
    EnemyType Type;

public:
    Enemy();

    void SetType(EnemyType type);
    void SetPosition(float x, float y);
    void Update(float dt);

    float GetX() const;
    float GetY() const;
    bool GetIsAlive() const;
    EnemyType GetType() const;
};