#pragma once

struct Vector2i;

struct Vector2f
{
    float x = 0; // Kolom
    float y = 0; // Baris

    Vector2f() {}
    Vector2f(float x, float y) : x(x), y(y) {}

    float Angle() const;
    float LengthSquared() const;

    void operator+=(const Vector2f& rhs);
    void operator*=(const float& rhs);
    Vector2f operator+(const Vector2f& rhs) const;
    Vector2f operator*(const float& rhs) const;
    bool operator==(const Vector2f& rhs) const;

    void Nol();
};
