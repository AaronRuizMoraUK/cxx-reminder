module;

export module Math;

export namespace Math
{
    constexpr float Pi = 3.14159265359f;
    constexpr float TwoPi = 2.0f * Pi;
    constexpr float HalfPi = Pi / 2.0f;

    float RadiansToDegrees(float radians);

    float DegressToRadians(float degrees);

    template<typename T>
    const T& Max(const T& lhs, const T& rhs)
    {
        return (lhs > rhs) ? lhs : rhs;
    }

    template<typename T>
    const T& Min(const T& lhs, const T& rhs)
    {
        return (lhs < rhs) ? lhs : rhs;
    }
}
