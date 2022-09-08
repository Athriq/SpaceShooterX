#pragma once

#include "game_object.h"

struct AnimationFrame
{
    Pixel m_pixel;
    float m_duration;

    AnimationFrame(Pixel frame, float duration) : m_pixel(frame), m_duration(duration) {}
};

class AnimationClip
{
public:
    void AddFrame(AnimationFrame&& p_frame)
    {
        s_sequence.push_back(std::move(p_frame));
        s_length += p_frame.m_duration;
    }

    const auto& GetSequence() const { return s_sequence; }
    float GetLength() const { return s_length; }

private:
    std::vector<AnimationFrame> s_sequence;
    float s_length = 0;
};

class AnimatedObject : public GameObject
{
public:
    void OnAttach();
    void OnUpdate(float elapsed);

    void AddAnimationClip(const std::string& p_name, AnimationClip&& p_clip);

    // Plays animation clip with the given name
    void Play(const std::string& p_name, bool p_loop = false);

    // Stops currently playing animation clip
    void Stop();

public:
    bool m_loop = false;
    
    // Mark this object for removal when the animation sequence completes.
    bool m_destroyOnAnimationEnd = false;

private:
    std::shared_ptr<AnimationClip> s_currentClip;
    std::map<std::string, std::shared_ptr<AnimationClip>> s_clips;
    float s_totalProgress = 0;
};