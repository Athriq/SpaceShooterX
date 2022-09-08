#pragma once

struct Vector2i;

struct Vector2
{
    float x = 0; // Kolom
    float y = 0; // Baris

    Vector2() {}
    Vector2(float x, float y) : x(x), y(y) {}

    void operator+=(const Vector2& rhs);
    void operator*=(float rhs);
    Vector2 operator+(const Vector2& rhs) const;
    bool operator==(const Vector2& rhs) const;

    int Index() const;

    void Nol();
};
