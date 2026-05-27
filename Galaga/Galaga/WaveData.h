#pragma once
#include "Enemy.h"

static const int WaveMaxEnemies = 6;

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
};

WaveDefinition GetWaveDefinition(int wave);