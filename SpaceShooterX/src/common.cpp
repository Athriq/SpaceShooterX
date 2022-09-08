#include "common.h"

void ShowWinError(const std::wstring& p_function)
{
	LPVOID messageBuffer;
	LPVOID displayBuffer;

	LPTSTR lpszFunction = const_cast<wchar_t*>(p_function.c_str());

	DWORD dw = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);

	displayBuffer = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)messageBuffer) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)displayBuffer, LocalSize(displayBuffer) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, messageBuffer);
	MessageBox(NULL, (LPCTSTR)displayBuffer, TEXT("Error"), MB_OK);

	LocalFree(messageBuffer);
	LocalFree(displayBuffer);
}

void ShowWinError(const std::wstring& p_function, const std::wstring& p_extraInfo)
{
	LPVOID messageBuffer;
	LPVOID displayBuffer;

	LPTSTR lpszFunction = const_cast<wchar_t*>(p_function.c_str());
	LPTSTR lpszAddInfo = const_cast<wchar_t*>(p_extraInfo.c_str());

	DWORD dw = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);

	displayBuffer = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)messageBuffer) + lstrlen((LPCTSTR)lpszFunction) + lstrlen((LPCTSTR)lpszAddInfo) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)displayBuffer, LocalSize(displayBuffer) / sizeof(TCHAR), TEXT("%s failed with error %d: %s%s"), lpszFunction, dw, messageBuffer, lpszAddInfo);
	MessageBox(NULL, (LPCTSTR)displayBuffer, TEXT("Error"), MB_OK);

	LocalFree(messageBuffer);
	LocalFree(displayBuffer);
}

void ShowGenericError(const std::wstring& p_info)
{
	MessageBox(NULL, p_info.c_str(), TEXT("Error"), MB_OK);
}

void ShowGenericError(const std::string& p_info)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, p_info.c_str(), p_info.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, p_info.c_str(), p_info.length(), &wstr[0], count);
	ShowGenericError(wstr);
}
