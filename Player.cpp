#include "Player.h"

#include "common\DirectInput.h"
#include "Input.h"
#include "Player.h"
#include "Board.h"
#include "Piece.h"
#include "Input.h"
#include "LocalState.h"

//! Constructor.
Player::Player(Color color)
{
	mSelectedPiece = NULL;
	mColor		= color;
	mName		= "#NOVALUE";
	mOpponent	= "#NOVALUE";
	mBoard		= NULL;
}

//! Destructor.
Player::~Player()
{
	
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
			}
			else	{
				mSelectedPiece = piece;
				mBoard->setSelected(pos.x, pos.y);
				// Return piece selected msg
				action = ActionResult(PIECE_SELECTED, pos);
			}
		}
	}
	// A piece is already selected
	else if(gInput->keyPressed(VK_LBUTTON) && mSelectedPiece != NULL)
	{
		Piece* piece = mBoard->getPieceAt(pos.x, pos.y);
		// Was it a piece of the same color that was pressed? - Change the selected piece
		if(piece != NULL && piece->getColor() == getColor())
		{
			mSelectedPiece = piece;
			action = ActionResult(PIECE_SELECTED, pos);
		}
		// Didn't select a piece of the same color as self
		else
		{
			// The piece can move to the pressed location
			if(mBoard->validMove(mSelectedPiece, pos.x, pos.y))	
			{
				// A piece was captured
				if(piece != NULL)	{
					handleCapture(piece->getColor(), piece->getType());
					mBoard->removePiece(piece);
				}

				// Return piece moved msg
				action = ActionResult(PIECE_MOVED, pos, mSelectedPiece->getPos());

				// Update the selected piece's position
				mSelectedPiece->setPos(pos.x, pos.y);
				mSelectedPiece->moved();

				// Was it castling?
				if(mSelectedPiece->getType() == KING && abs(action.from.x - action.position.x) == 2)
					handleCastling(mSelectedPiece);

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
				mSelectedPiece = NULL;
			}
		}
	}

	// Return the action
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
