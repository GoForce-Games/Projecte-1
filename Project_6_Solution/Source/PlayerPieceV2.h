#pragma once
#include "PuzzlePiece.h"

#define MAX_ANIMATION_TIMER 1200

class Path;
class PlayerInput;

class PlayerPieceV2
{
public:

	// Instancia de jugador sin piezas
	PlayerPieceV2();
	//Instancia de jugador con piezas
	PlayerPieceV2(PuzzlePiece* pieces[4]);

	//Destructor
	~PlayerPieceV2();

	bool Start();

	bool CleanUp();

	// Rota piezas
	bool Rotate();

	// Actualiza el estado de las piezas (posición y animación
	bool Update();

	void refreshColliderState();

	// Añade piezas nuevas
	void setPieces(PuzzlePiece* newPieces[4]);

public:
	PlayerInput* gamepad = nullptr;

	PuzzlePiece* pieces[2][2] =
	{ {nullptr, nullptr},
	 {nullptr, nullptr} };

	//Punto posición absoluta del grupo, la posicion de las piezas es relativa a esta variable
	iPoint position;

	//Animaciones de posicion
	Path* piecePaths[2][2];

	bool rotating = false;
	bool locked = false;

};

