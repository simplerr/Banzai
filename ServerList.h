#pragma once

#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

struct Server;

//! Contains a list of all online games.
class ServerList
{
public:
	ServerList(int x, int y, int width, int height);
	~ServerList();

	void refresh();
	void addServer(string host, string ip);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	void createListControl();

	vector<Server> mServerList;
	HWND mhList;
	HWND mhPanel;
	HWND mhRefreshButton;
	HWND mhConnectButton;
	HWND mhHostButton;
	HWND mhLabel;

	int mX, mY;
	int mWidth, mHeight;
};