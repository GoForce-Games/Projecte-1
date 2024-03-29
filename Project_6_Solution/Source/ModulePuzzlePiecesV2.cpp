#include "ModulePuzzlePiecesV2.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

#include "../External_Libraries/SDL/include/SDL.h"

#include <random>
#include <time.h>
#include <iostream>

ModulePuzzlePiecesV2::ModulePuzzlePiecesV2(bool startEnabled) : Module(startEnabled)
{

}

ModulePuzzlePiecesV2::~ModulePuzzlePiecesV2()
{
	CleanUp();
}

bool ModulePuzzlePiecesV2::Start()
{
	srand(time(NULL));
	locked = false;

	for (uint i = 0; i < MAX_PIECES; i++)
	{
		pieces[i] = nullptr;
	}

	for (uint i = 0; i < MAX_WALLS; i++)
	{
		walls[i] = nullptr;
	}

	textureBomberman = App->textures->Load("Assets/testerman.png");
	if (textureBomberman == nullptr) return false;

	App->collisions->Enable();

	// Animacion temporal, sacado de la demo de R-type que hicimos en clase
	animDefault.PushBack({ 0, 0, 16, 16 });
	animDefault.PushBack({ 16, 0, 16, 16 });
	animDefault.PushBack({ 32, 0, 16, 16 });
	animDefault.PushBack({ 48, 0, 16, 16 });
	animDefault.speed = 0.08f;

	animNone.PushBack({ 0,0,0,0 });
	animNone.loop = false;
	animNone.speed = 1.0f;


	templateMan.collider = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::NONE);
	templateMan.position.create(-64, -64);
	templateMan.texture = textureBomberman;
	templateMan.SetAnimation(&animDefault);
	templateMan.moving = false;
	templateMan.type = PieceType::WHITE;


	emptyPiece = AddPuzzlePiece(templateMan, Collider::Type::NONE);
	emptyPiece->currentAnimation = &animNone;
	emptyPiece->isEmpty = true;
	emptyPiece->type = PieceType::NONE;

	playArea.Init(emptyPiece);

	//Paredes
	PuzzlePiece* templateWall = new PuzzlePiece(*emptyPiece);
	templateWall->moving = false;
	templateWall->type = PieceType::WALL;

	// Columna izquierda
	iPoint offset = playArea.position;
	//offset.y += PIECE_SIZE;
	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[0][i] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.y += PIECE_SIZE;
	}

	// Columna derecha
	offset = playArea.position;
	//offset.y += PIECE_SIZE;
	offset.x += PIECE_SIZE * (PLAY_AREA_W - 1);
	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[PLAY_AREA_W - 1][i] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.y += PIECE_SIZE;

	}


	// Fondo
	offset = playArea.position;
	offset.y += PIECE_SIZE * (PLAY_AREA_H);
	for (size_t i = 0; i < PLAY_AREA_W; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[i][PLAY_AREA_H - 1] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.x += PIECE_SIZE;
	}

	//Techo (por ciertas razones relacionadas con la deteccion de grupos)
	// Fondo
	offset = playArea.position;
	offset.y += 0;
	for (size_t i = 0; i < PLAY_AREA_W; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[i][0] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.x += PIECE_SIZE;
	}

	delete templateWall;



	PuzzlePiece* newPieces[4];
	newPieces[0] = AddPuzzlePiece(templateMan);
	newPieces[1] = AddPuzzlePiece(templateMan);
	newPieces[2] = AddPuzzlePiece(templateMan);
	newPieces[3] = AddPuzzlePiece(*emptyPiece);
	player.setPieces(newPieces);

	collisionTester = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::PLAYER);

	playArea.collisionTester = collisionTester;

	player.position.create(64, 16);


	rotateFX = App->audio->LoadFx("Assets/SFX/rotate.wav");
	lockedFX = App->audio->LoadFx("Assets/SFX/piecelocked.wav");

	return true;
}

