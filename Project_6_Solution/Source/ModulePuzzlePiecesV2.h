#pragma once
#include "Module.h"

#include <queue>

#include "Animation.h"
#include "p2Point.h"
#include "PuzzlePiece.h"
#include "Collider.h"
#include "PlayerPiece.h"
#include "PlayArea.h"

struct SDL_Texture;

#define MAX_PIECES 200

#define MAX_WALLS 6

#define MAX_DROP_DELAY 15
#define MIN_DROP_DELAY 2
#define MAX_MOVE_DELAY 5

#define MOVE_SPEED PIECE_SIZE
#define GRAVITY 4

#define EXPLODE_COUNTDOWN 100

enum PlayerCollisionCheck {
	CENTER,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	DEBUG

	
};

class ModulePuzzlePiecesV2 :
	public Module
{
public:
	// Constructor
	ModulePuzzlePiecesV2(bool startEnabled = true);

	// Destructor
	~ModulePuzzlePiecesV2();

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

	void GeneratePuzzlePieces(uint amount);

	// Add new PuzzlePiece to the board
	PuzzlePiece* AddPuzzlePiece(const PuzzlePiece& newPiece, Collider::Type type = Collider::Type::PUZZLE_PIECE);

	void RemovePuzzlePiece(PuzzlePiece* piece);

	//Comprueba la colisión en eje cardinal según la dirección proporcionada (si solo una de las coordenadas es 1/-1 comprueba todo el lado)
	bool WillCollide(PlayerCollisionCheck direction);

	//Saca las piezas del jugador y las coloca en el tablero donde les toca
	void PlacePieces();

	bool PieceCanDrop(PuzzlePiece* piece);

public:
	
	// Avoid enabling twice
	bool isInitialized = false;

	PuzzlePiece templateMan;

	//Pieza vacía, sin nada asignado
	PuzzlePiece* emptyPiece = nullptr;

	PlayerPiece player;

	PlayArea playArea;

	bool fastFall = false;
	bool locked = false;

	uint dropDelay = MAX_DROP_DELAY;
	uint moveDelay = MAX_MOVE_DELAY;

	// Movement speed in pixels per frame
	uint moveSpeed = MOVE_SPEED;

	// Drop speed in pixels per frame
	uint gravity = GRAVITY;

	// Pieces (either bombermen or bombs) currently on screen
	PuzzlePiece* pieces[MAX_PIECES];

	// Piezas pregeneradas para añadir a la zona de juego
	std::queue<PuzzlePiece*> pieceQueue;

	// Pointer to active animation.
	// This will be switched randomly between the different animations based on random delays
	Animation* currentAnimation = nullptr;

	// Texture sheets
	SDL_Texture* textureBomberman = nullptr;
	SDL_Texture* textureBomb = nullptr;

	// Animaciones
	Animation animNone;
	Animation animDefault;
	Animation animIdle1;

	Collider* collider = nullptr;

	// Collider de uso manual, no gestionado por ModuleCollsions
	Collider* collisionTester = nullptr;

	// Para colisiones manuales al mover las piezas
	uint wallCount = 0;
	Collider* walls[MAX_WALLS];

	// To be used when clearing a group of bombermen or bombs
	bool explode = false;

	// Time until this piece disappears from the screen
	uint explodeCountdown = EXPLODE_COUNTDOWN;

	// No hace falta crear una variable extra por cada objeto para esto
	// SFX id number
	const static uint explosionFX = 0;

};