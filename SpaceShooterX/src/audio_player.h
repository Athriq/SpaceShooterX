#pragma once

#include "game_object.h"

class AudioPlayer : public GameObject
{
public:
    AudioPlayer() {}
    AudioPlayer(const char* p_path);
    ~AudioPlayer();

    void OnUpdate(float elapsed);

    void LoadFromFile(const char* p_path);
    void Play(float p_from = 0);

    void SetVolume(float p_value);
    void SetFrequency(float p_freq);

    bool IsPlaying();
    float GetPlayPosition();
    float GetLength();

private:
    float ConvertLinearToDb(float p_value);

public:
    bool m_loop = false;
    bool m_destroyOnFinish = false;

private:
    struct WavHeader
    {
        char m_chunkID[4];
        unsigned long m_chunkSize;
        char m_format[4];
        char m_subchunk1ID[4];
        unsigned long m_subchunk1Size;
        unsigned short m_audioFormat;
        unsigned short m_numChannels;
        unsigned long m_sampleRate;
        unsigned long m_byteRate;
        unsigned short m_blockAlign;
        unsigned short m_bitsPerSample;
    };

    struct Chunk
    {
        char m_ID[4];
        unsigned long m_size;
    };

private:
    WavHeader header;
    WAVEFORMATEX waveFormat;
    IDirectSoundBuffer8* s_dsBuffer = nullptr;
    DSBUFFERDESC bufferDesc;

    float s_volume = 0;
    float s_length = 0;

    bool s_hasBeenPlayedOnce = false;
};

