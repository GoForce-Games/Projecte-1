#ifndef MODULE_PUZZLE_PIECES_H

#define MODULE_PUZZLE_PIECES_H


#include "Module.h"
#include <stack>
#include <queue>
#include "PuzzlePiece.h"
#include "Collider.h"
#include "PlayerPieceV2.h"
#include "PlayArea.h"

#include "Particle.h"

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

	std::stack<PuzzlePiece*>& ModulePuzzlePiecesV3::GeneratePuzzlePieces(std::stack<PuzzlePiece*>& stack, uint amount, bool noBombs = false);

	// Add new PuzzlePiece to the board
	PuzzlePiece* AddPuzzlePiece(const PuzzlePiece& newPiece, Collider::Type type = Collider::Type::PUZZLE_PIECE);

	void RemovePuzzlePiece(PuzzlePiece* piece);

	//Comprueba la colisi�n en eje cardinal seg�n la direcci�n proporcionada (si solo una de las coordenadas es 1/-1 comprueba todo el lado)
	//bool WillCollide(PlayerCollisionCheck direction);

	bool CheckOutOfBounds(PlayArea* area, PlayerPieceV2* player);

	// Check if player can move left
	bool CanGoLeft(PlayArea* area, PlayerPieceV2* player);

	// Check if player can move right
	bool CanGoRight(PlayArea* area, PlayerPieceV2* player);

	// Check if gravity can pull down on player
	bool CanGoDown(PlayArea* area, PlayerPieceV2* player);

	bool CanRotate(PlayArea* area, PlayerPieceV2* player);

	//Saca las piezas del jugador y las coloca en el tablero donde les toca
	void PlacePieces();

	//Unused
	bool PieceCanDrop(PuzzlePiece* piece);

private: // Helper functions
	
	// Initializers

	void EnableDependencies();

	void LoadTextures();

	void InitAnims();

	void InitTemplates();

	// MUST RUN AFTER InitTemplates()
	void InitPieceArray();

	void InitWalls();

	void InitPlayers();

	void InitMisc();

	// Gameplay methods for Update()

	void ProcessInput();

	void ApplyPhysics();

	void ApplyLogic();

	void RemoveGroups();

	void AddBombs(PlayArea& area);

	void AssignNewPieces(PlayerPieceV2* player, bool noBombs = false);

	void DetonateBombs(PlayArea& area);

public:

	// Coordinate-related helper methods

	iPoint WorldToLocal(PlayArea& localArea, iPoint sCoordinates);

	iPoint AreaToWorld(PlayArea& localArea, iPoint lCoordinates);

public:

	// Avoid enabling twice
	bool isInitialized = false;

	//Godmode-related variables
	bool forcePrimedBomb = false;
	bool forceBombToggle = false;

	PuzzlePiece* templateMan = nullptr;
	PuzzlePiece* templateBomb = nullptr;

	//Pieza vac�a, sin nada asignado
	PuzzlePiece* emptyPiece = nullptr;

	PlayerPieceV2 player;

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

	// Piezas pregeneradas para a�adir a la zona de juego
	std::stack<PuzzlePiece*> pieceQueue;

	// Pointer to active animation.
	// This will be switched randomly between the different animations based on random delays
	Animation* currentAnimation = nullptr;

	// Texture sheets
	SDL_Texture* textureBomberman = nullptr;
	SDL_Texture* textureExplosion = nullptr;
	SDL_Texture* textureDeletion = nullptr;

	// Animaciones
	Animation animNone;
	Animation animDefaultTest;

	//Animaci�n pasiva
	Animation animIdle[PieceType::MAX_PIECE_TYPE];

	//Animaci�n usada cuando se eliminan las piezas
	Animation animDeletion[PieceType::MAX_PIECE_TYPE];

	Animation animExplosionEffect[ExplosionDir::MAX_BOMB_ANIM];

	Particle explosionTemplates[ExplosionDir::MAX_BOMB_ANIM];

	Animation EXPLTestAnim;

	Particle testParticle;


	Collider* collider = nullptr;

	// Collider de uso manual, no gestionado por ModuleCollsions
	Collider* collisionTester = nullptr;

	// Para colisiones manuales al mover las piezas
	uint wallCount;
	Collider* walls[MAX_WALLS];

	// SFX id number
	const uint explosionFX = 0;

	uint rotateFX;
	uint lockedFX;
	uint eliminaePiecesFX;

	bool cleanedUp = false;

};

#endif // !MODULE_PUZZLE_PIECES_H
