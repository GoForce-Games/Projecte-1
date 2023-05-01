#include "PlayArea.h"
#include "Globals.h"
#include "PuzzlePiece.h"
#include "PlayerPiece.h"

PlayArea::PlayArea()
{
	for (uint i = 0; i < PLAY_AREA_X; i++)
	{
		for (uint j = 0; j < PLAY_AREA_Y; j++)
		{	
			table[i][j] = nullptr;
		}
	}
}

PlayArea::~PlayArea()
{
}

void PlayArea::NewPieceSet(PlayerPiece* player)
{
	PuzzlePiece newPiece;
	newPiece.moving = true;
	PuzzlePiece** pieceArray = new PuzzlePiece*[3];
	for (uint i = 0; i < 3; i++)
	{
		pieceArray[i] = new PuzzlePiece(newPiece);
	}
	player->setPieces(pieceArray);
}

void PlayArea::checkGroupedPieces()
{
}

void PlayArea::explodeBombs()
{
}
