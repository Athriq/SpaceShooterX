#pragma once

#include "audio_player.h"

class RepairPack : public GameObject
{
public:
    void OnAttach();
    void OnCollide(GameObject& other);
};

