#pragma once
#include <cmath>

struct HitBox
{
    float X;
    float Y;
    float HalfWidth;
    float HalfHeight;
};

inline bool CheckHitBoxCollision(const HitBox& a, const HitBox& b)
{
    return std::fabs(a.X - b.X) <= (a.HalfWidth + b.HalfWidth) &&
        std::fabs(a.Y - b.Y) <= (a.HalfHeight + b.HalfHeight);
}