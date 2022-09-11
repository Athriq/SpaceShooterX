#pragma once

#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <dsound.h>
#include <stdexcept>
#include <strsafe.h>
#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <map>
#include <fstream>

#include "math/math.h"

#define TINGGI_TAMPILAN 55
#define LEBAR_TAMPILAN 100
#define LUAS_TAMPILAN TINGGI_TAMPILAN * LEBAR_TAMPILAN

#define INTERVAL_MUSUH_MUNCUL 10
#define LAJU_TEMBAK_MUSUH 0.7f
#define LAJU_TEMBAK 0.13f
#define BANYAK_MUSUH_TIAP_INTERVAL 3
#define MAX_PLAYER_HEALTH 20
#define REPAIR_PACK_SPAWN_INTERVAL 15

void ShowWinError(const std::wstring& p_function);
void ShowWinError(const std::wstring& p_function, const std::wstring& p_info);
void ShowGenericError(const std::wstring& p_info);
void ShowGenericError(const std::string& p_info);

template<typename ...Args>
void ShowGenericError(const char* format, Args&& ...args)
{
	char buffer[250];
	sprintf_s(buffer, format, args...);
	ShowGenericError(std::string(buffer));
}

enum COLOR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};
