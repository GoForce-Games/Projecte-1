#include "ModulePuzzlePiecesV3.h"

ModulePuzzlePiecesV3::ModulePuzzlePiecesV3(bool startEnabled) : Module(startEnabled)
{
	dropDelay = MAX_DROP_DELAY;
	moveDelay = MAX_MOVE_DELAY;
	moveSpeed = MOVE_SPEED;
	gravity = GRAVITY;
	wallCount = 0;
}

ModulePuzzlePiecesV3::~ModulePuzzlePiecesV3()
{
}

bool ModulePuzzlePiecesV3::Init()
{
	for (uint i = 0; i < MAX_PIECES; i++)
	{
		pieces[i] = nullptr;
	}

	for (uint i = 0; i < MAX_WALLS; i++)
	{
		walls[i] = nullptr;
	}
	return true;
}

bool ModulePuzzlePiecesV3::Start()
{
	EnableDependencies(),
	LoadTextures();
	InitAnims();
	InitTemplates();
	playArea.Init();
	InitWalls();
	InitPlayers();
	InitMisc();

	return true;
}

Update_Status ModulePuzzlePiecesV3::Update()
{
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePuzzlePiecesV3::PostUpdate()
{
	return Update_Status::UPDATE_CONTINUE;
}

void ModulePuzzlePiecesV3::OnCollision(Collider* c1, Collider* c2)
{
}

bool ModulePuzzlePiecesV3::CleanUp()
{
	return true
		;
}

std::stack<PuzzlePiece*>& ModulePuzzlePiecesV3::GeneratePuzzlePieces(std::stack<PuzzlePiece*>& stack, uint amount)
{
	return stack;
}

PuzzlePiece* ModulePuzzlePiecesV3::AddPuzzlePiece(const PuzzlePiece& newPiece, Collider::Type type)
{
	return nullptr;
}

void ModulePuzzlePiecesV3::RemovePuzzlePiece(PuzzlePiece* piece)
{
}

bool ModulePuzzlePiecesV3::WillCollide(PlayerCollisionCheck direction)
{
	// TODO usar el modulo de colisiones existente
	return false;
}

void ModulePuzzlePiecesV3::PlacePieces()
{
}

bool ModulePuzzlePiecesV3::PieceCanDrop(PuzzlePiece* piece)
{
	return false;
}
