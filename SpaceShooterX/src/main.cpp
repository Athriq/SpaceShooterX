#include <initguid.h>
#include "common.h"
#include "base_console.h"
#include "game_ui.h"
#include "animated_object.h"
#include "audio_player.h"

#include "math/vector_2i.h"
#include "random_extension.h"

int main(int argc, char* argv[])
{
    BaseConsole::GetInstance().Init();

    // A WHOLE NEW WORLD
    Game game;

    auto themePlayer = std::make_shared<AudioPlayer>("theme.wav");
    themePlayer->m_loop = true;
    themePlayer->SetVolume(0.8f);
    themePlayer->Play();

    GameUI gameUI(game);

    auto t0 = std::chrono::system_clock::now();
    auto t1 = std::chrono::system_clock::now();

    // TODO: implement
    bool paused = false;

    while (true)
    {
        while (!paused)
        {
            t1 = std::chrono::system_clock::now();
            std::chrono::duration<float> dt = t1 - t0;
            t0 = t1;
            float elapsed = dt.count();

            BaseConsole::GetInstance().ClearScreen();

            bool exit = game.Update(elapsed);

            gameUI.Update(elapsed);

            BaseConsole::GetInstance().Update(elapsed);

            if (exit)
                break;
        }

        BaseConsole::GetInstance().CenterCursor(-15);
        system("pause");

        game.ResetGame();
        game.SpawnPlayer();
    }

    BaseConsole::GetInstance().Clear();

    return 0;
}