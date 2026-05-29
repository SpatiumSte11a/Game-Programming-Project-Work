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
    WaveDefinition result{};

    result.WaveNumber = wave;
    result.SpawnCount = 0;
    result.Type2ShootCooldownScale = 1.0f;
    result.Type1MoveSpeedScale = 1.0f;
    result.Type1AttackDelayScale = 1.0f;
    result.Type3AttackDelayScale = 1.0f;

    int originalWave = wave;
    int layoutWave = wave;

    if (layoutWave > 10)
        layoutWave = 10;

    const float type1Y = 0.30f;
    const float type2Y = 0.48f;
    const float type3Y = 0.74f;

    const float leftX = -0.75f;
    const float centerX = 0.00f;
    const float rightX = 0.75f;

    const float gapLeftX = -0.375f;
    const float gapRightX = 0.375f;

    const float wave4LeftOuter = -0.90f;
    const float wave4LeftInner = -0.30f;
    const float wave4RightInner = 0.30f;
    const float wave4RightOuter = 0.90f;

    const float wave5Type2Left = -0.50f;
    const float wave5Type2Center = 0.00f;
    const float wave5Type2Right = 0.50f;

    const float wave6Type2FarLeft = -0.75f;
    const float wave6Type2MidLeft = -0.25f;
    const float wave6Type2MidRight = 0.25f;
    const float wave6Type2FarRight = 0.75f;

    if (layoutWave == 1)
    {
        result.WaveNumber = originalWave;
        result.SpawnCount = 3;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, leftX, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, centerX, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, rightX, type1Y);
    }
    else if (layoutWave == 2)
    {
        result.WaveNumber = originalWave;
        result.SpawnCount = 5;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, leftX, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, centerX, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, rightX, type1Y);

        result.Spawns[3] = MakeSpawn(EnemyType::Type2, gapLeftX, type2Y);
        result.Spawns[4] = MakeSpawn(EnemyType::Type2, gapRightX, type2Y);
    }
    else if (layoutWave == 3)
    {
        result.WaveNumber = originalWave;
        result.SpawnCount = 6;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, leftX, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, centerX, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, rightX, type1Y);

        result.Spawns[3] = MakeSpawn(EnemyType::Type2, gapLeftX, type2Y);
        result.Spawns[4] = MakeSpawn(EnemyType::Type2, gapRightX, type2Y);

        result.Spawns[5] = MakeSpawn(EnemyType::Type3, centerX, type3Y);
    }
    else if (layoutWave == 4)
    {
        result.WaveNumber = originalWave;
        result.SpawnCount = 7;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, wave4LeftOuter, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, wave4LeftInner, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, wave4RightInner, type1Y);
        result.Spawns[3] = MakeSpawn(EnemyType::Type1, wave4RightOuter, type1Y);

        result.Spawns[4] = MakeSpawn(EnemyType::Type2, gapLeftX, type2Y);
        result.Spawns[5] = MakeSpawn(EnemyType::Type2, gapRightX, type2Y);

        result.Spawns[6] = MakeSpawn(EnemyType::Type3, centerX, type3Y);
    }
    else if (layoutWave == 5)
    {
        result.WaveNumber = originalWave;
        result.SpawnCount = 8;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, wave4LeftOuter, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, wave4LeftInner, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, wave4RightInner, type1Y);
        result.Spawns[3] = MakeSpawn(EnemyType::Type1, wave4RightOuter, type1Y);

        result.Spawns[4] = MakeSpawn(EnemyType::Type2, wave5Type2Left, type2Y);
        result.Spawns[5] = MakeSpawn(EnemyType::Type2, wave5Type2Center, type2Y);
        result.Spawns[6] = MakeSpawn(EnemyType::Type2, wave5Type2Right, type2Y);

        result.Spawns[7] = MakeSpawn(EnemyType::Type3, centerX, type3Y);
    }
    else
    {
        result.WaveNumber = originalWave;
        result.SpawnCount = 9;

        result.Spawns[0] = MakeSpawn(EnemyType::Type1, wave4LeftOuter, type1Y);
        result.Spawns[1] = MakeSpawn(EnemyType::Type1, wave4LeftInner, type1Y);
        result.Spawns[2] = MakeSpawn(EnemyType::Type1, wave4RightInner, type1Y);
        result.Spawns[3] = MakeSpawn(EnemyType::Type1, wave4RightOuter, type1Y);

        result.Spawns[4] = MakeSpawn(EnemyType::Type2, wave6Type2FarLeft, type2Y);
        result.Spawns[5] = MakeSpawn(EnemyType::Type2, wave6Type2MidLeft, type2Y);
        result.Spawns[6] = MakeSpawn(EnemyType::Type2, wave6Type2MidRight, type2Y);
        result.Spawns[7] = MakeSpawn(EnemyType::Type2, wave6Type2FarRight, type2Y);

        result.Spawns[8] = MakeSpawn(EnemyType::Type3, centerX, type3Y);
    }

    if (layoutWave == 7)
    {
        result.Type2ShootCooldownScale = 0.75f;
    }
    else if (layoutWave == 8)
    {
        result.Type2ShootCooldownScale = 0.75f;
        result.Type1MoveSpeedScale = 1.20f;
    }
    else if (layoutWave == 9)
    {
        result.Type2ShootCooldownScale = 0.75f;
        result.Type1MoveSpeedScale = 1.20f;
        result.Type1AttackDelayScale = 0.90f;
        result.Type3AttackDelayScale = 0.75f;
    }
    else if (layoutWave >= 10)
    {
        result.Type2ShootCooldownScale = 0.65f;
        result.Type1MoveSpeedScale = 1.35f;
        result.Type1AttackDelayScale = 0.75f;
        result.Type3AttackDelayScale = 0.60f;
    }

    if (originalWave > 10)
    {
        int extraTier = (originalWave - 10) / 2;

        if (extraTier > 6)
            extraTier = 6;

        result.Type1MoveSpeedScale += 0.05f * extraTier;
        if (result.Type1MoveSpeedScale > 1.65f)
            result.Type1MoveSpeedScale = 1.65f;

        result.Type1AttackDelayScale -= 0.02f * extraTier;
        if (result.Type1AttackDelayScale < 0.65f)
            result.Type1AttackDelayScale = 0.65f;

        result.Type2ShootCooldownScale -= 0.025f * extraTier;
        if (result.Type2ShootCooldownScale < 0.50f)
            result.Type2ShootCooldownScale = 0.50f;

        result.Type3AttackDelayScale -= 0.025f * extraTier;
        if (result.Type3AttackDelayScale < 0.45f)
            result.Type3AttackDelayScale = 0.45f;
    }

    return result;
}