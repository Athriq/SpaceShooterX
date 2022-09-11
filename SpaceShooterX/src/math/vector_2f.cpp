#include "vector_2f.h"
#include "vector_2i.h"

#include <math.h>

float Vector2f::Angle() const
{
    return atan2f(y, x);
}

void Vector2f::operator+=(const Vector2f& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vector2f::operator*=(const float& rhs)
{
    x *= rhs;
    y *= rhs;
}

Vector2f Vector2f::operator+(const Vector2f& rhs) const
{
    return Vector2f(x + rhs.x, y + rhs.y);
}

Vector2f Vector2f::operator*(const float& rhs) const
{
    return Vector2f(x * rhs, y * rhs);
}

bool Vector2f::operator==(const Vector2f& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

void Vector2f::Nol()
{
    x = 0;
    y = 0;
}
