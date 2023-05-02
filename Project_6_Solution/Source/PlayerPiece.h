#pragma once
#include "PuzzlePiece.h"

class Path;

class PlayerPiece
{
public:

	// Instancia de jugador sin piezas
	PlayerPiece();
	//Instancia de jugador con piezas
	PlayerPiece(PuzzlePiece* pieces[4]);

	//Destructor
	~PlayerPiece();

	bool Start();

	bool CleanUp();

	// Rota piezas
	bool Rotate(bool clockwise);

	// Actualiza el estado de las piezas
	bool Update();

	// Añade piezas nuevas
	void setPieces(PuzzlePiece* newPieces[4]);

public:
	PuzzlePiece* pieces[2][2] = 
		{{nullptr, nullptr},
		 {nullptr, nullptr}};

	//Punto posición absoluta del grupo, la posicion de las piezas es relativa a esta variable
	iPoint position;

	//Animaciones de posicion
	Path* piecePaths[2][2];

};

