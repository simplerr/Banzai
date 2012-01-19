#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "common\d3dUtil.h"
#include "common\GameState.h"

class Menu;
class Game;

//! Active when in the main menu.
class MenuState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();

	bool menuHandler(std::string name);
	void destroyWindows();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	static MenuState* Instance() {
		return &mMenuState;
	}
protected:
	MenuState() {};
private:
	static MenuState mMenuState;
	IDirect3DTexture9* mBkgd;
	IDirect3DTexture9* mLogo;
	HWND mhOnlineButton;
	HWND mhLocalButton;
	HWND mhAboutButton;
	int mLobbyDelay;	// NOTE: Hack to avoid white rectangles where a window was destroyed
};

#endif