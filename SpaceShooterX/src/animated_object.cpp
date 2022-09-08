#include "animated_object.h"

void AnimatedObject::OnAttach()
{
    if (m_pixels.empty())
        AddPixel(L' ');
}

void AnimatedObject::OnUpdate(float elapsed)
{
    if (!s_currentClip)
        return;

    auto& sequence = s_currentClip->GetSequence();

    if (sequence.size() <= 0)
        return;

    s_totalProgress += elapsed;
    float progress = s_totalProgress;

    for (auto& frame : sequence)
    {
        progress -= frame.m_duration;

        if (progress <= 0.0f || &frame == &sequence.back())
        {
            for (auto& pixel : m_pixels)
            {
                pixel.m_glyph = frame.m_pixel.m_glyph == NULL ? pixel.m_glyph : frame.m_pixel.m_glyph;
                pixel.m_color = frame.m_pixel.m_color;
            }

            break;
        }
    }

    if (s_totalProgress > s_currentClip->GetLength())
    {
        if (m_destroyOnAnimationEnd)
            Invalidate();

        if (m_loop)
            s_totalProgress = 0;
    }
}

void AnimatedObject::AddAnimationClip(const std::string& p_name, AnimationClip&& p_clip)
{
    s_clips.insert({ p_name, std::make_shared<AnimationClip>(std::move(p_clip)) });
}

void AnimatedObject::Stop()
{
    s_currentClip = nullptr;
    s_totalProgress = 0;
}

void AnimatedObject::Play(const std::string& p_name, bool p_loop)
{
    auto found = s_clips.find(p_name);

    if (found == s_clips.end())
    {
        ShowGenericError("Not found animation clip: " + p_name);
        return;
    }

    if (found->second.get() == s_currentClip.get() && s_totalProgress <= s_currentClip->GetLength())
        return;

    Stop();
    s_currentClip = found->second;
    m_loop = p_loop;
}