Update_Status ModulePuzzlePiecesV2::Update()
{
	GamePad& pad = App->input->pads[0];
	//Update Animations
	playArea.Update();
	player.Update();

	if (!locked) {

		//Lee input

		Key_State* keys = App->input->keys;


		// Godmode: activa/desactiva gravedad
		if (keys[SDL_Scancode::SDL_SCANCODE_F9] == Key_State::KEY_DOWN || pad.a < 0.0f) {
			gravity = (gravity == 0) ? GRAVITY : 0;
		}



		// Rotacion

		if ((keys[SDL_Scancode::SDL_SCANCODE_P] == Key_State::KEY_DOWN) || pad.a < 0.0f){
			player.Rotate();
			App->audio->PlayFx(rotateFX);
		}





		// Acelera la ca�da
		if (keys[SDL_Scancode::SDL_SCANCODE_S] == Key_State::KEY_DOWN || pad.a < 0.0f) {
			dropDelay = MIN_DROP_DELAY;
		}
		fastFall = keys[SDL_Scancode::SDL_SCANCODE_S] == Key_State::KEY_REPEAT || pad.b < 0.0f;


		//El primer frame en el que intentas moverte a un lado es instantaneo
		if (keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_DOWN || pad.a < 0.0f) moveDelay = 0;
		else if (keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_DOWN || pad.a < 0.0f)	moveDelay = 0;


		// Mueve a la izquierda
		if (keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_REPEAT || pad.b < 0.0f && keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_IDLE || pad.x < 0.0f) {

			if (!WillCollide(PlayerCollisionCheck::LEFT)) {

				if (moveDelay == 0) {
					moveDelay = MAX_MOVE_DELAY;
					player.position.x -= moveSpeed;
				}
				else {
					moveDelay--;
				}
			}
		}

		// Mueve a la derecha
		if (keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_REPEAT && keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_IDLE) {

			if (!WillCollide(PlayerCollisionCheck::RIGHT)) {

				if (moveDelay == 0) {
					moveDelay = MAX_MOVE_DELAY;
					player.position.x += moveSpeed;
				}
				else {
					moveDelay--;
				}
			}
		}


		//Aplica gravedad

		if (dropDelay == 0) {
			if (!WillCollide(PlayerCollisionCheck::BOTTOM)) {
				dropDelay = fastFall ? MIN_DROP_DELAY : MAX_DROP_DELAY;
				player.position.y += gravity;
			}
			else {
				locked = true;
				App->audio->PlayFx(lockedFX);
				PlacePieces();
			}
			WillCollide(PlayerCollisionCheck::DEBUG); // Curiosamente quitar esto rompe la colision con el borde de abajo
		}
		else {
			dropDelay--;
		}






	}
	else { // Logica a aplicar entre piezas nuevas
		playArea.DropPieces();
		playArea.checkGroupedPieces();
		/*
		std::stack<PuzzlePiece*> s;
		GeneratePuzzlePieces(s, 3);
		PuzzlePiece* newPieces[4];
		newPieces[0] = s.top();
		s.pop();
		newPieces[1] = s.top();
		s.pop();
		newPieces[2] = s.top();
		s.pop();
		newPieces[3] = AddPuzzlePiece(*emptyPiece);
		player.setPieces(newPieces);
		*/
		player.position.create(64, 16);
	}


	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePuzzlePiecesV2::PostUpdate()
{
	for (size_t i = 0; i < MAX_PIECES; i++)
	{
		PuzzlePiece* p = pieces[i];
		if (p == nullptr) continue;
		SDL_Rect& currFrame = p->currentAnimation->GetCurrentFrame();
		iPoint& pos = p->position;
		SDL_Texture* texture = p->texture;
		App->render->Blit(texture, pos.x, pos.y, &currFrame);
	}

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePuzzlePiecesV2::OnCollision(Collider* c1, Collider* c2)
{
}

bool ModulePuzzlePiecesV2::CleanUp()
{
	RemovePuzzlePiece(&templateMan);
	playArea.CleanUp();
	App->collisions->CleanUp();

	for (size_t i = 0; i < MAX_PIECES; i++)
	{
		RemovePuzzlePiece(pieces[i]);
	}

	App->textures->Unload(textureBomberman);
	textureBomberman = nullptr;
	App->textures->Unload(textureBomb);
	textureBomb = nullptr;
	return true;
}

std::stack<PuzzlePiece*>& ModulePuzzlePiecesV2::GeneratePuzzlePieces(std::stack<PuzzlePiece*>& stack, uint amount)
{

	for (uint i = 0; i < amount; i++)
	{
		PuzzlePiece* newPiece = AddPuzzlePiece(templateMan);
		newPiece->type = (PieceType)(2 + (rand() % 3));
		stack.push(newPiece);
	}


	return stack;
}

PuzzlePiece* ModulePuzzlePiecesV2::AddPuzzlePiece(const PuzzlePiece& piece, Collider::Type type)
{
	for (uint i = 0; i < MAX_PIECES; i++) {
		if (pieces[i] == nullptr) {
			//Crea nueva pieza con una caja de colision copiada de la plantilla
			PuzzlePiece* newPiece = new PuzzlePiece(piece);
			newPiece->collider = App->collisions->AddCollider(templateMan.collider->rect, type);
			if (newPiece->collider != nullptr) 
				newPiece->collider->SetPos(newPiece->position.x, newPiece->position.y);
			return pieces[i] = newPiece;
		}
	}
	return nullptr;
}

void ModulePuzzlePiecesV2::RemovePuzzlePiece(PuzzlePiece* piece)
{
	if (piece == nullptr) return;

	piece->name = "To be deleted"; // Para debug

	for (size_t i = 0; i < MAX_PIECES; i++)
	{
		if (pieces[i] == piece) {
			piece->collider->pendingToDelete = true;
			delete piece;
			pieces[i] = nullptr;
			break;
		}

	}

}

bool ModulePuzzlePiecesV2::WillCollide(PlayerCollisionCheck direction)
{
	SDL_Rect& rect = collisionTester->rect;
	rect.w = PIECE_SIZE;
	rect.h = PIECE_SIZE;
	int x = 0, y = 0;

	switch (direction)
	{
	case CENTER: {
		break;
	}
	case LEFT: {
		x = -1;

		//Si el colisionador de la pieza correspondiente esta desactivado no hace falta comprobar m�s all� de donde se encuentra
		if (player.pieces[0][0]->collider->enabled) y--;
		if (player.pieces[1][0]->collider->enabled) y++;
		break;
	}
	case RIGHT: {
		x = 1;
		if (player.pieces[0][1]->collider->enabled) y--;
		if (player.pieces[1][1]->collider->enabled) y++;
		break;
	}
	case TOP: {
		y = -1;
		if (player.pieces[0][0]->collider->enabled) x--;
		if (player.pieces[0][1]->collider->enabled) x++;
		break;
	}
	case BOTTOM: {
		y = 1;
		if (player.pieces[1][0]->collider->enabled) x--;
		if (player.pieces[1][1]->collider->enabled) x++;
		break;
	}
	default:
		break;
	}

	if (x == 0) {
		//Hace que el colisionador ocupe completamente el ancho del jugador
		rect.x = player.position.x;
		rect.w = PIECE_SIZE * 2;
	}
	else {
		// Pone el colisionador a la izquierda o la derecha
		if (x > 0)
			rect.x = player.position.x + (PIECE_SIZE * 2);
		else
			rect.x = player.position.x - PIECE_SIZE;
	}

	if (y == 0) {
		//Hace que el colisionador ocupe completamente el alto del jugador
		rect.y = player.position.y + gravity;
		rect.h = PIECE_SIZE * 2;
	}
	else {
		// Pone el colisionador debajo (+1) o arriba (-1) del jugador
		if (y > 0)
			rect.y = player.position.y + (PIECE_SIZE)+gravity;
		else
			rect.y = player.position.y - PIECE_SIZE + gravity;
	}

	if (direction != PlayerCollisionCheck::DEBUG) {

		for (size_t i = 0; i < MAX_PIECES; i++)
		{
			if (pieces[i] != nullptr && collisionTester->Intersects(pieces[i]->collider->rect)) {
				return true;
			}
		}
		if (direction != CENTER && (x != 0 || y != 0)) {
			return WillCollide(PlayerCollisionCheck::CENTER);
		}

	}

	return false;
}

void ModulePuzzlePiecesV2::PlacePieces() {
	iPoint posPlayer = player.position;
	iPoint posZona = playArea.position;
	iPoint posTablero = (player.position - playArea.position) / PIECE_SIZE;
	posTablero.y--;

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (!player.pieces[i][j]->isEmpty)
				playArea.table[posTablero.x + i][posTablero.y + j] = player.pieces[i][j];
			player.pieces[i][j] = nullptr;
		}
	}
	//playArea.debugPiecePosition();
}

bool ModulePuzzlePiecesV2::PieceCanDrop(PuzzlePiece* piece)
{
	iPoint position = player.position;
	collisionTester->SetPos(position.x, position.y);
	collisionTester->rect.w = PIECE_SIZE;
	collisionTester->rect.h = gravity;

	for (size_t i = 0; i < MAX_WALLS; i++)
	{
		if (walls[i] != nullptr && collisionTester->Intersects(walls[i]->rect)) {
			return true;
		} 
	}

	return false;
}

