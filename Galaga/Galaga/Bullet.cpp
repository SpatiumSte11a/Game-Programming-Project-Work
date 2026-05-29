#include "Bullet.h"
#include "GraphicsContext.h"

Bullet::Bullet()
    : X(0.0f),
    Y(0.0f),
    Speed(0.0f),
    IsActive(false)
{
}

void Bullet::Spawn(float x, float y, float speed)
{
    X = x;
    Y = y;
    Speed = speed;
    IsActive = true;
}

void Bullet::Update(float dt)
{
    if (!IsActive)
        return;

    Y += Speed * dt;

    if (Y > 1.1f || Y < -1.1f)
        IsActive = false;
}

void Bullet::Deactivate()
{
    IsActive = false;
}

bool Bullet::GetIsActive() const
{
    return IsActive;
}

float Bullet::GetX() const
{
    return X;
}

float Bullet::GetY() const
{
    return Y;
}

HitBox Bullet::GetHitBox() const
{
    HitBox box = {};
    box.X = X;
    box.Y = Y;
    box.HalfWidth = 0.012f;
    box.HalfHeight = 0.020f;
    return box;
}

PlayerBulletSystem::PlayerBulletSystem()
    : BulletSpeed(1.0f),
    FireCooldown(0.35f),
    FireTimer(0.0f)
{
}

void PlayerBulletSystem::Update(float dt, bool shootPressed, float playerX, float playerY)
{
    if (FireTimer > 0.0f)
        FireTimer -= dt;

    if (shootPressed && FireTimer <= 0.0f)
    {
        for (int i = 0; i < MaxBullets; i++)
        {
            if (!Bullets[i].GetIsActive())
            {
                Bullets[i].Spawn(playerX, playerY + 0.08f, BulletSpeed);
                FireTimer = FireCooldown;
                break;
            }
        }
    }

    for (int i = 0; i < MaxBullets; i++)
    {
        Bullets[i].Update(dt);
    }
}

void PlayerBulletSystem::Render(GraphicsContext& graphics) const
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (Bullets[i].GetIsActive())
        {
            graphics.DrawTriangle(Bullets[i].GetX(), Bullets[i].GetY(), 0.18f, 0.35f);
        }
    }
}

int PlayerBulletSystem::GetBulletCount() const
{
    return MaxBullets;
}

Bullet& PlayerBulletSystem::GetBullet(int index)
{
    return Bullets[index];
}

const Bullet& PlayerBulletSystem::GetBullet(int index) const
{
    return Bullets[index];
}

EnemyBulletSystem::EnemyBulletSystem()
    : BulletSpeed(-1.0f)
{
}

void EnemyBulletSystem::Update(float dt)
{
    for (int i = 0; i < MaxBullets; i++)
    {
        Bullets[i].Update(dt);
    }
}

bool EnemyBulletSystem::TryShoot(float enemyX, float enemyY)
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (!Bullets[i].GetIsActive())
        {
            Bullets[i].Spawn(enemyX, enemyY - 0.08f, BulletSpeed);
            return true;
        }
    }

    return false;
}

void EnemyBulletSystem::Render(GraphicsContext& graphics) const
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (Bullets[i].GetIsActive())
        {
            graphics.DrawQuad(Bullets[i].GetX(), Bullets[i].GetY(), 0.22f, 0.22f);
        }
    }
}

int EnemyBulletSystem::GetBulletCount() const
{
    return MaxBullets;
}

Bullet& EnemyBulletSystem::GetBullet(int index)
{
    return Bullets[index];
}

const Bullet& EnemyBulletSystem::GetBullet(int index) const
{
    return Bullets[index];
}