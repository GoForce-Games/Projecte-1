#include "PlayArea.h"
#include "PuzzlePiece.h"
#include "Application.h"
#include "Globals.h"
#include "PlayerPiece.h"
#include "Collider.h"
#include "ModulePuzzlePiecesV2.h"
#include <stack>

PlayArea::PlayArea()
{
	for (uint i = 0; i < PLAY_AREA_X; i++)
	{
		for (uint j = 0; j < PLAY_AREA_Y; j++)
		{
			table[i][j] = nullptr;
		}
	}
}

PlayArea::~PlayArea()
{
}

void PlayArea::Init(PuzzlePiece* fillWith)
{
	ModulePuzzlePiecesV2* manager = App->pieces;

	for (uint i = 0; i < PLAY_AREA_X; i++)
	{
		for (uint j = 0; j < PLAY_AREA_Y; j++)
		{
			table[i][j] = manager->AddPuzzlePiece(*fillWith,fillWith->collider->type);
		}
	}
}

bool PlayArea::Update()
{
	for (size_t i = 0; i < PLAY_AREA_X; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_Y; j++)
		{
			if (table[i][j] != nullptr) {
				table[i][j]->position.create(position.x + PIECE_SIZE * i, position.y + PIECE_SIZE * (j+1));
				table[i][j]->Update();
			}
		}
	}

	return true;
}

/*
void PlayArea::NewPieceSet(PlayerPiece* player)
{
	PuzzlePiece newPiece;
	newPiece.moving = true;
	PuzzlePiece** pieceArray = new PuzzlePiece*[3];
	for (uint i = 0; i < 3; i++)
	{
		pieceArray[i] = new PuzzlePiece(newPiece);
	}
	player->setPieces(pieceArray);
}
*/

void PlayArea::RecurseGroups(std::deque<iPoint>& group, iPoint currPos, PieceType type, uint& count) {
	group.push_back(table[currPos.x][currPos.y]->position);
	bool existsInGroup = false;
	for (size_t i = -1; i < 2; i += 2)
	{
		//Si no es del mismo tipo salta esta iteracion
		if (table[currPos.x + i][currPos.y]->type != type) continue;

		//Busca si ya se encuentra en el grupo
		for (iPoint p : group)
		{
			if (p == table[currPos.x + i][currPos.y]->position) {
				existsInGroup = true;
				break;
			}
		}

		//Si ya ha sido añadido salta a la siguiente iteracion
		if (existsInGroup) {
			existsInGroup = false;
			continue;
		}
		else { //Si aun no ha sido añadido, lo añade y sigue buscando recursivamente
			currPos.x += i;
			RecurseGroups(group, currPos, type, ++count);
		}
	}
	
	for (size_t i = -1; i < 2; i += 2)
	{
		//Si no es del mismo tipo salta esta iteracion
		if (table[currPos.x][currPos.y + i]->type != type) continue;

		//Busca si ya se encuentra en el grupo
		for (iPoint p : group)
		{
			if (p == table[currPos.x][currPos.y + i]->position) {
				existsInGroup = true;
				break;
			}
		}

		//Si ya ha sido añadido salta a la siguiente iteracion
		if (existsInGroup) {
			existsInGroup = false;
			continue;
		}
		else { //Si aun no ha sido añadido, lo añade y sigue buscando recursivamente
			currPos.y += i;
			RecurseGroups(group, currPos, type, ++count);
		}
	}
}

void PlayArea::checkGroupedPieces()
{
	// Posiciones de las piezas a quitar
	std::stack<std::stack<iPoint>> groupsToRemove;

	PuzzlePiece* p = nullptr;

	for (size_t i = 0; i < PLAY_AREA_X; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_Y; j++)
		{
			p = table[i][j];
		}
	}

}



void PlayArea::explodeBombs()
{
}

bool PlayArea::CleanUp()
{
	for (size_t i = 0; i < PLAY_AREA_X; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_Y; j++)
		{
			if (table[i][j] != nullptr) {
				table[i][j] = nullptr;
			}
		}
	}

	return true;
}

void PlayArea::debugPiecePosition() {
	LOG("ALERTA, EL USO DE LA FUNCION PlayArea::debugPiecePosition() IMPLICA UN ALTO IMPACTO EN RENDIMIENTO DEBIDO A SU IMPLEMENTACION")

	for (size_t i = 0; i < PLAY_AREA_Y; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_X; j++)
		{
			switch (table[j][i]->type)
			{
			case NONE: {
				OutputDebugString("-");

				break;
			}
			case WALL: {
				OutputDebugString("P");

				break;
			}
			case WHITE: {
				OutputDebugString("B");

				break;
			}
			case BLACK: {
				OutputDebugString("N");

				break;
			}
			case BLUE: {
				OutputDebugString("A");

				break;
			}
			case GREEN: {
				OutputDebugString("V");

				break;
			}
			case BOMB: {
				OutputDebugString("K");

				break;
			}
			default:
				break;
			}


		}
		OutputDebugString("\n");
	}


}

void PlayArea::DropPieces()
{
	//OutputDebugString("Haciendo caer a las piezas...\n");
	//debugPiecePosition();
	for (size_t i = 0; i < PLAY_AREA_Y - 2; i++)
	{
		for (size_t j = 1; j < PLAY_AREA_X - 1; j++)
		{
			if (table[i][j] != nullptr)
				if (PieceCanDrop(table[j][i], table[j][i + 1])) {
					PuzzlePiece* aux = table[j][i];
					table[j][i] = table[j][i + 1];
					table[j][i + 1] = aux;

					//Actualiza la posicion de las piezas
					//table[j][i]->position.y += PIECE_SIZE;
					//table[j][i + 1]->position.y -= PIECE_SIZE;


				}

		}
	}
	//OutputDebugString("Resultado:\n");
	//debugPiecePosition();
	return;
}


bool PlayArea::PieceCanDrop(PuzzlePiece* pieceTop, PuzzlePiece* pieceBot)
{
	if (pieceTop == nullptr || pieceTop->isEmpty) return false; // Estas intentando mover "nada"
	if (pieceBot == nullptr || pieceBot->isEmpty) return true; // No hay nada debajo

	//if (pieceBot->type == PieceType::WALL) return false; // No puede intercambiar con una pared
	if (pieceBot->type == PieceType::NONE) return true; // Espacio sin colision debajo

	return false;
}