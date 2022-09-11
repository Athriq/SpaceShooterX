#pragma once

#include "common.h"

#include "game.h"
#include "math/vector_2f.h"
#include "math/vector_2i.h"
#include "math/rect.h"

struct Pixel
{
    wchar_t m_glyph;
    short m_color;
    Vector2i m_posisi;

    Pixel(wchar_t p_glyph, short p_color, Vector2i p_posisi) : m_glyph(p_glyph), m_color(p_color), m_posisi(p_posisi) {}
};

/**
 * @brief A game object manageable by the game.
 *
 */
class GameObject
{
    friend Game;
public:
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float elapsed) {}
    virtual void OnDraw();
    virtual void OnCollide(GameObject& other) {}

    void AddPixel(wchar_t glyph, Vector2i pos = { 0, 0 }, short color = FG_WHITE);
    const std::vector<Pixel>& GetPixels();

    /**
     *  @brief Requests object to be removed.
     * 
     */ 
    void Invalidate();

public:
    Rect m_rect;
    Vector2f m_velocity;
    bool m_collidable = true;
    bool lockVerticalBounds = false;

protected:
    // This is very unsafe lol
    // TODO: FIX THIS
    Game* m_game = nullptr;

    std::vector<Pixel> m_pixels;

private:
    bool s_markForRemoval = false;
};