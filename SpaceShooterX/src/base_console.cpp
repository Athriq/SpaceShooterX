#include "base_console.h"

#include "math/vector_2.h"
#include "math/vector_2i.h"

BaseConsole& BaseConsole::GetInstance()
{
	static BaseConsole instance;
	return instance;
}

void BaseConsole::Init()
{
    s_output = GetStdHandle(STD_OUTPUT_HANDLE);;
    CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(s_output, &scrBufferInfo);
    short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
    short scrBufferWidth = scrBufferInfo.dwSize.X;
    COORD newSize = { scrBufferWidth, winHeight };

    s_writeRegion = { 0, 0, LEBAR_TAMPILAN - 1, TINGGI_TAMPILAN - 1 };

    if (!SetConsoleActiveScreenBuffer(s_output))
        ShowWinError(L"SetConsoleActiveScreenBuffer");

    if (!SetConsoleScreenBufferSize(s_output, newSize))
        ShowWinError(L"SetConsoleScreenBufferSize");

#ifdef _DEBUG
    SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
#else
    if (!SetConsoleDisplayMode(s_output, CONSOLE_FULLSCREEN_MODE, 0))
    {
        ShowWinError(L"SetConsoleDisplayMode", L"\n\nAttempting another method...");
        SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
    }
#endif

    CONSOLE_FONT_INFOEX font;
    font.cbSize = sizeof(font);
    font.dwFontSize.X = 15;
    font.dwFontSize.Y = 15;
    font.FontFamily = FF_DONTCARE;
    font.FontWeight = FW_NORMAL;
    wcscpy_s(font.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(s_output, false, &font);

    s_screenBuffer = new CHAR_INFO[LUAS_TAMPILAN];

    // Init DirectSound interface

    auto dsCreated = DirectSoundCreate8(NULL, &s_directSound, NULL);

    if (dsCreated != DS_OK)
    {
        ShowWinError(L"DirectSoundCreate8");
    }
    else
    {
        s_directSound->SetCooperativeLevel(GetConsoleWindow(), DSSCL_PRIORITY);
    }
}

void BaseConsole::Update(float elapsed)
{
    wchar_t s[256];
    swprintf_s(s, 256, L"Space Shooter X - FPS: %3.2f", 1.0f / elapsed);
    SetConsoleTitle(s);
    
    if (WriteConsoleOutput(s_output, s_screenBuffer, { LEBAR_TAMPILAN, TINGGI_TAMPILAN }, { 0,0 }, &s_writeRegion) == 0)
        ShowWinError(L"WriteConsoleOutput");
}

void BaseConsole::Clear()
{
    delete[] s_screenBuffer;
    s_directSound->Release();
}

void BaseConsole::ClearScreen()
{
	for (int i = 0; i < LUAS_TAMPILAN; i++)
	{
		s_screenBuffer[i].Char.UnicodeChar = L' ';
		s_screenBuffer[i].Attributes = FG_WHITE;
	}
}

void BaseConsole::CenterCursor(short xOffset)
{
    SetConsoleCursorPosition(s_output, { (short)(LEBAR_TAMPILAN / 2 + xOffset), TINGGI_TAMPILAN / 2 });
}

void BaseConsole::Draw(int p_x, int p_y, const wchar_t& p_glyph, short p_color)
{
	auto index = Vector2i(p_x, p_y).Index();
	s_screenBuffer[index].Char.UnicodeChar = p_glyph;
	s_screenBuffer[index].Attributes = p_color;
}

void BaseConsole::DrawString(Vector2 p_pos, std::wstring p_content, COLOR p_col)
{
	for (int i = 0; i < (int)p_content.size(); i++)
	{
		Vector2 temp = { p_pos.x + i, p_pos.y };
		s_screenBuffer[temp.Index()].Char.UnicodeChar = p_content[i];
		s_screenBuffer[temp.Index()].Attributes = p_col;
	}
}

const LPDIRECTSOUND8& BaseConsole::GetSoundInterface() const
{
    return s_directSound;
}

