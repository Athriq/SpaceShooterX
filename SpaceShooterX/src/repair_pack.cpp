#include "repair_pack.h"

#include "random_extension.h"
#include "player.h"

void RepairPack::OnAttach()
{
    AddPixel(L'֍', { 0, 0 }, FG_GREEN);
    m_velocity.y = RandomUtils::random(2.0f, 4.0f);
}

void RepairPack::OnCollide(GameObject& other)
{
    if (auto player = dynamic_cast<Player*>(&other))
    {
        player->GiveHealth(10);
        Invalidate();
    }
}
