#include "common\DirectInput.h"
#include "Input.h"
#include "LocalPlayer.h"
#include "Board.h"
#include "Piece.h"
#include "Input.h"
#include "LocalState.h"

LocalPlayer::LocalPlayer(Color color) : Player(color)
{

}

LocalPlayer::~LocalPlayer()
{

}

void LocalPlayer::handleCastling(Piece* king)
{

}
	
void LocalPlayer::handleCapture(Color color, PieceType type)
{
	LocalState::Instance()->addCapturedPiece(color, type);
}