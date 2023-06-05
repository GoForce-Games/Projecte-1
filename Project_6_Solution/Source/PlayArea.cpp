#include "PlayArea.h"
#include "PuzzlePiece.h"
#include "Application.h"
#include "Globals.h"
#include "PlayerPiece.h"
#include "Collider.h"
#include "ModulePuzzlePiecesV3.h"
#include <stack>

PlayArea::PlayArea()
{
	for (uint i = 0; i < PLAY_AREA_W; i++)
	{
		for (uint j = 0; j < PLAY_AREA_H; j++)
		{
			table[i][j] = nullptr;
		}
	}

	explosionRange = 2;
	bigBombCharge = 0;
	bombsToSpawn = 0;

	state = PlayAreaState::INIT;
}

PlayArea::~PlayArea()
{
}

void PlayArea::Init(PuzzlePiece* fillWith)
{
	ModulePuzzlePiecesV3* manager = App->pieces;
	state = PlayAreaState::INIT;

	for (uint i = 0; i < PLAY_AREA_H; i++)
	{
		for (uint j = 0; j < PLAY_AREA_W; j++)
		{
			if (table[i][j] == nullptr) {
				table[i][j] = manager->AddPuzzlePiece(*fillWith/*, fillWith->collider->type */ );
				//table[i][j]->collider->name = "filler";
			}
		}
	}

	explosionRange = 2;
	bigBombCharge = 0;
	bombsToSpawn = 0;

	state = PlayAreaState::INIT;
}

bool PlayArea::Update()
{
	if (player != nullptr) player->Update();
	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_W; j++)
		{
			if (table[i][j] != nullptr) {
				table[i][j]->position.create(position.x + PIECE_SIZE * j, position.y + PIECE_SIZE * (i + 1));
				table[i][j]->Update();
			}
		}
	}

	return true;
}

void PlayArea::RecursePieces(std::deque<PuzzlePiece*>& deq, iPoint currPos, const PieceType& type, const CheckDirection& dir) {

	if (dir == CheckDirection::HORIZONTAL)
		currPos.x++;
	else
		currPos.y++;
	PuzzlePiece* piece = table[currPos.y][currPos.x];
	if (piece == nullptr || piece->type != type) return;
	deq.push_back(piece);
	RecursePieces(deq, currPos, type, dir);

	return;
}

bool PlayArea::checkGroupedPieces()
{

	PuzzlePiece* p = nullptr;

	// Empty deque before calculating
	while (piecesToRemove.size() > 0) piecesToRemove.pop_back();

	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_W; j++)
		{
			p = table[i][j];
			if (p->type == PieceType::BLACK || p->type == PieceType::WHITE || p->type == PieceType::RED || p->type == PieceType::BLUE || p->type == PieceType::GREEN) {
				std::deque<PuzzlePiece*> deq;
				iPoint pos;
				pos.create(j, i);
				LOG("%s %s\n", "Empieza la busqueda de piezas", (PuzzlePiece::enumLookup[p->type]));
				RecursePieces(deq, pos, p->type, CheckDirection::HORIZONTAL);
				deq.push_back(p);
				if (deq.size() >= GROUP_MIN_COUNT) {
					bombsToSpawn += (deq.size() - GROUP_MIN_COUNT + 1);
					for (PuzzlePiece* n; deq.size()>0;)
					{
						n = deq.back();
						piecesToRemove.push_back(n);
						n->SetAnimation(&App->pieces->animDeletion[n->type]);
						n->texture = App->pieces->textureDeletion;
						deq.pop_back();
					}
				}
				else {
					for (;deq.size()>0;deq.pop_back()) {} //Vacia el grupo
				}
				RecursePieces(deq, pos, p->type, CheckDirection::VERTICAL);
				deq.push_back(p);
				if (deq.size() >= GROUP_MIN_COUNT) {
					bombsToSpawn += (deq.size() - GROUP_MIN_COUNT + 1);
					for (PuzzlePiece* n; deq.size() > 0;)
					{
						n = deq.back();
						piecesToRemove.push_back(n);
						n->SetAnimation(&App->pieces->animDeletion[n->type]);
						n->texture = App->pieces->textureDeletion;
						deq.pop_back();
					}
				}
			}
			else if (p->type == PieceType::PRIMED_BOMB) {
				piecesToRemove.push_back(p);
				return true;
			}
		}
	}

	for (PuzzlePiece* p : piecesToRemove)
	{
		if (!p->pendingToDelete) {
			p->pendingToDelete = true;
		}
	}

	return piecesToRemove.size() > 0;
}



void PlayArea::explodeBombs()
{
}

bool PlayArea::CleanUp()
{
	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		for (size_t j = 0; j < PLAY_AREA_W; j++)
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

		for (size_t i = 0; i < PLAY_AREA_H; i++)
		{
			for (size_t j = 0; j < PLAY_AREA_W; j++)
			{
				switch (table[i][j]->type)
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
	bool finishedDropping = false;
	while (!finishedDropping) {
		finishedDropping = true;
		for (size_t i = 1; i < PLAY_AREA_H - 2; i++)
		{
			for (size_t j = 1; j < PLAY_AREA_W - 0; j++)
			{
				if (table[i][j] != nullptr)
					if (PieceCanDrop(table[i][j], table[i + 1][j])) {
						finishedDropping = false;
						PuzzlePiece* aux = table[i][j];
						table[i][j] = table[i + 1][j];
						table[i + 1][j] = aux;

						//Actualiza la posicion de las piezas
						//table[j][i]->position.y += PIECE_SIZE;
						//table[j][i + 1]->position.y -= PIECE_SIZE;


					}

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

bool PlayArea::PlaceBomb(int col)
{
	if (!table[1][col]->isEmpty) return false;

	for (size_t i = 1; i < PLAY_AREA_H - 2; i++)
	{
		PuzzlePiece* aux = table[i][col];
		table[i][col] = table[i + 1][col];
		table[i + 1][col] = aux;
	}
	App->pieces->RemovePuzzlePiece(table[PLAY_AREA_H - 2][col]);
	table[PLAY_AREA_H - 2][col] = App->pieces->AddPuzzlePiece(*App->pieces->templateBomb);

	return false;
}

PuzzlePiece* PlayArea::GetPiece(int x, int y)
{
	if (x < 0 || x >= PLAY_AREA_W) return nullptr;
	if (y < 0 || y >= PLAY_AREA_H) return nullptr;
	return table[y][x];
}

