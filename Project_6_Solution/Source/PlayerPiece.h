#pragma once
#include "PuzzlePiece.h"

class Path;

class PlayerPiece
{
public:

	// Instancia de jugador sin piezas
	PlayerPiece();
	//Instancia de jugador con piezas
	PlayerPiece(PuzzlePiece* pieces[3]);

	//Destructor
	~PlayerPiece();

	bool Start();

	bool CleanUp();

	// Rota piezas
	bool Rotate(bool clockwise);

	// Actualiza el estado de las piezas
	bool Update();

	// Añade piezas nuevas
	void setPieces(PuzzlePiece* newPieces[3]);

public:
	PuzzlePiece* pieces[2][2] = 
		{{nullptr, nullptr},
		 {nullptr, nullptr}};

	//Punto pivote para el grupo de 3 piezas... pensandolo bien, esto no sirve de mucho
	iPoint position;

	//Animaciones de posicion
	Path* piecePaths[2][2];




};

