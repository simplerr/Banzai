#ifndef CHAT_H
#define CHAT_H

#include <Windows.h>
#include <string>
#include "RakPeerInterface.h"
using namespace std;

#define IDC_SEND_BUTTON 101
#define IDC_CHAT_BOX	102
#define IDC_INPUT_BOX	103

//! Contains the chat components.
class Chat
{
public:
	Chat();
	~Chat();

	void addMessage(string from, string message);
	void sendChatMessage();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void AddText(HWND hwnd, char *szTextIn, COLORREF crNewColor);
private:
	// Window controls.
	HWND mhChatBox;
	HWND mhInputBox;
	HWND mhSendButton;

};

#endif