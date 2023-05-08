#pragma once
#include "Module.h"
#include <stack>
#include "PuzzlePiece.h"
#include "Collider.h"


enum PlayerCollisionCheck {
	CENTER,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	DEBUG


};

class ModuleControlV3 :
    public Module
{
public:
	// Constructor
	ModuleControlV3(bool startEnabled = true);

	// Destructor
	~ModuleControlV3();

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

	std::stack<PuzzlePiece*>& ModuleControlV3::GeneratePuzzlePieces(std::stack<PuzzlePiece*>& stack, uint amount);

	// Add new PuzzlePiece to the board
	PuzzlePiece* AddPuzzlePiece(const PuzzlePiece& newPiece, Collider::Type type = Collider::Type::PUZZLE_PIECE);

	void RemovePuzzlePiece(PuzzlePiece* piece);

	//Comprueba la colisión en eje cardinal según la dirección proporcionada (si solo una de las coordenadas es 1/-1 comprueba todo el lado)
	bool WillCollide(PlayerCollisionCheck direction);

	//Saca las piezas del jugador y las coloca en el tablero donde les toca
	void PlacePieces();

	bool PieceCanDrop(PuzzlePiece* piece);


};

