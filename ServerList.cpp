#include "common\Runnable.h"
#include "Database.h"
#include "ServerList.h"
#include "Enums.h"
#include "LobbyState.h"


void setFont(HWND hwnd, int height, int weight = FW_DONTCARE, string family="Arial");
string getPublicIp();
string getLocalIp();

//! Constructor
ServerList::ServerList(int x, int y, int width, int height)
{
	// Create the panel
	mhPanel = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | WS_BORDER, x, y, width, height, 
		gGame->getMainWnd(), NULL, gGame->getAppInst(), NULL);

	// Create the list box
	mhList =  NULL;

	// Create the label
	mhLabel = CreateWindow("STATIC", "Game list", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, 30, 20, width-10, 40,
		mhPanel, NULL, gGame->getAppInst(), NULL);

	// Create the buttons
	mhRefreshButton = CreateWindow("BUTTON", "Refresh", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
        x+width-187, y+height-50, 70, 40, gGame->getMainWnd(), (HMENU)IDC_REFRESH_BUTTON, gGame->getAppInst(), NULL);

	mhConnectButton = CreateWindow("BUTTON", "Connect", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
        x+width-105, y+height-50, 70, 40, gGame->getMainWnd(), (HMENU)IDC_CONNECT_BUTTON, gGame->getAppInst(), NULL);

	mhHostButton = CreateWindow("BUTTON", "Host", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
        x+130, y+height-50, 70, 40, gGame->getMainWnd(), (HMENU)IDC_HOST_BUTTON, gGame->getAppInst(), NULL);

	mhLanCheckBox = CreateWindow("BUTTON", "Lan", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | BS_AUTOCHECKBOX,
        x+30, y+height-50, 70, 40, gGame->getMainWnd(), (HMENU)IDC_LAN_CHECKBOX, gGame->getAppInst(), NULL);

	setFont(mhRefreshButton, 17);
	setFont(mhConnectButton, 17);
	setFont(mhHostButton, 17);
	setFont(mhLanCheckBox, 17);
	setFont(mhLabel, 23, FW_BOLD);

	mX = x;
	mY = y;
	mHeight = height;
	mWidth = width;

	refresh();
}

//! Destructor.
ServerList::~ServerList()
{
	DestroyWindow(mhPanel);
	DestroyWindow(mhList);
	DestroyWindow(mhRefreshButton);
	DestroyWindow(mhConnectButton);
	DestroyWindow(mhHostButton);
	DestroyWindow(mhLabel);
	DestroyWindow(mhLanCheckBox);
}

//! Creates the list control that contains all games.
void ServerList::createListControl()
{
	// Create the list box
	mhList =  CreateWindow("ListBox", "Server List", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | LBS_DISABLENOSCROLL | WS_VSCROLL | WS_BORDER | LBS_NOTIFY,
    mX+30, mY+70, mWidth-60, mHeight-130, gGame->getMainWnd(), (HMENU)IDC_SERVER_LIST, gGame->getAppInst(), NULL);

	setFont(mhList, 18, FW_DONTCARE);
}

//! Refreshses the game list.
void ServerList::refresh()
{
	if(mhList != NULL)
		DestroyWindow(mhList);

	// Clear the list box
	createListControl();

	// Clear the server list and retrieve a new one from the database
	mServerList.clear();

	vector<Server> tmp = gDatabase->getServers();

	for(int i = 0; i < tmp.size(); i++)
	{
		addServer(tmp[i].host, tmp[i].publicIp, tmp[i].localIp);
	}
}

//! Adds a game to the list.
void ServerList::addServer(string host, string publicIp, string localIp)
{
	// Add new server to the server list
	Server server;
	server.host = host;
	server.publicIp = publicIp;
	server.localIp = localIp;
	mServerList.push_back(server);

	// Add new item to the list box
	SendMessage(mhList, LB_ADDSTRING, 0, (LPARAM)host.c_str());
}

//! Handles window messages.
LRESULT ServerList::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		// The list box was pressed
		if(LOWORD(wParam) == IDC_SERVER_LIST)
		{
			// Check the notification code
			switch(HIWORD(wParam))
			{
			// An item was double clicked
			case LBN_DBLCLK:
				{
					connectButtonPressed();
					break;	
				}
			}
		}
		else if(LOWORD(wParam) == IDC_CONNECT_BUTTON)
		{
			connectButtonPressed();
		}
		else if(LOWORD(wParam) == IDC_REFRESH_BUTTON)
		{
			refresh();
		}
		else if(LOWORD(wParam) == IDC_HOST_BUTTON)
		{
			LobbyState::Instance()->hostServer();
		}
		break;
	}

	return 0;
}

void ServerList::connectButtonPressed()
{
	// Get selected index
	int index = SendMessage(mhList, LB_GETCURSEL, 0, 0);
					
	// An item must be selected
	if(index >= 0)	{
		// Connect to the selected server
		if(SendMessage(mhLanCheckBox, BM_GETSTATE,0,0) == BST_CHECKED)
			LobbyState::Instance()->connectToServer(mServerList[index].localIp);
		else
			LobbyState::Instance()->connectToServer(mServerList[index].publicIp);
	}
}