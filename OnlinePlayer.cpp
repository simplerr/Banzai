#include <fstream>
#include "common\DirectInput.h"
#include "common\Graphics.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "OnlinePlayer.h"
#include "Board.h"
#include "Piece.h"
#include "StatusText.h"
#include "PlayingOnline.h"
#include "MenuState.h"
#include "Input.h"
#include "resource1.h"
#include <Richedit.h>
#include <Commctrl.h>
#include "Textures.h"
#include "PiecesCaptured.h"
#include "Database.h"
#include "Enums.h"
#include "GUI.h"
#include "Sound.h"
#include <time.h>

void setFont(HWND hwnd, int height, int weight = FW_DONTCARE, string family="Arial");

//! Constructor.
/**
@param color The color of the player. Black or white.
*/
OnlinePlayer::OnlinePlayer(Color color) : Player(color)
{
	mCheckMate		= false;
	mWaitingOnAnswer= false;
	setBoard(new Board(80));
	srand(time(0));
}

//! Destructor
OnlinePlayer::~OnlinePlayer()
{
	delete getBoard();
}

//! Sets waiting status and color.
/**
@param type The type - either HOST or CLIENT.
@note Host is white and client black.
@note The host start (white starts).
*/
void OnlinePlayer::init(PeerType type)
{
	if(type == HOST)
	{
		mWaitingForOpponent = true;
		mWaitingOnMove = false;
		setColor(WHITE);
		mGui->setStatus("Waiting for opponent!", RED, 300.0f);
	}
	else if(type == CLIENT)
	{
		mWaitingForOpponent = false;
		mWaitingOnMove = true;
		setColor(BLACK);
	}
}

//! Polls action from player and sends it to the opponent.
/**
@param dt Time since last frame.
*/
void OnlinePlayer::update(float dt)
{
	// Is it the players turn?
	if(!mWaitingOnMove && !mWaitingForOpponent && !mCheckMate)
	{
		// Get action from the player
		ActionResult action = performAction();

		switch(action.result)
		{
			case PIECE_MOVED:	{
				// Place them off screen - not needed
				setSelected(-10, -10);
				setInvalid(-10, -10);

				// Send the move to the opponent
				// ID - piece.x - piece.y - new.x - new.y
				RakNet::BitStream bitstream;
				bitstream.Write((unsigned char)ID_PIECE_MOVED);
				bitstream.Write(action.from.x);
				bitstream.Write(action.from.y);
				bitstream.Write(action.position.x);
				bitstream.Write(action.position.y);

				PlayingOnline::Instance()->getPeer()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

				// Is now waiting on move
				mWaitingOnMove = true;

				pieceMovedSound();

				// Checkmate? NOTE: Important to check after the move is sent to opponent
				if(getBoard()->checkMate(Color(getColor()*-1)))	{
					mCheckMate = true;
					mGui->setStatus("Check mate!", GREEN, 100.0f);
					mGui->displayCheckMate(true);
					gDatabase->addWin(getName());
				}
				else
					mGui->setStatus("#NOVALUE", RED, 0.0f);

				break;
			}
			// Invalid position								
			case INVALID_POSITION:
				setInvalid(action.position.x, action.position.y);
				setSelected(-10, -10);
				mGui->setStatus("Invalid position!", RED, 2.0f);
				gSound->playEffect(ILLEGAL_SOUND);
				break;
			// Wrong color
			case WRONG_COLOR:
				setInvalid(action.position.x, action.position.y);
				if(getColor() == WHITE)
					mGui->setStatus("You are white", RED, 2.0f);
				else
					mGui->setStatus("You are black", RED, 2.0f);
				gSound->playEffect(ILLEGAL_SOUND);
				break;
			// Gets checked
			case GETS_CHECKED:
				setInvalid(action.position.x, action.position.y);
				setSelected(-10, -10);
				mGui->setStatus("Check!", RED, 2.0f);
				gSound->playEffect(ILLEGAL_SOUND);
				break;
			// Piece was selected
			case PIECE_SELECTED:
				setSelected(action.position.x, action.position.y);
				setInvalid(-10, -10);
				break;
		}
	}
	// Promt "Not your turn" if the player tries to move when he's waiting on the other player to move
	else if(!mWaitingForOpponent && !mCheckMate && mWaitingOnMove)	{
		if(gInput->keyPressed(VK_LBUTTON) && gInput->mousePosition().x < 740)	{
			mGui->setStatus("Not your turn!", RED, 2.0f);
			gSound->playEffect(ILLEGAL_SOUND);
		}
	}
}

//! Draws the board and gui.
void OnlinePlayer::draw()
{
	Player::draw();
	mGui->draw();	
}

