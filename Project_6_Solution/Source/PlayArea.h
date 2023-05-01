#pragma once

#include "PuzzlePiece.h"
#include "PlayerPiece.h"

#define PLAY_AREA_X 10
#define PLAY_AREA_Y 13

class PlayArea
{
public:
	PlayArea();

	~PlayArea();

	void NewPieceSet(PlayerPiece* player);

	void checkGroupedPieces();

	void explodeBombs();

public:

	//Array bidimensional de punteros a pieza
	PuzzlePiece* table[PLAY_AREA_X][PLAY_AREA_Y];
};

