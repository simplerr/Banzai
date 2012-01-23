#include "common\Graphics.h"
#include "common\Menu.h"
#include "common\DirectInput.h"
#include "LocalState.h"
#include "MenuState.h"
#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include "LocalPlayer.h"
#include "Input.h"
#include "PiecesCaptured.h"
#include "Sound.h"

LocalState LocalState::mLocalState;

void LocalState::init(Game* game)
{
	// important!
	setGame(game);

	mBkgd			= gGraphics->loadTexture("imgs\\bkgd.bmp");
	mGreyBkgd		= gGraphics->loadTexture("imgs\\grey_bkgd.bmp");

	mBoard			= new Board(80);
	mActivePlayer	= new LocalPlayer(WHITE);
	mPlayer2		= new LocalPlayer(BLACK);
	mPiecesCaptured = new PiecesCaptured(700, 40);

	// Both players uses the same board
	mActivePlayer->setBoard(mBoard);
	mPlayer2->setBoard(mBoard);
	mSelectedPiece	= NULL;

	// Create the status text
	mStatusText		=	StatusText("Invalid position!", 675, 220, 0.0f);
	gSound->playEffect(NEW_GAME_SOUND);
}

void LocalState::cleanup()
{
	ReleaseCOM(mBkgd);
	ReleaseCOM(mGreyBkgd);
	delete mBoard;
	delete mActivePlayer;
	delete mPlayer2;
	delete mPiecesCaptured;
}

void LocalState::update(double dt)
{
	// Update the status text
	mStatusText.update(dt);

	// Get the result of the players action.
	ActionResult actionResult =  mActivePlayer->performAction();

	// Switch the result
	switch(actionResult.result)
	{
		case PIECE_MOVED:	{
			LocalPlayer* tmp = mPlayer2;
			mPlayer2 = mActivePlayer;
			mActivePlayer = tmp;
			mActivePlayer->setSelected(-10, -10);
			mActivePlayer->setInvalid(-10, -10);

			// Send the last move to the other player NOTE: Kinda ugly..
			mActivePlayer->setLastMove(actionResult.from, actionResult.position);

			// Checkmate?
			if(mActivePlayer->getCheckMate())	{
				displayCheckMate();
			}
			
			break;
		}
		case INVALID_POSITION:
			mActivePlayer->setInvalid(actionResult.position.x, actionResult.position.y);
			mActivePlayer->setSelected(-10, -10);
			mStatusText.setText("Invalid position!", RED, 2.0f);
			break;
		case WRONG_COLOR:
			mActivePlayer->setInvalid(actionResult.position.x, actionResult.position.y);
			if(mActivePlayer->getColor() == WHITE)
				mStatusText.setText("You are white", RED, 2.0f);
			else
				mStatusText.setText("You are black", RED, 2.0f);
			break;
		case GETS_CHECKED:
			mActivePlayer->setInvalid(actionResult.position.x, actionResult.position.y);
			mActivePlayer->setSelected(-10, -10);
			mStatusText.setText("Check!", RED, 2.0f);
			break;
		case PIECE_SELECTED:
			mActivePlayer->setSelected(actionResult.position.x, actionResult.position.y);
			mActivePlayer->setInvalid(-10, -10);
			break;
	}

	// Ask the user if he really wants to leave when after he pressed enter
	if(gInput->keyPressed(VK_ESCAPE))	{
		int result = MessageBox(gGame->getMainWnd(), "Are you sure you want to exit the match?", "Leaving match", MB_YESNO | MB_ICONQUESTION);
		if(result == IDYES)	{
			gSound->playEffect(OPPONENT_LEAVE_SOUND);
			changeState(MenuState::Instance());
		}
	}
}

void LocalState::draw()
{
	gGraphics->drawTexture(mBkgd, 600, 400, 1200, 800);		
	
	mActivePlayer->draw();
	mStatusText.draw();

	// Draw the captured pieces and a background behind them

	// Gray backround behind the captured pieces
	gGraphics->drawTexture(mGreyBkgd, 865, 115, 377, 190);
	mPiecesCaptured->draw();
}

//! Add a captured piece.
void LocalState::addCapturedPiece(Color color, PieceType type)
{
	mPiecesCaptured->add(color, type);
}

//! Display check mate message and ask for rematch.
void LocalState::displayCheckMate()
{
	// Create the text to display
	string buffer = "White wins!\nDo you want to play again?";

	if(mActivePlayer->getColor() == WHITE)
		buffer = "Black wins!\nDo you want to play again?";

	// Promt message box
	int result = MessageBox(gGame->getMainWnd(), buffer.c_str(), "Check mate!", MB_YESNO | MB_ICONQUESTION);

	// Rematch?
	if(result == IDYES)	{
		mActivePlayer->resetBoard();
		mPiecesCaptured->clear();
		if(mActivePlayer->getColor() == BLACK)	{
			// White starts
			LocalPlayer* tmp = mActivePlayer;
			mActivePlayer = mPlayer2;
			mPlayer2 = tmp;
		}

		gSound->playEffect(NEW_GAME_SOUND);
	}
	else	{
		// No - change state
		changeState(MenuState::Instance());
	}
}
