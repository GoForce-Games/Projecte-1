#include "PlayerPieceV2.h"

#include "Collider.h"
#include "Path.h"

PlayerPieceV2::PlayerPieceV2()
{
	pieces[0][0] = nullptr;
	pieces[0][1] = nullptr;
	pieces[1][0] = nullptr;
	pieces[1][1] = nullptr;

	piecePaths[0][0] = new Path();
	piecePaths[0][1] = new Path();
	piecePaths[1][0] = new Path();
	piecePaths[1][1] = new Path();

	piecePaths[0][0]->name = "TopLeft";
	piecePaths[0][1]->name = "TopRight";
	piecePaths[1][0]->name = "BotLeft";
	piecePaths[1][1]->name = "BotRight";



	// Impide que se mueva si el jugador no le da a la tecla de rotacion
	piecePaths[0][0]->PushBack({ 0,0 }, 1);
	piecePaths[0][1]->PushBack({ 0,0 }, 1);
	piecePaths[1][0]->PushBack({ 0,0 }, 1);
	piecePaths[1][1]->PushBack({ 0,0 }, 1);

	piecePaths[0][0]->PushBack({ 4,0 }, 3);
	piecePaths[0][1]->PushBack({ 0,4 }, 3);
	piecePaths[1][0]->PushBack({ 0,-4 }, 3);
	piecePaths[1][1]->PushBack({ -4,0 }, 3);

	piecePaths[0][0]->loop = false;
	piecePaths[0][1]->loop = false;
	piecePaths[1][0]->loop = false;
	piecePaths[1][1]->loop = false;

	piecePaths[0][0]->finished = true;
	piecePaths[0][1]->finished = true;
	piecePaths[1][0]->finished = true;
	piecePaths[1][1]->finished = true;
}

PlayerPieceV2::PlayerPieceV2(PuzzlePiece* pieces[4])
{
	setPieces(pieces);
}

PlayerPieceV2::~PlayerPieceV2()
{
	CleanUp();
}

bool PlayerPieceV2::Start()
{
	return true;
}

bool PlayerPieceV2::CleanUp()
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

bool PlayerPieceV2::Rotate()
{
	return true;
}

bool PlayerPieceV2::Update()
{
	return true;
}

void PlayerPieceV2::setPieces(PuzzlePiece* newPieces[4])
{
}
