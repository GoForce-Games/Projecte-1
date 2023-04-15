#include "p2Point.h"
#include "Globals.h"

#include "Animation.h"

struct Collider;
class Animation;
struct SDL_Texture;

struct PuzzlePiece {
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