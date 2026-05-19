#pragma once
#include <chrono>

class DeltaTime
{
private:
    std::chrono::high_resolution_clock::time_point PrevTime;

public:
    DeltaTime()
    {
        PrevTime = std::chrono::high_resolution_clock::now();
    }

    float GetDelta()
    {
        auto CurrTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(CurrTime - PrevTime).count();
        PrevTime = CurrTime;
        return dt;
    }
};