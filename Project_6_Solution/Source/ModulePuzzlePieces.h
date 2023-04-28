#pragma once

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "PuzzlePiece.h"
#include "Collider.h"

struct SDL_Texture;

#define MAX_PIECES 200

#define MAX_WALLS 6

#define MAX_DROP_DELAY 15
#define MIN_DROP_DELAY 2
#define MAX_MOVE_DELAY 5

class ModulePuzzlePieces :
    public Module
{
public:
    // Constructor
    ModulePuzzlePieces(bool startEnabled = true);

    // Destructor
    ~ModulePuzzlePieces();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	Update_Status Update() override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	Update_Status PostUpdate() override;

	// Collision callback, called when the player intersects with a puzzle piece
	void OnCollision(Collider* c1, Collider* c2) override;

	bool CleanUp() override;

	// Add new PuzzlePiece to the board
	PuzzlePiece* AddPuzzlePiece(const PuzzlePiece& newPiece);

	void RemovePuzzlePiece(PuzzlePiece* piece);

	bool WillCollide(iPoint position);

public:
	//Array bidimensional de punteros a pieza (PuzzlePiece** * table;)
	PuzzlePiece** * table;

	// Active puzzle piece
	PuzzlePiece* currentPiece = nullptr;

	bool fastFall = false;
	bool locked = false;

	uint dropDelay = MAX_DROP_DELAY;
	uint moveDelay = MAX_MOVE_DELAY;
	
	// Movement speed in pixels per frame
	uint moveSpeed = 16;

	// Pieces (either bombermen or bombs) currently on screen
	PuzzlePiece* pieces[MAX_PIECES];

	// Pointer to active animation.
	// This will be switched randomly between the different animations based on random delays
	Animation* currentAnimation = nullptr;

	// Texture sheets
	SDL_Texture* textureBomberman = nullptr;
	SDL_Texture* textureBomb = nullptr;

	// Animaciones
	Animation animDefault;
	Animation animIdle1;

	Collider* collider = nullptr;

	// Collider de uso manual, no gestionado por ModuleCollsions
	Collider* collisionTester = nullptr;

	// Para colisiones manuales al mover las piezas
	uint wallCount = 0;
	Collider* walls[MAX_WALLS];

	Collider::Type collidingWith = Collider::Type::NONE;

	// To be used when clearing a group of bombermen or bombs
	bool explode = false;

	// Time until this piece disappears from the screen
	uint explodeCountdown = 100;

	// No hace falta crear una variable extra por cada objeto para esto
	// SFX id number
	const static uint explosionFX = 0;

};

