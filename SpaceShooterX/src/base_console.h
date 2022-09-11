#pragma once

#include "common.h"

struct Vector2f;
struct Vector2i;

class BaseConsole
{
public:
	static BaseConsole& GetInstance();

	void Init();
	void Update(float elapsed);
	void Clear();

	void ClearScreen();
	void CenterCursor(short xOffset);
	void Draw(int p_x, int p_y, const wchar_t& p_glyph, short p_color = FG_WHITE);
	void DrawString(Vector2f p_pos, std::wstring p_content, COLOR p_col = FG_WHITE);

	template<typename ...Args>
	void DrawString(Vector2f p_pos, COLOR p_col, const wchar_t* p_format, Args&& ...args)
	{
		wchar_t buffer[350];
		swprintf_s(buffer, 350, p_format, args...);
		DrawString(p_pos, std::wstring(buffer), p_col);
	}

	const LPDIRECTSOUND8& GetSoundInterface() const;

	static int CartesianToGrid(const Vector2f& p_pos);
	static int CartesianToGrid(const Vector2i& p_pos);
	static int CartesianToGrid(int p_x, int p_y);

private:
	BaseConsole() {}

	CHAR_INFO* s_screenBuffer;
	HANDLE s_output;
	SMALL_RECT s_writeRegion;

	LPDIRECTSOUND8 s_directSound;

public:
	BaseConsole(BaseConsole const&) = delete;
	void operator=(BaseConsole const&) = delete;
};

