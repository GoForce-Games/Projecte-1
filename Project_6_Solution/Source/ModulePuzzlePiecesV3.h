#pragma once
#include "Module.h"
#include <stack>
#include <queue>
#include "PuzzlePiece.h"
#include "Collider.h"
#include "PlayerPiece.h"
#include "PlayArea.h"

#include "GameConstants.h"

class ModulePuzzlePiecesV3 :
    public Module
{
public:
	// Constructor
	ModulePuzzlePiecesV3(bool startEnabled = true);

	// Destructor
	~ModulePuzzlePiecesV3();

	bool Init() override;

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

	std::stack<PuzzlePiece*>& ModulePuzzlePiecesV3::GeneratePuzzlePieces(std::stack<PuzzlePiece*>& stack, uint amount);

	// Add new PuzzlePiece to the board
	PuzzlePiece* AddPuzzlePiece(const PuzzlePiece& newPiece, Collider::Type type = Collider::Type::PUZZLE_PIECE);

	void RemovePuzzlePiece(PuzzlePiece* piece);

	//Comprueba la colisión en eje cardinal según la dirección proporcionada (si solo una de las coordenadas es 1/-1 comprueba todo el lado)
	bool WillCollide(PlayerCollisionCheck direction);

	//Saca las piezas del jugador y las coloca en el tablero donde les toca
	void PlacePieces();

	bool PieceCanDrop(PuzzlePiece* piece);

private: // Helper functions
	
	// Initializers

	void EnableDependencies();

	void LoadTextures();

	void InitAnims();

	void InitTemplates();

	void InitWalls();

	void InitPlayers();

	void InitMisc();

	// Gameplay methods for Update()

	void ProcessInput();

	void ApplyPhysics();

	void ApplyLogic();

	// Coordinate-related helper methods

	iPoint ScreenToLocal(PlayArea& localArea, iPoint sCoordinates);

	iPoint AreaToScreen(PlayArea& localArea, iPoint lCoordinates);

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

	uint dropDelay;
	uint moveDelay;

	// Movement speed in pixels per frame
	uint moveSpeed;

	// Drop speed in pixels per frame
	uint gravity;

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
	uint wallCount;
	Collider* walls[MAX_WALLS];

	// SFX id number
	const uint explosionFX = 0;

};

