#pragma once

#define PLAY_AREA_X 10
#define PLAY_AREA_Y 13

class PuzzlePiece;
class PlayerPiece;

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

