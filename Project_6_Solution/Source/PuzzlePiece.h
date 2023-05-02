#pragma once

#include "p2Point.h"
#include "Globals.h"

#include "Animation.h"

#define PIECE_SIZE 16

struct Collider;
struct SDL_Texture;

class PuzzlePiece {
public:
	// Constructor
	PuzzlePiece();

	// Copy constructor
	PuzzlePiece(const PuzzlePiece& p);

	// Destructor
	~PuzzlePiece();

	// Active piece update (player movement)
	void Update();

	void SetAnimation( Animation newAnimation);

public:
	bool isEmpty = false;

	// Variable para indicar si la pieza está siendo movida por el jugador o no
	bool moving = false;

	// Position of the puzzle piece on the screen
	iPoint position;

	//This piece's collider;
	Collider* collider = nullptr;

	// The piece's current animation
	Animation currentAnimation;

	SDL_Texture* texture = nullptr;

};