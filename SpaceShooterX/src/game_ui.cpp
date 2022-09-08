#include "game_ui.h"

#include "base_console.h"
#include "player.h"

void GameUI::Update(float elapsed)
{
    wchar_t hpValue[250];
    COLOR hpColor = FG_WHITE;

    if (!s_game.PlayerDead())
    {
        float formattedHp = (s_game.GetPlayerHealth() / (float)MAX_PLAYER_HEALTH) * 100.0f;
        std::swprintf(hpValue, 250, L"%.0f %%", formattedHp);

        //GambarString({ 1, 6 }, L"Velocity: (" + std::to_wstring(player->m_velocity.x) + L", " + std::to_wstring(player->m_velocity.y) + L')');

        if (formattedHp >= 70)
        {
            hpColor = FG_CYAN;
        }
        else if (formattedHp >= 40 && formattedHp < 70)
        {
            hpColor = FG_YELLOW;
        }
        else if (formattedHp >= 0 && formattedHp < 40)
        {
            hpColor = FG_RED;
        }
    }
    else
    {
        std::swprintf(hpValue, 250, L"Destroyed!");
        hpColor = FG_RED;
    }

    BaseConsole::GetInstance().DrawString({ 1, 1 }, FG_WHITE, L"Objects: %d", s_game.GetObjectCount());
    BaseConsole::GetInstance().DrawString({ 1, 2 }, FG_WHITE, L"Next Wave: %.0fs / %ds", s_game.GetTimeUntilNextWave(), INTERVAL_MUSUH_MUNCUL);
    BaseConsole::GetInstance().DrawString({ 1, 3 }, FG_WHITE, L"Difficulty increase: %.1f (min spawn time)", s_game.GetDifficultyLevel());

    std::wstring scoreText(L"Score: " + std::to_wstring(s_game.m_score));
    BaseConsole::GetInstance().DrawString({ (LEBAR_TAMPILAN / 2) - ((float)scoreText.length() / 2) - scoreText.length(), TINGGI_TAMPILAN - 2}, scoreText, FG_MAGENTA);

    BaseConsole::GetInstance().DrawString({ (LEBAR_TAMPILAN / 2), TINGGI_TAMPILAN - 2 }, L"Hull:", FG_GREEN);
    BaseConsole::GetInstance().DrawString({ (LEBAR_TAMPILAN / 2) + 6, TINGGI_TAMPILAN - 2 }, hpValue, hpColor);

    if (s_game.GameOver())
    {
        BaseConsole::GetInstance().DrawString({ (LEBAR_TAMPILAN / 2) - 5, (TINGGI_TAMPILAN / 2) - 2 }, L"GAME OVER!", BG_WHITE);
        BaseConsole::GetInstance().DrawString({ (LEBAR_TAMPILAN / 2) - 10, (TINGGI_TAMPILAN / 2) - 1 }, L"HIGHEST SCORE: " + std::to_wstring(s_game.m_score), BG_WHITE);
    }
}
