module;

module Math;

import <format>;
import Print;

namespace Math
{
    float RadiansToDegrees(float radians)
    {
        const float degrees = radians * 180.0f / Pi;
        Print::PrintMsg(std::format("RadiansToDegrees {} -> {}", radians, degrees));
        return degrees;
    }

    float DegressToRadians(float degrees)
    {
        const float radians = degrees * Pi / 180.0f;
        Print::PrintMsg(std::format("DegressToRadians {} -> {}", degrees, radians));
        return radians;
    }
}
