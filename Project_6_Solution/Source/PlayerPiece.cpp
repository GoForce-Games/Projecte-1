#include "PlayerPiece.h"
#include "Path.h"

PlayerPiece::PlayerPiece()
{
	pieces[0][0] = nullptr;
	pieces[0][1] = nullptr;
	pieces[1][0] = nullptr;
	pieces[1][1] = nullptr;

	piecePaths[0][0] = new Path();
	piecePaths[0][1] = new Path();
	piecePaths[1][0] = new Path();
	piecePaths[1][1] = new Path();

	piecePaths[0][0]->PushBack({4,0},4);
	piecePaths[0][1]->PushBack({0,4},4);
	piecePaths[1][0]->PushBack({-4,0},4);
	piecePaths[1][1]->PushBack({0,-4},4);
}

PlayerPiece::PlayerPiece(PuzzlePiece* pieces_[3]) : PlayerPiece()
{	
	pieces[0][0] = pieces_[0];
	pieces[0][1] = pieces_[1];
	pieces[1][0] = pieces_[2];
}

PlayerPiece::~PlayerPiece()
{
	CleanUp();
}

bool PlayerPiece::Start()
{
	return true;
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
	pieces[0][0] = newPieces[0];
	pieces[0][1] = newPieces[1];
	pieces[1][0] = newPieces[2];
	pieces[1][1] = nullptr;
}
