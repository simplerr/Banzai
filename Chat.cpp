#include "common\Runnable.h"
#include "BitStream.h"
#include "Chat.h"
#include "Enums.h"
#include "PlayingOnline.h"
#include "Database.h"
#include "OnlinePlayer.h"
#include <Windowsx.h>
#include <Richedit.h>
#include <Commctrl.h>

void setFont(HWND hwnd, int height, int weight = FW_DONTCARE, string family="Arial");

// Message IDs.
#define ID_SEND_MESSAGE 101

// The default edit control procedure.
WNDPROC DefEditProc;

bool first = true;

//! Adds text to a richedit control.
/**
@param hwnd Handle to the control.
@param szTextIn The text to add.
@param crNewColor The text color.
@note Works on any window control, without the color effect.
*/
void Chat::AddText(HWND hwnd, char *szTextIn, COLORREF crNewColor)
{
   char *Text = (char *)malloc(lstrlen(szTextIn) + 5);
   CHARFORMAT cf;
   int iTotalTextLength = GetWindowTextLength(hwnd);
   int iStartPos = iTotalTextLength;
   int iEndPos;

   strcpy(Text, szTextIn);

   SendMessage(hwnd, EM_SETSEL, (WPARAM)(int)iTotalTextLength, (LPARAM)(int)iTotalTextLength);
   SendMessage(hwnd, EM_REPLACESEL, (WPARAM)(BOOL)FALSE, (LPARAM)(LPCSTR)Text);

   free(Text);

   cf.cbSize      = sizeof(CHARFORMAT);
   cf.dwMask      = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
   cf.dwEffects   = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
   cf.crTextColor = crNewColor;

   iEndPos = GetWindowTextLength(hwnd);

   SendMessage(hwnd, EM_SETSEL, (WPARAM)(int)iStartPos, (LPARAM)(int)iEndPos);
   SendMessage(hwnd, EM_SETCHARFORMAT, (WPARAM)(UINT)SCF_SELECTION, (LPARAM)&cf);
   SendMessage(hwnd, EM_HIDESELECTION, (WPARAM)(BOOL)TRUE, (LPARAM)(BOOL)FALSE);

   SendMessage(hwnd, EM_LINESCROLL, (WPARAM)(int)0, (LPARAM)(int)1);
}

//! Subclassed msg proc for the input box.
LRESULT inputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	bool enter = false;
    switch (uMsg)
    {
	case WM_CHAR:
		// Enter was pressed.
		if(wParam == VK_RETURN)
			enter = true;
	default:
		// Handles all default actions.
		result =  CallWindowProc(DefEditProc, hwnd, uMsg, wParam, lParam);
    }

	// Has to be here since case WM_CHAR is before the default procedure. 
	// Otherwise the caret positions gets set but then changed when the ENTER msg is proccessed.
	if(enter)
	{
		// Send the message so Chat can catch it.
		SendMessage(gGame->getMainWnd(), ID_SEND_MESSAGE, 0, 0);
	}

	return result;
}

//! Constructor.
Chat::Chat()
{
	// Needed
	InitCommonControls(); 

	// Set dimensions and coordinates
	int x = 675;
	int y = 410;
	int width = 380;
	int height = 200;

	// Create all the windows controls needed for the chat
	LoadLibrary("riched32.dll");
	mhChatBox = CreateWindowEx(WS_EX_CLIENTEDGE, "RichEdit", NULL, 
		WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
         x, y, width, height, gGame->getMainWnd(), (HMENU)IDC_CHAT_BOX, gGame->getAppInst(), NULL);

	mhSendButton = CreateWindow("BUTTON", "Send", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
        x+width-70, y+height+10, 70, 40, gGame->getMainWnd(), (HMENU)IDC_SEND_BUTTON, gGame->getAppInst(), NULL);

	mhInputBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | ES_MULTILINE | ES_AUTOVSCROLL,
        x, y+height+10, width-80, 40, gGame->getMainWnd(), (HMENU)IDC_INPUT_BOX, gGame->getAppInst(), NULL);

	// Set the char format for the chat box
	CHARFORMAT cf;
	cf.cbSize = sizeof (CHARFORMAT);  
    cf.dwMask = CFM_FACE; 
    strcpy(cf.szFaceName, "Arial"); 
    SendMessage(mhChatBox, EM_SETCHARFORMAT, (WPARAM)(UINT)0, (LPARAM)&cf);

	// Change the font in the input box
	setFont(mhInputBox, 16);

	// Set the default edit control proc
	DefEditProc = (WNDPROC)SetWindowLong(mhInputBox, GWL_WNDPROC, (DWORD)inputProc);

	// Add info text to the chat
	AddText(mhChatBox, "Welcome!\nYou can write /stats to see your opponents stats.\nVisit http://simplersnet.com for rankings.", RGB(175, 77, 146));
}
	
