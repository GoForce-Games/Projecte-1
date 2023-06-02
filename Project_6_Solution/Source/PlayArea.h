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
	GAME_LOOP,
	PIECES_PLACED,
	CHECK_GROUPS,
	DELETE_GROUPS,
	DETONATE_BOMBS,
	NEW_PIECES,
	PAUSE,

	GAME_END,
	NEW_GAME

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

	bool checkGroupedPieces();

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

	// Posiciones de las piezas a quitar
	std::deque<PuzzlePiece*> piecesToRemove;

	Collider* collisionTester = nullptr;
};

