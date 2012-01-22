//#include <crtdbg.h>
#include <time.h>
#include <fstream>
#include <time.h>
#include <string>

#include "common\Runnable.h"
#include "common\Graphics.h"
#include "common\DirectInput.h"

#include "Sound.h"
#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include "Player.h"
#include "MenuState.h"
#include "Input.h"
#include "mysql++.h"
#include "Textures.h"
#include "Database.h"
#include "PlayingOnline.h"


// Set the globals
Runnable*			gGame		= 0;
IDirect3DDevice9*	gd3dDevice	= 0;
Graphics*			gGraphics	= 0;
Input*				gInput		= 0;
Textures*			gTextures	= 0;
Database*			gDatabase	= 0;
Sound*				gSound		= 0;

// Use Windows 7 looking controls.
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd){
	//InitAllocCheck(ACOutput_XML); // ACOutput_XML

	// Enable run-time memory check for debug builds.
	//#if defined(DEBUG) | defined(_DEBUG)
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//#endif
	//
	Game game(hInstance, "Banzai v0.1", 1090, 710, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gGame = &game;

	gInput = new Input();
	
	gGame->init();

	// Run the game
	return gGame->run();
}

//! Constructor.
Game::Game(HINSTANCE hInstance, std::string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP)
	:Runnable(hInstance, caption, width, height, devType, requestedVP)
{
	gGraphics = new Graphics();
	gTextures = new Textures();
	gSound = new Sound();
	mCamera = new Camera(DIMENSION_3D, 0, 0, getScreenWidth(), getScreenHeight());
	mCamera->setPosition(0, 0, 500);
	mCamera->setTarget(0, 0, 0);
	mCamera->update();
	mGfxStats = new GfxStats();
}

//! Destructor.
Game::~Game()
{
	mGameState->cleanup();
	delete mCamera;
	delete mGfxStats;
	delete gInput;
	delete gTextures;
	delete gDatabase;
	delete gSound;
}

//! Inits all game related content.
void Game::init()
{
	mGameState = NULL;
	changeState(MenuState::Instance());
}

//! Changes the state.
void Game::changeState(GameState* state)
{
	if(mGameState != NULL)
		mGameState->cleanup();

	mGameState = state;
	mGameState->init(this);
}

//! Updates everything.
void Game::update(float t, float dt)
{
	gInput->update(dt);
	mGameState->update(dt);
	mGfxStats->update(dt);
}
	
//! Draws everything.
void Game::draw()
{
	mGameState->draw();
	//gInput->draw();
}

//! The starting msgProc function.
LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = gInput->msgProc(msg, wParam, lParam);
	result = mGameState->msgProc(msg, wParam, lParam);
	result = Runnable::msgProc(msg, wParam, lParam);

	return result;
}

void Game::onLostDevice()
{
	gGraphics->onLostDevice();
	mGfxStats->onLostDevice();
}
void Game::onResetDevice()
{
	gGraphics->onResetDevice();
	mGfxStats->onResetDevice();
}