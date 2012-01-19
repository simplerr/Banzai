#pragma once
#include "Enums.h"
#include "Player.h"
#include <string>
using namespace std;

class Board;
class Piece;

//! The player when not playing online.
class LocalPlayer : public Player
{
public:
	LocalPlayer(Color color);
	~LocalPlayer();

	void handleCastling(Piece* king);
	void handleCapture(Color color, PieceType type);
private:
};
