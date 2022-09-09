#include "audio_player.h"

#include <algorithm>
#include <filesystem>

#include "base_console.h"

constexpr auto DATA_ID = 0x61746164;

AudioPlayer::AudioPlayer(const char* p_path)
{
    Load(p_path);
}

AudioPlayer::~AudioPlayer()
{
    if (s_dsBuffer)
        s_dsBuffer->Release();
}

void AudioPlayer::OnUpdate(float elapsed)
{
    if (!s_dsBuffer)
        return;

    if (m_destroyOnFinish && s_hasBeenPlayedOnce && !IsPlaying())
        Invalidate();
}

void AudioPlayer::Load(const char* p_path)
{
    FILE* infile = nullptr;

    std::string path(p_path);

    if (!std::filesystem::exists(p_path))
        path = "assets/sounds/" + path;

    if (fopen_s(&infile, path.c_str(), "rb") != 0)
    {
        ShowGenericError("AudioPlayer::Load(): failed to open \"%s\"", path);
        return;
    }

    // Read header
    fread(&header, sizeof(header), 1, infile);

    // Read data header
    Chunk chunk;
    while (true)
    {
        fread(&chunk, sizeof(chunk), 1, infile);

        if (*(unsigned int*)&chunk.m_ID == DATA_ID)
            break;

        fseek(infile, chunk.m_size, SEEK_CUR);
    }


    int sample_size = header.m_bitsPerSample / 8;
    int samples_count = chunk.m_size * 8 / header.m_bitsPerSample;

    short int* data = new short int[samples_count];
    memset(data, 0, sizeof(short int) * samples_count);

    // Read the raw sound data
    for (int i = 0; i < samples_count; i++)
    {
        fread(&data[i], sample_size, 1, infile);
    }

    fclose(infile);

    // Create sound buffer
    memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
    waveFormat.wFormatTag = header.m_audioFormat;
    waveFormat.nChannels = header.m_numChannels;
    waveFormat.nSamplesPerSec = header.m_sampleRate;
    waveFormat.nBlockAlign = header.m_blockAlign;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.wBitsPerSample = header.m_bitsPerSample;

    memset(&bufferDesc, 0, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2;
    bufferDesc.dwBufferBytes = samples_count;
    bufferDesc.lpwfxFormat = &waveFormat;

    LPDIRECTSOUNDBUFFER tempBuffer = NULL;
    BaseConsole::GetInstance().GetSoundInterface()->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
    tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&s_dsBuffer);
    tempBuffer->Release();

    LPVOID lpvWrite;
    DWORD dwLength;
    s_dsBuffer->Lock(0, samples_count, &lpvWrite, &dwLength, NULL, NULL, 0);
    memcpy(lpvWrite, data, dwLength);
    s_dsBuffer->Unlock(lpvWrite, dwLength, NULL, 0);

    s_length = samples_count / waveFormat.nAvgBytesPerSec;

    delete[] data;
}

void AudioPlayer::Play()
{
    if (!s_dsBuffer)
        return;

    s_dsBuffer->SetCurrentPosition(0);
    s_dsBuffer->SetVolume(s_volume);

    if (FAILED(s_dsBuffer->Play(0, 0, m_loop ? DSBPLAY_LOOPING : 0)))
        ShowWinError(L"AudioPlayer::Play");

    s_hasBeenPlayedOnce = true;
}

void AudioPlayer::SetVolume(float p_value)
{
    s_volume = ConvertLinearToDb(p_value);
}

bool AudioPlayer::IsPlaying()
{
    if (!s_dsBuffer)
        return false;
    
    DWORD status;
    s_dsBuffer->GetStatus(&status);
    return status == DSBSTATUS_PLAYING;
}

float AudioPlayer::GetPlayPosition()
{
    if (!s_dsBuffer)
        return 0;

    DWORD playCursor = 0;
    
    if (FAILED(s_dsBuffer->GetCurrentPosition(&playCursor, NULL)))
        ShowWinError(L"AudioPlayer::GetPlayPosition");

    // Figure out why this doesn't match cached length
    return (float)playCursor / waveFormat.nAvgBytesPerSec;
}

float AudioPlayer::GetLength()
{
    return s_length;
}

float AudioPlayer::ConvertLinearToDb(float p_value)
{
    if (p_value < 0)
        p_value = 0;
    if (p_value > 1)
        p_value = 1;

    float minimum = std::abs(DSBVOLUME_MIN);
    return -(1 - ((p_value * minimum) / minimum)) * minimum;
}
