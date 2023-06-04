#include "PuzzlePiece.h"
#include "Application.h"

#include "Animation.h"
#include "Collider.h"
#include "ModulePuzzlePiecesV3.h"

#include <random>

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
	currentAnimation = p.currentAnimation;
	texture = p.texture;
	isEmpty = p.isEmpty;
	type = p.type;
	animTimer = rand() % MAX_ANIMATION_TIMER;
}

PuzzlePiece::~PuzzlePiece()
{
}

void PuzzlePiece::Update()
{
	if (!isEmpty &&!moving) {
		if (animTimer > 0)
			animTimer--;
		else {
			currentAnimation.Update();
		}
		if (currentAnimation.HasFinished()) {
			animTimer = rand() % MAX_ANIMATION_TIMER;
			currentAnimation.Reset();
		}
	}
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
