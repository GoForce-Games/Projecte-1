#pragma once

#include "PuzzlePiece.h"
#include "PlayerPieceV2.h"
#include "p2Point.h"
#include <deque>

#define PLAY_AREA_W 10
#define PLAY_AREA_H 13

#define GROUP_MIN_COUNT 3

enum PlayAreaState {
	INIT,
	GAME_START,
	PIECES_PLACED,
	NEW_PIECES,
	PAUSE,

	GAME_END

};

enum CheckDirection {
	HORIZONTAL,
	VERTICAL,
};

class PlayArea
{
public:
	PlayArea();

	~PlayArea();

	void Init(PuzzlePiece* fillWith = nullptr);

	//Update player and table pieces
	bool Update();

	void RecursePieces(std::deque<PuzzlePiece*>& group, iPoint currPos, const PieceType& type, const CheckDirection& dir);

	void checkGroupedPieces();

	void explodeBombs();

	bool CleanUp();

	void debugPiecePosition();

	void DropPieces();

	bool PieceCanDrop(PuzzlePiece* pieceTop, PuzzlePiece* pieceBot);


public:
	PlayAreaState state;

	iPoint position;

	PlayerPieceV2* player = nullptr;

	//Array bidimensional de punteros a pieza, recorrer con las coordenadas invertidas (table[coord_y][coord_x])
	PuzzlePiece* table[PLAY_AREA_H][PLAY_AREA_W];

	Collider* collisionTester = nullptr;
};

