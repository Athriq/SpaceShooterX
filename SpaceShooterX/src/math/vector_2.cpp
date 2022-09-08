#include "vector_2.h"
#include "vector_2i.h"

#include "../common.h"

void Vector2::operator+=(const Vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vector2::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
}

Vector2 Vector2::operator+(const Vector2& rhs) const
{
    return Vector2(x + rhs.x, y + rhs.y);
}

bool Vector2::operator==(const Vector2& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

int Vector2::Index() const
{
    return (int)y * LEBAR_TAMPILAN + (int)x;
}

void Vector2::Nol()
{
    x = 0;
    y = 0;
}
