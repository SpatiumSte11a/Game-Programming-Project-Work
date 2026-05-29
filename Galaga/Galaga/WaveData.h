#pragma once

#include "Enemy.h"

static const int WaveMaxEnemies = 9;

struct EnemySpawnData
{
    bool IsUsed = false;
    EnemyType Type = EnemyType::Type1;
    float X = 0.0f;
    float Y = 0.0f;
};

struct WaveDefinition
{
    int WaveNumber = 1;
    int SpawnCount = 0;
    EnemySpawnData Spawns[WaveMaxEnemies];

    float Type2ShootCooldownScale = 1.0f;
    float Type1MoveSpeedScale = 1.0f;
    float Type1AttackDelayScale = 1.0f;
    float Type3AttackDelayScale = 1.0f;
};

WaveDefinition GetWaveDefinition(int wave);