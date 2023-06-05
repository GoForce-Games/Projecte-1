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
	ANIMATE_DELETION,
	DELETE_GROUPS,
	ADD_BOMBS,
	DETONATE_BOMBS,
	NEW_PIECES,
	PAUSE,

	GAME_END,
	BACK_TO_TITLE

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
	
	// Devuelve true si hay espacio debajo de las piezas
	bool PlaceBomb(int col);

	PuzzlePiece* GetPiece(int x, int y);


public:
	PlayAreaState state;

	iPoint position;

	PlayerPieceV2* player = nullptr;

	//Array bidimensional de punteros a pieza, recorrer con las coordenadas invertidas (table[coord_y/Fila][coord_x/Columna])
	PuzzlePiece* table[PLAY_AREA_H][PLAY_AREA_W];

	// Posiciones de las piezas a quitar
	std::deque<PuzzlePiece*> piecesToRemove;

	Collider* collisionTester = nullptr;

	uint explosionRange;
	
	uint bombsToSpawn;

	uint bigBombCharge;
};

