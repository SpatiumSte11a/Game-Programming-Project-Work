#pragma once
#include "GraphicsContext.h"

class StartupScreen
{
public:
    StartupScreen() = default;

    void Initialize();

    void Render(GraphicsContext& graphics, float globalTime);
};