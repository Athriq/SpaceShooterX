#pragma once

struct Vector2;

struct Vector2i
{
    int x = 0; // Kolom
    int y = 0; // Baris

    Vector2i() {}
    Vector2i(int x, int y) : x(x), y(y) {}

    void operator+=(const Vector2i& rhs);
    Vector2i operator+(const Vector2i& rhs) const;
    bool operator==(const Vector2i& rhs) const;

    int Index() const;

    void Nol();
};