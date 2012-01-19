#ifndef ONLINEPLAYER_H
#define ONLINEPLAYER_H

#include "RakPeerInterface.h"
#include "Enums.h"
#include "StatusText.h"
#include "Player.h"

class Board;
class Piece;
class PiecesCaptured;
class GUI;
class Database;

//! Contains the game board and handles user actions.
class OnlinePlayer : public Player
{
public:
	OnlinePlayer(Color color);
	~OnlinePlayer();

	void update(float dt);
	void draw();

	void handleCastling(Piece* king);
	void handleCapture(Color color, PieceType type);

	void setGui(GUI* gui);

	void init(PeerType type);

	void	opponentMoved(Position from, Position to);

	bool	getCheckMate();	// NOTE: Needed?

	void requestRematch();
	void restartMatch();
	void setWaitingForOpponent(bool waiting);
	void setWaitingOnAnswer(bool waiting);

	bool waitingOnOpponent();
	bool waitingOnAnswer();
	bool waitingOnMove();
private:
	GUI*		mGui;

	bool		mWaitingForOpponent;
	bool		mWaitingOnMove;
	bool		mCheckMate;
	bool		mWaitingOnAnswer;
};

#endif