#include "ModulePuzzlePiecesV2.h"

ModulePuzzlePiecesV2::ModulePuzzlePiecesV2(bool startEnabled)
{
}

ModulePuzzlePiecesV2::~ModulePuzzlePiecesV2()
{
}

bool ModulePuzzlePiecesV2::Start()
{
	return false;
}

Update_Status ModulePuzzlePiecesV2::Update()
{
	return Update_Status();
}

Update_Status ModulePuzzlePiecesV2::PostUpdate()
{
	return Update_Status();
}

void ModulePuzzlePiecesV2::OnCollision(Collider* c1, Collider* c2)
{
}

bool ModulePuzzlePiecesV2::CleanUp()
{
	return false;
}

PuzzlePiece* ModulePuzzlePiecesV2::AddPuzzlePiece(const PuzzlePiece& newPiece)
{
	return nullptr;
}

void ModulePuzzlePiecesV2::RemovePuzzlePiece(PuzzlePiece* piece)
{
}

bool ModulePuzzlePiecesV2::WillCollide(iPoint position)
{
	return false;
}
