#include "PuzzlePiece.h"
#include "Application.h"

#include "Animation.h"
#include "Collider.h"
#include "ModulePuzzlePiecesV3.h"

const char* PuzzlePiece::enumLookup[] = {
	"NONE",
	"BLACK",
	"WHITE",
	"RED",
	"BLUE",
	"GREEN",
	"BOMB",
	"PRIMED_BOMB",
	"WALL",


	"MAX"
};

PuzzlePiece::PuzzlePiece() : position()
{
	type = PieceType::NONE;
}

PuzzlePiece::PuzzlePiece(const PuzzlePiece& p)
{
	moving = p.moving;
	position.create(p.position.x, p.position.y);
	collider = nullptr;
	currentAnimation = p.currentAnimation;
	texture = p.texture;
	isEmpty = p.isEmpty;
	type = p.type;
}

PuzzlePiece::~PuzzlePiece()
{
	if (!App->cleanUp && collider != nullptr) {
		collider->pendingToDelete = true;
	}
	collider = nullptr;
}

void PuzzlePiece::Update()
{
	currentAnimation.Update();
}

void PuzzlePiece::SetType(PieceType newType)
{
	type = newType;
	SetAnimation(&App->pieces->animIdle[type]);
}

// Borra la copia de la animación actual y asigna una nueva
// Esto permite que las piezas se animen por separado, aunque seguro que hay mejores formas de hacerlo
void PuzzlePiece::SetAnimation(Animation* newAnimation) {
	if (newAnimation != nullptr)
		currentAnimation = *newAnimation;
}
