#ifndef ENUMS_H
#define ENUMS_H

#include <string>
using namespace std;

#define IDC_TRY_LOGIN		102
#define IDC_INVALID_LOGIN	103
#define IDC_STATIC_TEXT		105
#define IDC_RESIGN_BUTTON	106
#define IDC_APPLAUD_BUTTON	107
#define IDC_SERVER_LIST		108
#define IDC_CONNECT_BUTTON	109
#define IDC_REFRESH_BUTTON	110
#define IDC_HOST_BUTTON		111
#define IDC_ONLINE_BUTTON	112
#define IDC_LOCAL_BUTTON	113
#define IDC_HELP_BUTTON		114
#define IDC_ABOUT_BUTTON	115

enum ActionId
{
	WRONG_COLOR,
	INVALID_POSITION,
	GETS_CHECKED,
	PIECE_SELECTED,
	PIECE_MOVED,
	NOTHING
};

enum MessageId
{
	ID_PIECE_MOVED = 150,
	ID_REQUEST_REMATCH,
	ID_ACCEPT_REMATCH,
	ID_DECLINE_REMATCH,
	ID_LEFT_GAME,
	ID_MESSAGE_SENT,
	ID_REQUEST_NAME,
	ID_SENT_APPLAUD
};

//! The position on the game board.
struct Position
{
	Position()	{
		x = y = 0;
	}

	Position(int x, int y)	{
		this->x = x;
		this->y = y;
	}
	int x;
	int y;
};

//! Name and IP of a server.
struct Server
{
	string host;
	string ip;
};

//! The result of the players action.
struct ActionResult
{
	ActionResult()	{
		this->result = NOTHING;
		this->position = Position(-10, -10);
		this->from = Position(-10, -10);
	}
	ActionResult(ActionId result, Position pos, Position from = Position(0, 0))	{
		this->result = result;
		this->position = pos;
		this->from = from;
	}

	ActionId result;
	Position position;
	Position from;
};

enum PeerType
{
	HOST,
	CLIENT
};

enum StateId
{
	MENU,
	ABOUT,
	HELP,
	LOCAL,
	ONLINE
};

enum Color
{
	WHITE = -1,
	BLACK = 1,
	RED,
	GREEN,
	NONE
};

enum PieceType
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK, 
	QUEEN,
	KING
};



#endif