//! Destructor.
Chat::~Chat()
{
	// Destroy all windows
	DestroyWindow(mhChatBox);
	DestroyWindow(mhInputBox);
	DestroyWindow(mhSendButton);
	first = true;
}

//! Adds a message to the chat.
void Chat::addMessage(string from, string message)
{
	string name;
	if(first)
		name = "\n<";
	else
		name = "<";

	name += from;
	name += ">: ";
	AddText(mhChatBox, (char*)name.c_str(), RGB(180, 0, 0));
	AddText(mhChatBox, (char*)message.c_str(), RGB(0, 0, 0));
	first = false;
}
	
//! Send message to opponent and add it to your own chat.
void Chat::sendChatMessage()
{
	// Add the message to your own chat.
	int inputLen = GetWindowTextLength(mhInputBox) + 1;
	char* inputBuffer = new char[inputLen];
	GetWindowText(mhInputBox, inputBuffer, inputLen);

	// Display stats
	string input = string(inputBuffer);
	if(input.find("/stats", 0) == 0)//string::npos)//input.compare("/stats") == 0)
	{
		string opponent = PlayingOnline::Instance()->getPlayer()->getOpponent();

		if(opponent != "#NOVALUE")	{
			string tmp;
			if(first)
			 tmp = "\n";

			tmp += opponent;
			tmp += "'s stats:\n";
			AddText(mhChatBox, (char*)tmp.c_str(), RGB(177, 77, 146));

			char buffer[10];
			tmp = "Wins: ";
			tmp += itoa(gDatabase->getWins(opponent), buffer, 10);
			tmp += "\n";
			AddText(mhChatBox, (char*)tmp.c_str(), RGB(0, 0, 0));

			tmp = "Losses: ";
			tmp += itoa(gDatabase->getLosses(opponent), buffer, 10);
			tmp += "\n";
			AddText(mhChatBox, (char*)tmp.c_str(), RGB(0, 0, 0));
		}
		else	{
			if(first)
				AddText(mhChatBox, "\nNo opponent yet.\n", RGB(177, 77, 146));
			else
				AddText(mhChatBox, "No opponent yet.\n", RGB(177, 77, 146));
		}

		SetWindowText(mhInputBox, "");
		SendMessage(mhInputBox, EM_SETSEL, 0, MAKELONG(0, 0));	// NOTE: Not needed any more.
	}
	else
	{
		string name;
		if(first)
			name = "\n<";
		else
			name = "<";

		name += PlayingOnline::Instance()->getPlayerName();
		name += ">: ";
		AddText(mhChatBox, (char*)name.c_str(), RGB(0, 180, 0));
		AddText(mhChatBox, inputBuffer, RGB(0, 0, 0));

		SetWindowText(mhInputBox, "");
		SendMessage(mhInputBox, EM_SETSEL, 0, MAKELONG(0, 0));	// NOTE: Not needed any more.

		// Send the message to your opponent.
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)ID_MESSAGE_SENT);
		bitstream.Write(PlayingOnline::Instance()->getPlayerName().c_str());
		bitstream.Write(inputBuffer);

		PlayingOnline::Instance()->getPeer()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	first = false;
}

//! The message proc for the chat.
LRESULT Chat::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case ID_SEND_MESSAGE:
		sendChatMessage();
	case WM_COMMAND:
		// LOWORD(wParam) is the ID on the control window the message comes from.
		// lParam is a handle to the control window.
		if(LOWORD(wParam) == IDC_SEND_BUTTON)
			sendChatMessage();
	default:
		return 0;
	}
}