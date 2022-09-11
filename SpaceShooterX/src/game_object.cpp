#include "game_object.h"

#include "base_console.h"

void GameObject::OnDraw()
{
    Vector2i rounded;
    rounded.x = (int)m_rect.position.x;
    rounded.y = (int)m_rect.position.y;

    for (auto& pixel : m_pixels)
    {
        auto pixelPos = rounded + pixel.m_posisi;
        BaseConsole::GetInstance().Draw(pixelPos.x, pixelPos.y, pixel.m_glyph, pixel.m_color);
    }
}

void GameObject::AddPixel(wchar_t p_glyph, Vector2i p_pos, short p_color)
{
    if (p_pos.x < 0 || p_pos.y < 0)
        return;

    Pixel pixel = { p_glyph, p_color, p_pos };
    m_pixels.push_back(std::move(pixel));

    if (m_rect.size.x + p_pos.x > m_rect.size.x)
        m_rect.size.x = p_pos.x;

    if (m_rect.size.y + p_pos.y > m_rect.size.y)
        m_rect.size.y = p_pos.y;
}

void GameObject::Invalidate()
{
    s_markForRemoval = true;
}

const std::vector<Pixel>& GameObject::GetPixels()
{
    return m_pixels;
}
