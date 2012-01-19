#include <string>
#include <Windows.h>
#include <wininet.h>
using namespace std;

#pragma comment(lib, "wininet")

//! Sets the font in a window control
/**
@param hwnd The HWND to set the font in.
@param height The height of the font.
@param weight The weight of the font.
@param family The family of the font.
*/
void setFont(HWND hwnd, int height, int weight = FW_DONTCARE, string family="Arial")
{
	// Change to font in the input box.
	HFONT hFont = CreateFont (height, 0, 0, 0, weight, FALSE, FALSE, FALSE, ANSI_CHARSET, \
	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
	DEFAULT_PITCH | FF_SWISS, family.c_str());

	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), true);
}

string getText(HWND hwnd)
{
	int inputLen = GetWindowTextLength(hwnd) + 1;
	char* inputBuffer = new char[inputLen];
	GetWindowText(hwnd, inputBuffer, inputLen);
	
	string text = inputBuffer;
	delete inputBuffer;

	return text;
}

//! Returns the IP adress
string getIP()
{
	HINTERNET hInternet, hFile;
    DWORD rSize;
    char buffer[32];

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    hFile = InternetOpenUrl(hInternet, "http://automation.whatismyip.com/n09230945.asp", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize);
    buffer[rSize] = '\0';

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

	return buffer;
}