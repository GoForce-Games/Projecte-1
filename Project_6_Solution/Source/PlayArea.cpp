#include "PuzzlePiece.h"
#include "PlayArea.h"
#include "Globals.h"
#include "PlayerPiece.h"

PlayArea::PlayArea()
{
	Init();
}

PlayArea::~PlayArea()
{
}

void PlayArea::Init(PuzzlePiece* fillWith)
{
	for (uint i = 0; i < PLAY_AREA_X; i++)
	{
		for (uint j = 0; j < PLAY_AREA_Y; j++)
		{
			table[i][j] = fillWith;
		}
	}
}

bool PlayArea::Update()
{
	for (size_t i = 0; i < PLAY_AREA_X; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_Y; j++)
		{
			if (table[i][j] != nullptr)
				table[i][j]->Update();
		}
	}

	return true;
}

/*
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
*/

void PlayArea::checkGroupedPieces()
{
}

void PlayArea::explodeBombs()
{
}
