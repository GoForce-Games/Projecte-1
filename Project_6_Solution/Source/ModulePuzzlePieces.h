#pragma once

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "PuzzlePiece.h"

struct SDL_Texture;
struct Collider;

#define MAX_PIECES 200
#define MAX_DROP_DELAY 75

class ModulePuzzlePieces :
    public Module
{
public:
    // Constructor
    ModulePuzzlePieces();

    // Destructor
    ~ModulePuzzlePieces();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	update_status Update() override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	update_status PostUpdate() override;

	// Collision callback, called when the player intersects with a puzzle piece
	void OnCollision(Collider* c1, Collider* c2) override;

	// Add new PuzzlePiece to the board
	PuzzlePiece* AddPuzzlePiece(const PuzzlePiece& newPiece);

	void RemovePuzzlePiece(PuzzlePiece* piece);

public:
	// Active puzzle piece
	PuzzlePiece* currentPiece = nullptr;

	uint dropDelay = MAX_DROP_DELAY;

	// Pieces (either bombermen or bombs) currently on screen
	PuzzlePiece* pieces[MAX_PIECES];

	// Pointer to active animation.
	// This will be switched randomly between the different animations based on random delays
	Animation* currentAnimation = nullptr;

	// Texture sheets
	SDL_Texture* textureBomberman = nullptr;
	SDL_Texture* textureBomb = nullptr;

	// Animation array. Dynamic array (bombs only have 1 animation, while bombermen have multiple)
	Animation animDefault;
	Animation animIdle1;

	Collider* collider = nullptr;

	// To be used when clearing a group of bombermen or bombs
	bool explode = false;

	// Time until this piece disappears from the screen
	uint explodeCountdown = 100;

	// No hace falta crear una variable extra por cada objeto para esto
	// SFX id number
	const static uint explosionFX = 0;

};

