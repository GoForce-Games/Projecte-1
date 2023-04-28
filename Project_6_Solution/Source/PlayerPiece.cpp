#include "PlayerPiece.h"
#include "Path.h"

PlayerPiece::PlayerPiece()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			pieces[i][j] = nullptr;
		}
	}

}

PlayerPiece::PlayerPiece(PuzzlePiece* pieces_[3])
{
	pieces[0][0] = pieces_[0];
	pieces[0][1] = pieces_[1];
	pieces[1][0] = pieces_[2];
	pieces[1][1] = nullptr;
}

PlayerPiece::~PlayerPiece()
{
	CleanUp();
}

bool PlayerPiece::Start()
{
	return false;
}

bool PlayerPiece::CleanUp()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (pieces[i][j] != nullptr)
				pieces[i][j] = nullptr; // La limpieza de piezas la hace el modulo principal
		}
	}
	return true;
}

bool PlayerPiece::Rotate(bool clockwise)
{
	return false;
}

bool PlayerPiece::Update()
{
	return false;
}

void PlayerPiece::setPieces(PuzzlePiece* newPieces[3])
{
}
