#pragma once
#include "Collision.h"

enum class EnemyType
{
    Type1,
    Type2,
    Type3
};

enum class EnemyState
{
    Idle,
    Diving,
    Looping,
    Beaming,
    Capturing,
    Returning
};

class Enemy
{
private:
    float X;
    float Y;
    float FormationX;
    float FormationY;
    float MoveSpeed;
    float MoveDirection;
    bool IsAlive;
    EnemyType Type;
    int Health;
    EnemyState State;
    float Timer;
    float ShootTimer;
    bool WantsToShoot;
    float DiveStartX;
    float DiveStartY;
    bool IsBeaming;
    float BeamScale;
    float BeamTimer;
    bool IsPlayerCaptured;
    bool HasCapturedShipVisual;

    float MoveSpeedScale;
    float ShootCooldownScale;
    float AttackDelayScale;

public:
    Enemy();

    void SetType(EnemyType type);
    void SetPosition(float x, float y);
    void Update(float dt, float playerX = 0.0f, float playerY = 0.0f);

    void SetMoveSpeedScale(float value);
    void SetShootCooldownScale(float value);
    void SetAttackDelayScale(float value);

    float GetX() const;
    float GetY() const;
    bool GetIsAlive() const;
    EnemyType GetType() const;
    bool GetWantsToShoot() const;
    void ClearWantsToShoot();
    float GetRadius() const;

    void StartDive();
    void TakeDamage();
    int GetHealth() const;
    bool GetIsBeaming() const;
    float GetBeamScale() const;
    bool GetIsPlayerCaptured() const;
    bool GetHasCapturedShipVisual() const;
    void ReleasePlayer();

    HitBox GetHitBox() const;

private:
    void UpdateFormation(float dt);
    void UpdateIdle(float dt);
    void UpdateDive(float dt, float playerX, float playerY);
    void UpdateLoop(float dt);
    void UpdateBeaming(float dt, float playerX, float playerY);
    void UpdateCapturing(float dt);
    void UpdateReturn(float dt);
};