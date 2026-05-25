#pragma once

class GraphicsContext;

class Bullet
{
private:
    float X;
    float Y;
    float Speed;
    bool IsActive;

public:
    Bullet();

    void Spawn(float x, float y, float speed);
    void Update(float dt);
    void Deactivate();

    bool GetIsActive() const;
    float GetX() const;
    float GetY() const;
};

class PlayerBulletSystem
{
private:
    static const int MaxBullets = 12;
    Bullet Bullets[MaxBullets];

    float BulletSpeed;
    float FireCooldown;
    float FireTimer;

public:
    PlayerBulletSystem();

    void Update(float dt, bool shootPressed, float playerX, float playerY);
    void Render(GraphicsContext& graphics) const;
};

class EnemyBulletSystem
{
private:
    static const int MaxBullets = 20;
    Bullet Bullets[MaxBullets];

    float BulletSpeed;
    float FireCooldown;
    float FireTimer;

public:
    EnemyBulletSystem();

    void Update(float dt, bool canShoot, float enemyX, float enemyY);
    void Render(GraphicsContext& graphics) const;
};