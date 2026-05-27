#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

Enemy::Enemy()
    : X(0.0f),
    Y(0.70f),
    FormationX(0.0f),
    FormationY(0.70f),
    MoveSpeed(0.35f),
    MoveDirection(1.0f),
    IsAlive(true),
    Type(EnemyType::Type1),
    Health(1),
    State(EnemyState::Idle),
    Timer(0.0f),
    ShootTimer(0.0f),
    WantsToShoot(false),
    DiveStartX(0.0f),
    DiveStartY(0.0f),
    IsBeaming(false),
    BeamScale(0.0f),
    BeamTimer(0.0f),
    IsPlayerCaptured(false),
    HasCapturedShipVisual(false)
{
    ShootTimer = 0.5f + (static_cast<float>(rand()) / RAND_MAX) * 2.0f;
}

void Enemy::SetType(EnemyType type)
{
    Type = type;

    switch (Type)
    {
    case EnemyType::Type1:
    case EnemyType::Type2:
        MoveSpeed = 0.35f;
        Health = 1;
        break;

    case EnemyType::Type3:
        MoveSpeed = 0.35f;
        Health = 2;
        break;
    }
}

void Enemy::SetPosition(float x, float y)
{
    X = x;
    Y = y;
    FormationX = x;
    FormationY = y;
}

void Enemy::UpdateFormation(float dt)
{
    FormationX += MoveDirection * MoveSpeed * dt;

    if (FormationX > 0.85f)
    {
        FormationX = 0.85f;
        MoveDirection = -1.0f;
    }

    if (FormationX < -0.85f)
    {
        FormationX = -0.85f;
        MoveDirection = 1.0f;
    }
}

void Enemy::Update(float dt, float playerX, float playerY)
{
    if (!IsAlive)
        return;

    UpdateFormation(dt);

    switch (State)
    {
    case EnemyState::Idle:
        UpdateIdle(dt);
        break;

    case EnemyState::Diving:
        UpdateDive(dt, playerX, playerY);
        break;

    case EnemyState::Beaming:
        UpdateBeaming(dt, playerX, playerY);
        break;

    case EnemyState::Capturing:
        UpdateCapturing(dt);
        break;

    case EnemyState::Looping:
        UpdateLoop(dt);
        break;

    case EnemyState::Returning:
        UpdateReturn(dt);
        break;
    }
}

void Enemy::UpdateIdle(float dt)
{
    X = FormationX;
    Y = FormationY;

    Timer += dt;

    float triggerTime = (Type == EnemyType::Type3) ? 8.0f : 6.0f;
    if (Timer > triggerTime)
    {
        StartDive();
    }
}

void Enemy::StartDive()
{
    State = EnemyState::Diving;
    Timer = 0.0f;
    DiveStartX = X;
    DiveStartY = Y;
    IsBeaming = false;
    IsPlayerCaptured = false;
}

void Enemy::UpdateDive(float dt, float playerX, float playerY)
{
    Timer += dt;

    if (Type == EnemyType::Type1)
    {
        float t = std::min(Timer * 0.6f, 1.0f);

        float p0x = DiveStartX;
        float p0y = DiveStartY;
        float p1x = DiveStartX + (DiveStartX > 0.0f ? 0.4f : -0.4f);
        float p1y = -0.2f;
        float p2x = 0.0f;
        float p2y = -0.6f;

        X = (1 - t) * (1 - t) * p0x + 2 * (1 - t) * t * p1x + t * t * p2x;
        Y = (1 - t) * (1 - t) * p0y + 2 * (1 - t) * t * p1y + t * t * p2y;

        if (t >= 1.0f)
        {
            State = EnemyState::Looping;
            Timer = 0.0f;
        }
    }
    else if (Type == EnemyType::Type2)
    {
        Y = DiveStartY - 0.75f * Timer;
        X = DiveStartX + sinf(Timer * 10.0f) * 0.35f;

        if (Y <= -1.0f)
        {
            State = EnemyState::Returning;
            Timer = 0.0f;
        }
    }
    else if (Type == EnemyType::Type3)
    {
        float t = std::min(Timer / 1.5f, 1.0f);

        X = DiveStartX * (1.0f - t);
        Y = DiveStartY * (1.0f - t) + 0.0f * t;

        if (t >= 1.0f)
        {
            X = 0.0f;
            Y = 0.0f;
            State = EnemyState::Beaming;
            BeamTimer = 0.0f;
            BeamScale = 0.0f;
            IsBeaming = true;
        }
    }

    ShootTimer -= dt;

    if (ShootTimer <= 0.0f)
    {
        WantsToShoot = true;
        float cooldown = (Type == EnemyType::Type2) ? 0.8f : 1.5f;
        ShootTimer = cooldown + (static_cast<float>(rand()) / RAND_MAX) * 1.0f;
    }
}

