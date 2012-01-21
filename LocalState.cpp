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

	mBoard			= new Board(90);
	mActivePlayer	= new LocalPlayer(WHITE);
	mPlayer2		= new LocalPlayer(BLACK);
	mPiecesCaptured = new PiecesCaptured(800, 50);

	// Both players uses the same board
	mActivePlayer->setBoard(mBoard);
	mPlayer2->setBoard(mBoard);
	mSelectedPiece	= NULL;

	// Create the status text
	mStatusText		=	StatusText("Invalid position!", 280, 328, 0.0f);
	
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
			gSound->playEffect(ILLEGAL_SOUND);
			break;
		case WRONG_COLOR:
			mActivePlayer->setInvalid(actionResult.position.x, actionResult.position.y);
			if(mActivePlayer->getColor() == WHITE)
				mStatusText.setText("You are white", RED, 2.0f);
			else
				mStatusText.setText("You are black", RED, 2.0f);
			gSound->playEffect(ILLEGAL_SOUND);
			break;
		case GETS_CHECKED:
			mActivePlayer->setInvalid(actionResult.position.x, actionResult.position.y);
			mActivePlayer->setSelected(-10, -10);
			mStatusText.setText("Check!", RED, 2.0f);
			gSound->playEffect(ILLEGAL_SOUND);
			break;
		case PIECE_SELECTED:
			mActivePlayer->setSelected(actionResult.position.x, actionResult.position.y);
			mActivePlayer->setInvalid(-10, -10);
			break;
	}

	// Ask the user if he really wants to leave when after he pressed enter
	if(gInput->keyPressed(VK_ESCAPE))	{
		int result = MessageBox(0, "Are you sure you want to exit the match?", "Leaving match", MB_YESNO | MB_ICONQUESTION);
		if(result == IDYES)
			changeState(MenuState::Instance());
	}
}

void LocalState::draw()
{
	gGraphics->drawTexture(mBkgd, 600, 400, 1200, 800);		
	
	mActivePlayer->draw();
	mStatusText.draw();

	// Draw the captured pieces and a background behind them
	gGraphics->drawTexture(mGreyBkgd, 965, 115, 377, 190);
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
	int result = MessageBox(0, buffer.c_str(), "Check mate!", MB_YESNO | MB_ICONQUESTION);

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
	}
	else	{
		// No - change state
		changeState(MenuState::Instance());
	}
}

void LocalState::pieceMovedSound()
{
	int random = rand() % 2;
	if(random == 0)
		gSound->playEffect(MOVE1_SOUND);
	else
		gSound->playEffect(MOVE2_SOUND);
}
	
void LocalState::pieceCapturedSound()
{
	int random = rand() % 3;

	if(random == 0)
		gSound->playEffect(CAPTURE1_SOUND);
	else if(random == 3)
		gSound->playEffect(CAPTURE2_SOUND);
	else if(random == 2)
		gSound->playEffect(CAPTURE3_SOUND);
}
