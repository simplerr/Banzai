#include "Player.h"
#include <time.h>
#include "common\DirectInput.h"
#include "Input.h"
#include "Player.h"
#include "Board.h"
#include "Piece.h"
#include "Input.h"
#include "LocalState.h"
#include "Sound.h"
#include "common\Graphics.h"

//! Constructor.
Player::Player(Color color)
{
	mSelectedPiece = NULL;
	mColor		= color;
	mName		= "#NOVALUE";
	mOpponent	= "#NOVALUE";
	mBoard		= NULL;
	srand(time(0));
	mLine = gGraphics->loadTexture("imgs\\line.png");
	mArrow = gGraphics->loadTexture("imgs\\arrow.png");
	setLastMove();
}

//! Destructor.
Player::~Player()
{
	ReleaseCOM(mLine);
	ReleaseCOM(mArrow);
}

//! Updates components.
/**
@note Get overriden.
*/
void Player::update(float dt)
{

}

//! Draws components.
/**
@note Get overriden.
*/
void Player::draw()
{
	mBoard->draw();

	// Draw the last move effect
	Vector from = mBoard->toGlobalPos(mLastMove.from);
	Vector to = mBoard->toGlobalPos(mLastMove.to);
	Vector diff = to - from;
	float angle = atan2f(diff.y, diff.x);

	gGraphics->drawTexturedLine(mLine, mBoard->toGlobalPos(mLastMove.from), mBoard->toGlobalPos(mLastMove.to), 20.0f);

	// The arrow
	int width = 32;
	int height = 64;
	gGraphics->drawTexture(mArrow, mBoard->toGlobalPos(mLastMove.to).x + cosf(angle)*(width/2), mBoard->toGlobalPos(mLastMove.to).y + sinf(angle)*(width/2), width, height, angle);
}

//! Finds out what the result of the players action is.
/**
@return The result of the action the player made. WRONG_COLOR, PIECE_SELECTED, PIECE_MOVED, INVALID_POSITION.
*/
ActionResult Player::performAction()
{
	// Translate mouse coordinate to sqaure coordinates
	Position pos = mBoard->toGridPos(gInput->mousePosition());
	ActionResult action;
	bool moved = false;

	// No piece is selected
	if(gInput->keyPressed(VK_LBUTTON) && mSelectedPiece == NULL)
	{
		Piece* piece = mBoard->getPieceAt(pos.x, pos.y);
		// A piece was pressed
		if(piece != NULL)
		{
			// The piece has another color than the players
			if(piece->getColor() != getColor())	{
				// Return wrong color msg
				action = ActionResult(WRONG_COLOR, pos);
				gSound->playEffect(ILLEGAL_SOUND);
			}
			else	{
				mSelectedPiece = piece;
				mBoard->setSelected(pos.x, pos.y);
				// Return piece selected msg
				action = ActionResult(PIECE_SELECTED, pos);

				mBoard->setMovingOffset(getMouseOffset());
				SetCursor(LoadCursor(0, IDC_HAND));
			}
		}
	}
	// A piece is already selected
	else if(gInput->keyPressed(VK_LBUTTON) && mSelectedPiece != NULL)
	{
		mBoard->setMovingOffset(getMouseOffset());
		action = moveSelectedPiece(pos);
	}

	if(gInput->keyDown(VK_LBUTTON) && mSelectedPiece != NULL)
	{
		mSelectedPiece->setMoving(true);
	}
	else if(gInput->keyReleased(VK_LBUTTON) && mSelectedPiece != NULL)
	{
		mSelectedPiece->setMoving(false);
		action = moveSelectedPiece(mBoard->toGridPos(gInput->mousePosition() + 0*mBoard->getMovingOffset()));
		SetCursor(LoadCursor(0, IDC_ARROW));
	}

	// Return the action
	return action;
}

Vector Player::getMouseOffset()
{
	Vector tmp;
	tmp.x = mSelectedPiece->getPos().x;
	tmp.y = mSelectedPiece->getPos().y;

	tmp.x = tmp.x*mBoard->getSquareSize() + 20 + mBoard->getSquareSize()/2;
	tmp.y = tmp.y*mBoard->getSquareSize() + 20 + mBoard->getSquareSize()/2;

	tmp -= gInput->mousePosition();

	return tmp;
}

