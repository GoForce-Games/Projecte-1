#include "PuzzlePiece.h"
#include "Application.h"

#include "Animation.h"
#include "Collider.h"

PuzzlePiece::PuzzlePiece() : position()
{
	type = PieceType::NONE;
}

PuzzlePiece::PuzzlePiece(const PuzzlePiece& p)
{
	moving = p.moving;
	position.create(p.position.x,p.position.y);
	collider = nullptr;
	currentAnimation = new Animation(*p.currentAnimation);
	texture = p.texture;
	isEmpty = p.isEmpty;
	type = p.type;
}

PuzzlePiece::~PuzzlePiece()
{
	if (collider != nullptr) {
		collider->pendingToDelete = true;
	}
}

void PuzzlePiece::Update()
{
	currentAnimation->Update();
}

// Borra la copia de la animación actual y asigna una nueva
// Esto permite que las piezas se animen por separado, aunque seguro que hay mejores formas de hacerlo
void PuzzlePiece::SetAnimation(Animation* newAnimation) {
	currentAnimation = newAnimation;
}
