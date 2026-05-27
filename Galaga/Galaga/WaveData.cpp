#include "WaveData.h"

static EnemySpawnData MakeSpawn(EnemyType type, float x, float y)
{
    EnemySpawnData spawn;
    spawn.IsUsed = true;
    spawn.Type = type;
    spawn.X = x;
    spawn.Y = y;
    return spawn;
}

WaveDefinition GetWaveDefinition(int wave)
{
    WaveDefinition result;

    const float type1Y = 0.20f;
    const float type2Y = 0.48f;
    const float type3Y = 0.74f;

    const float leftX = -0.75f;
    const float centerX = 0.00f;
    const float rightX = 0.75f;

    const float gapLeftX = -0.375f;
    const float gapRightX = 0.375f;

    if (wave <= 1)
    {
        result.WaveNumber = 1;
        result.SpawnCount = 3;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, leftX, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, centerX, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, rightX, type1Y);
    }
    else if (wave == 2)
    {
        result.WaveNumber = 2;
        result.SpawnCount = 5;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, leftX, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, centerX, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, rightX, type1Y);

        result.Spawns[3] = MakeSpawn(EnemyType::Type2, gapLeftX, type2Y);
        result.Spawns[4] = MakeSpawn(EnemyType::Type2, gapRightX, type2Y);
    }
    else
    {
        result.WaveNumber = wave;
        result.SpawnCount = 6;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, leftX, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, centerX, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, rightX, type1Y);

        result.Spawns[3] = MakeSpawn(EnemyType::Type2, gapLeftX, type2Y);
        result.Spawns[4] = MakeSpawn(EnemyType::Type2, gapRightX, type2Y);

        result.Spawns[5] = MakeSpawn(EnemyType::Type3, centerX, type3Y);
    }

    return result;
}