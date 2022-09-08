#include "vector_2i.h"
#include "vector_2.h"

#include "../common.h"

void Vector2i::operator+=(const Vector2i& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

Vector2i Vector2i::operator+(const Vector2i& rhs) const
{
    return Vector2i(x + rhs.x, y + rhs.y);
}

bool Vector2i::operator==(const Vector2i& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

int Vector2i::Index() const
{
    return y * LEBAR_TAMPILAN + x;
}

void Vector2i::Nol()
{
    x = 0;
    y = 0;
}