void Enemy::UpdateBeaming(float dt, float playerX, float playerY)
{
    BeamTimer += dt;
    IsBeaming = true;

    if (BeamScale < 1.0f)
        BeamScale += dt * 0.5f;

    float beamWidth = 0.15f * BeamScale;

    if (fabsf(playerX - X) < beamWidth && playerY < Y)
    {
        IsPlayerCaptured = true;
        HasCapturedShipVisual = true;
        IsBeaming = false;
        State = EnemyState::Capturing;
        Timer = 0.0f;
        return;
    }

    if (BeamTimer > 4.0f)
    {
        IsBeaming = false;
        State = EnemyState::Looping;
        Timer = 0.0f;
        DiveStartX = X;
        DiveStartY = Y;
    }
}

void Enemy::UpdateCapturing(float dt)
{
    Timer += dt;
    float t = std::min(Timer / 2.0f, 1.0f);

    Y = DiveStartY * (1.0f - t) + FormationY * t;
    X = DiveStartX * (1.0f - t) + FormationX * t;

    if (t >= 1.0f)
    {
        State = EnemyState::Idle;
        Timer = 0.0f;
    }
}

void Enemy::UpdateLoop(float dt)
{
    Timer += dt * 5.0f;

    float radius = (Type == EnemyType::Type3) ? 0.4f : 0.18f;
    float centerX = 0.0f;
    float centerY = (Type == EnemyType::Type3) ? -0.4f : -0.6f - radius;

    X = centerX + radius * sinf(Timer);
    Y = centerY + radius * cosf(Timer);

    if (Timer >= 6.28318f)
    {
        State = EnemyState::Returning;
        Timer = 0.0f;
        DiveStartX = X;
        DiveStartY = Y;
    }
}

void Enemy::UpdateReturn(float dt)
{
    if (Y > -1.1f && State == EnemyState::Returning && Timer == 0.0f)
    {
        Y -= MoveSpeed * 2.5f * dt;

        if (Y <= -1.1f)
        {
            Y = 1.1f;
            X = FormationX;
            Timer = 1.0f;
            DiveStartX = X;
            DiveStartY = Y;
        }
    }
    else if (Timer > 0.0f)
    {
        float t = std::min((Timer - 1.0f) * 2.0f, 1.0f);
        Timer += dt;

        X = DiveStartX * (1.0f - t) + FormationX * t;
        Y = DiveStartY * (1.0f - t) + FormationY * t;

        if (t >= 1.0f)
        {
            State = EnemyState::Idle;
            Timer = 0.0f;
        }
    }
}

void Enemy::TakeDamage()
{
    if (!IsAlive)
        return;

    Health--;

    if (Health <= 0)
    {
        Health = 0;
        IsAlive = false;
        WantsToShoot = false;
        IsBeaming = false;
        IsPlayerCaptured = false;
        HasCapturedShipVisual = false;
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

bool Enemy::GetWantsToShoot() const
{
    return WantsToShoot;
}

void Enemy::ClearWantsToShoot()
{
    WantsToShoot = false;
}

float Enemy::GetRadius() const
{
    return (Type == EnemyType::Type3) ? 0.09f : 0.06f;
}

int Enemy::GetHealth() const
{
    return Health;
}

bool Enemy::GetIsBeaming() const
{
    return IsBeaming;
}

float Enemy::GetBeamScale() const
{
    return BeamScale;
}

bool Enemy::GetIsPlayerCaptured() const
{
    return IsPlayerCaptured;
}

bool Enemy::GetHasCapturedShipVisual() const
{
    return HasCapturedShipVisual;
}

void Enemy::ReleasePlayer()
{
    IsPlayerCaptured = false;
}

HitBox Enemy::GetHitBox() const
{
    HitBox box = {};
    box.X = X;
    box.Y = Y;

    switch (Type)
    {
    case EnemyType::Type1:
        box.HalfWidth = 0.05f;
        box.HalfHeight = 0.05f;
        break;

    case EnemyType::Type2:
        box.HalfWidth = 0.04f;
        box.HalfHeight = 0.05f;
        break;

    case EnemyType::Type3:
        box.HalfWidth = 0.045f;
        box.HalfHeight = 0.063f;
        break;
    }

    return box;
}