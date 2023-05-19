#pragma once

#include "PuzzlePiece.h"
#include "PlayerPieceV2.h"
#include "p2Point.h"
#include <deque>

#define PLAY_AREA_X 10
#define PLAY_AREA_Y 13

#define GROUP_MIN_COUNT 3

class PlayArea
{
public:
	PlayArea();

	~PlayArea();

	void Init(PuzzlePiece* fillWith = nullptr);

	bool Update();

	void PlayArea::RecurseGroups(std::deque<iPoint>& group, iPoint currPos, PieceType type);

	void checkGroupedPieces();

	void explodeBombs();

	bool CleanUp();

	void PlayArea::debugPiecePosition();

	void DropPieces();

	bool PieceCanDrop(PuzzlePiece* pieceTop, PuzzlePiece* pieceBot);


public:
	PuzzlePiece* watched = nullptr;

	iPoint position;

	//Array bidimensional de punteros a pieza
	PuzzlePiece* table[PLAY_AREA_X][PLAY_AREA_Y];

	Collider* collisionTester = nullptr;
};