//! Try to move the selected piece to (pos.x;pos.y).
ActionResult Player::moveSelectedPiece(Position pos)
{
	ActionResult action;

	Piece* piece = mBoard->getPieceAt(pos.x, pos.y);
	Position oldPos = mSelectedPiece->getPos();
	// Was it a piece of the same color that was pressed? - Change the selected piece
	if(piece != NULL && piece->getColor() == getColor())
	{
		mSelectedPiece = piece;
		mBoard->setMovingOffset(getMouseOffset());
		action = ActionResult(PIECE_SELECTED, pos);
		SetCursor(LoadCursor(0, IDC_HAND));
	}
	// Didn't select a piece of the same color as self
	else
	{
		// The piece can move to the pressed location
		if(mBoard->validMove(mSelectedPiece, pos.x, pos.y))	
		{
			// Return piece moved msg
			action = ActionResult(PIECE_MOVED, pos, mSelectedPiece->getPos());

			// Update the selected piece's position
			mSelectedPiece->setPos(pos.x, pos.y);
			mSelectedPiece->moved();

			// A piece was captured
			if(piece != NULL)	{
				handleCapture(piece->getColor(), piece->getType());
				mBoard->removePiece(piece);
				pieceCapturedSound();
			}
			// Castling?
			else if(mSelectedPiece->getType() == KING && abs(mSelectedPiece->getPos().x - oldPos.x) > 1)	{
				handleCastling(mSelectedPiece);
				gSound->playEffect(CASTLE_SOUND);
			}
			// Normal move
			else
				pieceMovedSound();

			mSelectedPiece = NULL;
		}
		// Can't move to the pressed location
		else
		{
			// Return invalid position or checked msg
			if(mBoard->resultsInCheck(mSelectedPiece, pos.x, pos.y))
				action = ActionResult(GETS_CHECKED, mBoard->getPiece(KING, mSelectedPiece->getColor())->getPos());
			else	{
				if(pos.x >= 0 && pos.x <= 7 && pos.y >= 0 && pos.y <= 7)
					action = ActionResult(INVALID_POSITION, pos);
				else
					action = ActionResult(PIECE_SELECTED, Position(-10, -10));
			}
			gSound->playEffect(ILLEGAL_SOUND);
			mSelectedPiece = NULL;
		}
	}

	return action;
}

//! Get the color.
Color Player::getColor()
{
	return mColor;
}

//! Handle castling.
/**
@note Gets overriden.
*/
void Player::handleCastling(Piece* king)
{
	
}
	
//! Handle capturing.
/**
@note Gets overriden.
*/
void Player::handleCapture(Color color, PieceType type)
{

}

//! Set the board.
void Player::setBoard(Board* board)
{
	mBoard = board;
}

//! Sets the color.
void Player::setColor(Color color)
{
	mColor = color;
}

//! Set on which sqaure to draw the green selected piece effect.
void Player::setSelected(int x, int y)
{
	mBoard->setSelected(x, y);
}

//! Set on which square to draw the red invalid move effect.
void Player::setInvalid(int x, int y)
{
	mBoard->setInvalid(x, y);
}

//! Ask the board if it's check mate.
bool Player::getCheckMate()
{
	return mBoard->checkMate(getColor());
}

//! Reset the board and restore all pieces.
void Player::resetBoard()
{
	mBoard->reset();
}

//! Sets the player name.
void Player::setName(string name)
{
	mName = name;
}

//! Get the player name.
string Player::getName()
{
	return mName;
}

//! Set the opponent name.
void Player::setOpponent(string name)
{
	mOpponent = name;
}

//! Returns the name of the opponent.
string Player::getOpponent()
{
	return mOpponent;
}

//! Returns the board.
Board*	Player::getBoard()
{
	return mBoard;
}

void Player::pieceMovedSound()
{
	int random = rand() % 2;
	if(random == 0)
		gSound->playEffect(MOVE1_SOUND);
	else
		gSound->playEffect(MOVE2_SOUND);
}
	
void Player::pieceCapturedSound()
{
	int random = rand() % 3;

	if(random == 0)
		gSound->playEffect(CAPTURE1_SOUND);
	else if(random == 1)
		gSound->playEffect(CAPTURE2_SOUND);
	else if(random == 2)
		gSound->playEffect(CAPTURE3_SOUND);
}

void Player::setLastMove(Position from, Position to)
{
	mLastMove.from = from;
	mLastMove.to = to;
}

LastMove Player::getLastMove()
{
	return mLastMove;
}