//! Called when the opponent moved.
/**
@param from The old position.
@param to The new position.
*/
void OnlinePlayer::opponentMoved(Position from, Position to)
{
	// Get the piece perhaps gets captured
	Piece* piece = getBoard()->getPieceAt(to.x, to.y);
	if(piece != NULL)	{
		mGui->addCapture(piece->getColor(), piece->getType());
		getBoard()->removePiece(piece);
		pieceCapturedSound();
	}
	else
		pieceMovedSound();

	// Get the piece that moved
	Piece* movedPiece = getBoard()->getPieceAt(from.x, from.y);

	// Castling?
	if(movedPiece->getType() == KING && abs(from.x - to.x) == 2)
		gSound->playEffect(CASTLE_SOUND);

	movedPiece->setPos(to.x, to.y);

	// No longer waiting for the opponent to do a move
	mWaitingOnMove = false;

	// Checkmate?
	if(getBoard()->checkMate(getColor()))	{
		mCheckMate = true;
		mGui->displayCheckMate(false);
		gDatabase->addLoss(getName());
		mGui->setStatus("Check mate!", RED, 100.0f);
	}
	else
		mGui->setStatus("Your turn!", GREEN, 300.0f);

	
}

void OnlinePlayer::handleCastling(Piece* king)
{
	// Send the move to the opponent.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_PIECE_MOVED);

	Position from, to;

	// Hack...
	if(getColor() == WHITE)
	{
		if(king->getPos().x == 2)	{
			from = Position(0, 7);
			to = Position(3, 7);
		}
		else if(king->getPos().x == 6)	{
			from = Position(7, 7);
			to = Position(5, 7);
		}
	}
	else if(getColor() == BLACK)
	{
		if(king->getPos().x == 2)	{
			from = Position(0, 0);
			to = Position(3, 0);
		}
		else if(king->getPos().x == 6)	{
			from = Position(7, 0);
			to = Position(5, 0);
		}
	}	

	gSound->playEffect(CASTLE_SOUND);

	// Setup and send the message
	bitstream.Write(from.x);
	bitstream.Write(from.y);
	bitstream.Write(to.x);
	bitstream.Write(to.y);
	PlayingOnline::Instance()->getPeer()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
	
void OnlinePlayer::handleCapture(Color color, PieceType type)
{
	mGui->addCapture(color, type);
	pieceCapturedSound();
}

//! Ask the opponent for a rematch.
void OnlinePlayer::requestRematch()
{
	// Send rematch request to opponent.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_REQUEST_REMATCH);
	PlayingOnline::Instance()->getPeer()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
	
//! Restart the match.
void OnlinePlayer::restartMatch()
{
	mCheckMate = false;
	mWaitingOnAnswer = false;
	getBoard()->reset();
	if(getColor() == WHITE)
		mWaitingOnMove = false;
	else
		mWaitingOnMove = true;

	mGui->setStatus("Nothing", RED, 0.0f);

	if(getColor() == WHITE)
		mGui->setStatus("Your turn!", GREEN, 300.0f);

	mGui->clearCaptures();
	mGui->clearStats();
	mGui->initStats();
}

//! Sets waiting on opponent or not.
void OnlinePlayer::setWaitingForOpponent(bool waiting)
{
	mWaitingForOpponent = waiting;
}

//! Sets waiting on request answer or not.
void OnlinePlayer::setWaitingOnAnswer(bool waiting)
{
	mWaitingOnAnswer = waiting;
}

//! Set the gui.
void OnlinePlayer::setGui(GUI* gui)
{
	mGui = gui;
}

//! Returns waiting on opponent or not.
bool OnlinePlayer::waitingOnOpponent()
{
	return mWaitingForOpponent;
}
	
//! Returns waiting on request answer or not.
bool OnlinePlayer::waitingOnAnswer()
{
	return mWaitingOnAnswer;
}

//! Resturns if check mate or not.
bool OnlinePlayer::getCheckMate()
{
	return mCheckMate;
}

bool OnlinePlayer::waitingOnMove()
{
	return mWaitingOnMove;
}

void OnlinePlayer::pieceMovedSound()
{
	int random = rand() % 2;
	if(random == 0)
		gSound->playEffect(MOVE1_SOUND);
	else
		gSound->playEffect(MOVE2_SOUND);
}
	
void OnlinePlayer::pieceCapturedSound()
{
	int random = rand() % 3;

	if(random == 0)
		gSound->playEffect(CAPTURE1_SOUND);
	else if(random == 3)
		gSound->playEffect(CAPTURE2_SOUND);
	else if(random == 2)
		gSound->playEffect(CAPTURE3_SOUND);
}