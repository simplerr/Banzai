#pragma once

#include "Enums.h"
#include <vector>
using namespace std;

//! Holds and display all captured pieces.
class PiecesCaptured
{
public:
	PiecesCaptured(int x, int y, int piecesPerRow = 8);
	~PiecesCaptured();

	void clear();
	void draw();
	void add(Color color, PieceType type);
private:
	vector<PieceType> mWhiteCaptured;
	vector<PieceType> mBlackCaptured;
	int		mPiecesPerRow;
	int		mX, mY;
};