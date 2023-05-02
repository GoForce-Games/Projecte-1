#pragma once

#include "PuzzlePiece.h"
#include "PlayerPiece.h"
#include "p2Point.h"

#define PLAY_AREA_X 10
#define PLAY_AREA_Y 13

class PlayArea
{
public:
	PlayArea();

	~PlayArea();

	void Init(PuzzlePiece* fillWith = nullptr);

	bool Update();

	void checkGroupedPieces();

	void explodeBombs();



public:
	iPoint position;

	//Array bidimensional de punteros a pieza
	PuzzlePiece* table[PLAY_AREA_X][PLAY_AREA_Y];
};

