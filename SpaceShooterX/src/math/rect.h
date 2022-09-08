#pragma once

#include "vector_2.h"

struct Rect
{
    Vector2 position;
    Vector2 size;

    int ScaledIndex() const
    {
        return (position + size).Index();
    }

    bool Intersects(const Rect& other)
    {
        if ((int)position.x > ((int)other.position.x + (int)other.size.x))
            return false;

        if ((int)(position.x + size.x) < (int)other.position.x)
            return false;

        if ((int)position.y > (int)(other.position.y + other.size.y))
            return false;

        if ((int)(position.y + size.y) < (int)other.position.y)
            return false;

        return true;
    }
};