#pragma once

#include "p2Point.h"
#include "Globals.h"

#include "Animation.h"
#include <string>

#define PIECE_SIZE 16

struct Collider;
struct SDL_Texture;

enum PieceType {
	NONE,
	BLACK,
	WHITE,
	RED,
	BLUE,
	GREEN,
	BOMB,
	PRIMED_BOMB,
	WALL,

	MAX_PIECE_TYPE
};

enum ExplosionDir {
	TOP_TIP,
	TOP_MIDDLE,
	BOTTOM_TIP,
	BOTTOM_MIDDLE,
	LEFT_TIP,
	LEFT_MIDDLE,
	RIGHT_TIP,
	RIGHT_MIDDLE,
	BOMB_CENTER,

	MAX_BOMB_ANIM
};

class PuzzlePiece {
public:

	static const char* enumLookup[];


	// Constructor
	PuzzlePiece();

	// Copy constructor
	PuzzlePiece(const PuzzlePiece& p);

	// Destructor
	~PuzzlePiece();

	// Active piece update (player movement)
	void Update();

	void SetType(PieceType color);

	void SetAnimation(Animation* newAnimation);

public:
	// For debug purposes
	std::string name = "";

	PieceType type;

	bool isEmpty = false;

	// Variable para indicar si la pieza está siendo movida por el jugador o no
	bool moving = false;

	bool pendingToDelete = false;

	// Position of the puzzle piece on the screen
	iPoint position;

	// The piece's current animation
	Animation currentAnimation;

	SDL_Texture* texture = nullptr;

	int animTimer = 0;

	//For use with bombs
	bool exploding = false;
	
};
