#include "PuzzlePiece.h"
#include "Application.h"

#include "Animation.h"
#include "Collider.h"

PuzzlePiece::PuzzlePiece() : position()
{
}

PuzzlePiece::PuzzlePiece(const PuzzlePiece& p) :
	moving(p.moving),
	position(p.position),
	collider(p.collider),
	currentAnimation(p.currentAnimation),
	texture(p.texture)
{}

PuzzlePiece::~PuzzlePiece()
{
	if (collider != nullptr) {
		collider->pendingToDelete = true;
	}
}

void PuzzlePiece::Update()
{
	currentAnimation.Update();
}

// Borra la copia de la animación actual y asigna una nueva
// Esto permite que las piezas se animen por separado, aunque seguro que hay mejores formas de hacerlo
void PuzzlePiece::SetAnimation(Animation newAnimation) {
	currentAnimation = newAnimation;
}
