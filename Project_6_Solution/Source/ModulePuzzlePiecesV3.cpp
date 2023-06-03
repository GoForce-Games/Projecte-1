#pragma once
#include "ModulePuzzlePiecesV3.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "Puntuation.h"
#include "ModuleFadeToBlack.h"

#include "../External_Libraries/SDL/include/SDL.h"
#include <algorithm>

ModulePuzzlePiecesV3::ModulePuzzlePiecesV3(bool startEnabled) : Module(startEnabled)
{
	dropDelay = MAX_DROP_DELAY;
	moveDelay = MAX_MOVE_DELAY;
	moveSpeed = MOVE_SPEED;
	gravity = GRAVITY;
	wallCount = 0;
	for (uint i = 0; i < MAX_PIECES; i++)
	{
		pieces[i] = nullptr;
	}

	for (uint i = 0; i < MAX_WALLS; i++)
	{
		walls[i] = nullptr;
	}
}

ModulePuzzlePiecesV3::~ModulePuzzlePiecesV3()
{
}

bool ModulePuzzlePiecesV3::Init()
{
	for (uint i = 0; i < MAX_PIECES; i++)
	{
		pieces[i] = nullptr;
	}

	for (uint i = 0; i < MAX_WALLS; i++)
	{
		walls[i] = nullptr;
	}

	InitAnims();

	return true;
}

bool ModulePuzzlePiecesV3::Start()
{
	//TODO cambiar playArea y player a variables dinamicas para mayor flexibilidad si se pone multijugador (debe contener el jugador en la variable dedicada)
	EnableDependencies();
	LoadTextures();
	InitTemplates();
	InitWalls();
	InitPlayers();
	InitMisc();

	return true;
}

Update_Status ModulePuzzlePiecesV3::Update()
{
	switch (playArea.state)
	{
	case PlayAreaState::INIT: {
		if (true) { // TODO cambiar esta condicion por final de animacion de inicio
			playArea.state = PlayAreaState::GAME_START;
		}
		return Update_Status::UPDATE_CONTINUE;
	}
	case PlayAreaState::GAME_START: {
		LOG("Game started");
		playArea.state = PlayAreaState::GAME_LOOP;
		// No hay break para que también se ejecute el game loop en este frame
	}
	case PlayAreaState::GAME_LOOP: {
		//Este caso debe encontrarse despues de GAME_START
		ProcessInput();
		ApplyPhysics();
		break;
	}
	case PlayAreaState::PIECES_PLACED: {

		ApplyLogic(); // TODO logic devuelve estado de juego (?)
		break;
	}
	case PlayAreaState::CHECK_GROUPS: {

		break;
	}
	case PlayAreaState::DELETE_GROUPS: {
		RemoveGroups();
		break;
	}
	case PlayAreaState::DETONATE_BOMBS: {

		break;
	}
	case PlayAreaState::NEW_PIECES: {

		break;
	}
	case PlayAreaState::PAUSE: {
		return Update_Status::UPDATE_CONTINUE;
	}
	case PlayAreaState::GAME_END: {
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->win_lose, 90);
		playArea.state = NEW_GAME;
		break;
	}
	case PlayAreaState::NEW_GAME: {

		break;
	}
	default:
		break;
	}
	player.Update();
	playArea.Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePuzzlePiecesV3::PostUpdate()
{
	for (size_t i = 0; i < MAX_PIECES; i++)
	{
		PuzzlePiece* p = pieces[i];
		if (p == nullptr || p->type == PieceType::NONE || p->type == PieceType::WALL) continue;
		SDL_Rect& currFrame = p->currentAnimation.GetCurrentFrame();
		iPoint& pos = p->position;
		SDL_Texture* texture = p->texture;
		App->render->Blit(texture, pos.x, pos.y, &currFrame);
	}
	return Update_Status::UPDATE_CONTINUE;
}

void ModulePuzzlePiecesV3::OnCollision(Collider* c1, Collider* c2)
{
}

bool ModulePuzzlePiecesV3::CleanUp()
{
	RemovePuzzlePiece(templateMan);
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

std::stack<PuzzlePiece*>& ModulePuzzlePiecesV3::GeneratePuzzlePieces(std::stack<PuzzlePiece*>& stack, uint amount)
{
	for (size_t i = 0; i < amount; i++)
	{
		PuzzlePiece* newPiece = AddPuzzlePiece(*templateMan, Collider::Type::PUZZLE_PIECE);
		PieceType type = (PieceType)((rand() % 4) + 1); // TODO CAMBIAR ESTO 
		newPiece->SetType(type);
		stack.push(newPiece);
	}
	return stack;
}

PuzzlePiece* ModulePuzzlePiecesV3::AddPuzzlePiece(const PuzzlePiece& piece, Collider::Type type)
{
	for (uint i = 0; i < MAX_PIECES; i++) {
		if (pieces[i] == nullptr) {
			//Crea nueva pieza con una caja de colision copiada de la plantilla
			PuzzlePiece* newPiece = new PuzzlePiece(piece);
			/*
			newPiece->collider = App->collisions->AddCollider(templateMan->collider->rect, type);
			if (newPiece->collider != nullptr)
				newPiece->collider->SetPos(newPiece->position.x, newPiece->position.y);
			*/
			newPiece->SetType(piece.type);
			return pieces[i] = newPiece;
		}
	}
	return nullptr;
}

void ModulePuzzlePiecesV3::RemovePuzzlePiece(PuzzlePiece* piece)
{
	if (piece == nullptr) return;

	piece->name = "To be deleted"; // Para debug

	for (size_t i = 0; i < MAX_PIECES; i++)
	{
		if (pieces[i] == piece) {
			delete piece;
			pieces[i] = nullptr;
			break;
		}

	}
}

/*bool ModulePuzzlePiecesV3::WillCollide(PlayerCollisionCheck direction)
{
	// TODO usar el modulo de colisiones existente si puede ser
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

		//Si el colisionador de la pieza correspondiente esta desactivado no hace falta comprobar más allá de donde se encuentra
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
		//Hace que el colisionador ocupe el ancho del jugador
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
		//Hace que el colisionador ocupe el alto del jugador
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
}*/

bool ModulePuzzlePiecesV3::CheckOutOfBounds(PlayArea* area, PlayerPieceV2* player)
{
	iPoint playerPos = player->position;
	iPoint gridPos = WorldToLocal(*area, playerPos);
	return (gridPos.x <= 0 || gridPos.x >= PLAY_AREA_W || gridPos.y <= 0 || gridPos.y >= PLAY_AREA_H);
}

bool ModulePuzzlePiecesV3::CanGoLeft(PlayArea* area, PlayerPieceV2* player)
{
	// TODO finish this
	iPoint playerPos = player->position;
	iPoint gridPos = WorldToLocal(*area, playerPos);
	if (CheckOutOfBounds(area, player)) {
		LOG("Can't move left, player is out of bounds: x=%i, y=%i", gridPos.x, gridPos.y);
		return false;
	}
	if (gridPos.x < 2) // Tiene pared a la izquierda
		return false;
	int offsetTop = (player->pieces[0][0]->isEmpty) ? 0 : 1;
	int offsetBot = (player->pieces[1][0]->isEmpty) ? 0 : 1;
	PuzzlePiece* leftTop = area->table[gridPos.y][gridPos.x - offsetTop];
	PuzzlePiece* leftBot = area->table[gridPos.y + 1][gridPos.x - offsetBot];

	return leftTop->type == PieceType::NONE && leftBot->type == PieceType::NONE;
	/*
	//Check top piece
	if (!player->pieces[0][0]->isEmpty || !player->pieces[0][1]->isEmpty) {
		if (leftTop->type != PieceType::NONE) {
			return false;
		}
	}
	//Check bottom piece
	if (!player->pieces[1][0]->isEmpty || !player->pieces[1][1]->isEmpty) {
		if (leftBot->type != PieceType::NONE) {
			return false;
		}
	}

	return true;
	*/
}

bool ModulePuzzlePiecesV3::CanGoRight(PlayArea* area, PlayerPieceV2* player)
{
	iPoint playerPos = player->position;
	iPoint gridPos = WorldToLocal(*area, playerPos);
	if (CheckOutOfBounds(area, player)) {
		LOG("Can't move right, player is out of bounds: x=%i, y=%i", gridPos.x, gridPos.y);
		return false;
	}
	if (gridPos.x > PLAY_AREA_W - 4) // Tiene pared a la derecha
		return false;
	int offsetTop = (player->pieces[0][1]->isEmpty) ? 1 : 2;
	int offsetBot = (player->pieces[1][1]->isEmpty) ? 1 : 2;
	PuzzlePiece* rightTop = area->table[gridPos.y][gridPos.x + offsetTop];
	PuzzlePiece* rightBot = area->table[gridPos.y + 1][gridPos.x + offsetBot];

	return rightTop->type == PieceType::NONE && rightBot->type == PieceType::NONE;

	/*
	//Check top piece
	if (!player->pieces[0][1]->isEmpty || !player->pieces[0][0]->isEmpty) {
		if (rightTop->type != PieceType::NONE) {
			return false;
		}
	}
	//Check bottom piece
	if (!player->pieces[1][1]->isEmpty || !player->pieces[1][0]->isEmpty) {
		if (rightBot->type != PieceType::NONE) {
			return false;
		}
	}
	return true;
	*/
}

bool ModulePuzzlePiecesV3::CanGoDown(PlayArea* area, PlayerPieceV2* player)
{
	iPoint playerPos = player->position;
	//Ya esta dentro de una casilla, no compruebes hasta que vaya a pasar a la siguiente
	if ((playerPos.y + PIECE_SIZE + gravity) % PIECE_SIZE != 0) return true;


	iPoint gridPos = WorldToLocal(*area, playerPos);
	if (CheckOutOfBounds(area, player)) {
		LOG("Can't move down, player is out of bounds: x=%i, y=%i", gridPos.x, gridPos.y);
		return false;
	}

	int offsetLeft = (player->pieces[1][0]->isEmpty) ? 1 : 2;
	int offsetRight = (player->pieces[1][1]->isEmpty) ? 1 : 2;
	PuzzlePiece* botLeft = area->table[gridPos.y + offsetLeft][gridPos.x];
	PuzzlePiece* botRight = area->table[gridPos.y + offsetRight][gridPos.x + 1];


	return botLeft->type == PieceType::NONE && botRight->type == PieceType::NONE;
	/*
	// Check left piece
	if (!player->pieces[1][0]->isEmpty) {
		if (botLeft->type != PieceType::NONE) {
			return false;
		}
	}

	// Check right piece (if active piece is bomb this has to be skipped)
	if (!player->pieces[1][1]->isEmpty || !player->pieces[0][1]->isEmpty) {
		if (botLeft->type != PieceType::NONE) {
			return false;
		}
	}

	return true;
	*/
}

void ModulePuzzlePiecesV3::PlacePieces()
{
	iPoint posTablero = WorldToLocal(playArea, player.position);
	posTablero.y--;

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (!player.pieces[i][j]->isEmpty) {
				if (playArea.table[posTablero.y + i][posTablero.x + j]->isEmpty) {
					RemovePuzzlePiece(playArea.table[posTablero.y + i][posTablero.x + j]);
					playArea.table[posTablero.y + i][posTablero.x + j] = player.pieces[i][j];
				}
				else { //TODO: este puede ser un buen sitio para asignar la variable responsable de la condicion de fin de partida, ya que en un principio solo se intentara sobreescribir piezas si ya se ha llegado arriba del todo en la zona de juego
					//Si las coordenadas del jugador son las iniciales entonces fin de juego (?)
					playArea.state = PlayAreaState::GAME_END;
					RemovePuzzlePiece(playArea.table[posTablero.y + i][posTablero.x + j]);
					playArea.table[posTablero.y + i][posTablero.x + j] = player.pieces[i][j];

					LOG("TRIED TO REPLACE EXISTING PIECE AT COORDINATES (x:%i, y:%i)\n", posTablero.x + j, posTablero.y + i);
				}
			}
			player.pieces[i][j] = nullptr;
		}
	}

	//playArea.debugPiecePosition();
}

bool ModulePuzzlePiecesV3::PieceCanDrop(PuzzlePiece* piece)
{
	return false;
}

void ModulePuzzlePiecesV3::EnableDependencies()
{
	App->collisions->Enable();
}

void ModulePuzzlePiecesV3::LoadTextures()
{
	textureBomberman = App->textures->Load("Assets/Sprites/HeadsAndBombs.png");
}

void ModulePuzzlePiecesV3::InitAnims()
{
	//TODO: Arreglar animaciones. Podeis cambiar el como se crean las animaciones si quereis para tener un poco mas de flexibilidad

	// Animacion temporal, sacado de la demo de R-type que hicimos en clase
	animDefaultTest.PushBack({ 0, 0, 16, 16 });
	animDefaultTest.PushBack({ 16, 0, 16, 16 });
	animDefaultTest.PushBack({ 32, 0, 16, 16 });
	animDefaultTest.PushBack({ 48, 0, 16, 16 });
	animDefaultTest.speed = 0.08f;

	animNone.PushBack({ 0,0,1,1 });

	animIdle[PieceType::NONE] = animNone;

	//Animaciones por tipo de pieza
	iPoint pixelCoords;
	SDL_Rect firstFrame;

	// Animacion bomberman /Negro
	pixelCoords.create(0, 0);
	firstFrame = { pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE };
	for (size_t i = 0; i < 13; i++)
	{
		animIdle[PieceType::BLACK].PushBack({ pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE });
		pixelCoords.x += PIECE_SIZE;
	}
	animIdle[PieceType::BLACK].PushBack(firstFrame);

	// Animacion bomberman Blanco
	pixelCoords.x = 0;
	pixelCoords.y += PIECE_SIZE;
	firstFrame = { pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE };
	for (size_t i = 0; i < 9; i++)
	{
		animIdle[PieceType::WHITE].PushBack({ pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE });
		pixelCoords.x += PIECE_SIZE;
	}
	animIdle[PieceType::WHITE].PushBack(firstFrame);

	// Animacion bomberman Rojo
	pixelCoords.x = 0;
	pixelCoords.y += PIECE_SIZE;
	firstFrame = { pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE };
	for (size_t i = 0; i < 9; i++)
	{
		animIdle[PieceType::RED].PushBack({ pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE });
		pixelCoords.x += PIECE_SIZE;
	}
	animIdle[PieceType::RED].PushBack(firstFrame);

	// Animacion bomberman Azul
	pixelCoords.x = 0;
	pixelCoords.y += PIECE_SIZE;
	firstFrame = { pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE };
	for (size_t i = 0; i < 3; i++)
	{
		animIdle[PieceType::BLUE].PushBack({ pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE });
		pixelCoords.x += PIECE_SIZE;
	}
	animIdle[PieceType::BLUE].PushBack(firstFrame);

	// Animacion bomberman Verde
	pixelCoords.x = 0;
	pixelCoords.y += PIECE_SIZE;
	firstFrame = { pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE };
	for (size_t i = 0; i < 2; i++)
	{
		animIdle[PieceType::GREEN].PushBack({ pixelCoords.x,pixelCoords.y,PIECE_SIZE,PIECE_SIZE });
		pixelCoords.x += PIECE_SIZE;
	}
	animIdle[PieceType::GREEN].PushBack(firstFrame);

	//Desactiva bucle para las piezas
	for (size_t i = 0; i < PieceType::MAX; i++)
	{
		animIdle[i].loop = false;
		animIdle[i].speed = 0.08f;
	}

}

void ModulePuzzlePiecesV3::InitTemplates()
{
	templateMan = new PuzzlePiece();

	//templateMan->collider = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::NONE);
	templateMan->position.create(-64, -64);
	templateMan->texture = textureBomberman;
	templateMan->SetAnimation(&animDefaultTest);
	templateMan->moving = false;
	templateMan->type = PieceType::WHITE;


	emptyPiece = AddPuzzlePiece(*templateMan, Collider::Type::NONE);
	emptyPiece->currentAnimation = animIdle[PieceType::NONE];
	emptyPiece->isEmpty = true;
	emptyPiece->type = PieceType::NONE;
}

void ModulePuzzlePiecesV3::InitWalls()
{
	PuzzlePiece* templateWall = new PuzzlePiece(*emptyPiece);
	templateWall->moving = false;
	templateWall->type = PieceType::WALL;

	// Columna izquierda
	iPoint offset = playArea.position;
	//offset.y += PIECE_SIZE;
	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		templateWall->position = offset;
		RemovePuzzlePiece(playArea.table[i][0]);
		PuzzlePiece* piece = playArea.table[i][0] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		//piece->collider->name = "WALL";
		offset.y += PIECE_SIZE;
	}

	// Columna derecha
	offset = playArea.position;
	offset.x += PIECE_SIZE * (PLAY_AREA_W - 1);
	//offset.y += PIECE_SIZE;
	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		templateWall->position = offset;
		RemovePuzzlePiece(playArea.table[i][PLAY_AREA_W - 1]);
		PuzzlePiece* piece = playArea.table[i][PLAY_AREA_W - 1] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		//piece->collider->name = "WALL";
		offset.y += PIECE_SIZE;

	}


	// Fondo
	offset = playArea.position;
	offset.y += PIECE_SIZE * (PLAY_AREA_H);
	for (size_t i = 0; i < PLAY_AREA_W; i++)
	{
		templateWall->position = offset;
		RemovePuzzlePiece(playArea.table[PLAY_AREA_H - 1][i]);
		PuzzlePiece* piece = playArea.table[PLAY_AREA_H - 1][i] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		//piece->collider->name = "WALL";
		offset.x += PIECE_SIZE;
	}

	// Techo (por ciertas razones relacionadas con la deteccion de grupos)
	offset = playArea.position;
	//offset.x += PIECE_SIZE;
	for (size_t i = 0; i < PLAY_AREA_W - 1; i++)
	{
		templateWall->position = offset;
		RemovePuzzlePiece(playArea.table[0][i]);
		PuzzlePiece* piece = playArea.table[0][i] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		//piece->collider->name = "WALL";
		offset.x += PIECE_SIZE;
	}

	delete templateWall;

}

void ModulePuzzlePiecesV3::InitPlayers()
{
	playArea.player = &player;
	player.gamepad = &(App->input->control[0]);

	player.position.create(64, 16);

	while (pieceQueue.size() > 0)
	{
		pieceQueue.pop();
	}
	if (pieceQueue.size() < 3)
		GeneratePuzzlePieces(pieceQueue, 12);

	PuzzlePiece* newPieces[4];
	newPieces[0] = pieceQueue.top();
	pieceQueue.pop();
	newPieces[1] = pieceQueue.top();
	pieceQueue.pop();
	newPieces[2] = pieceQueue.top();
	pieceQueue.pop();
	newPieces[3] = AddPuzzlePiece(*emptyPiece);
	player.setPieces(newPieces);
	player.locked = false;

	playArea.Init(emptyPiece);
	playArea.player = &player;
}

void ModulePuzzlePiecesV3::InitMisc()
{
	collisionTester = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::PLAYER);

	playArea.collisionTester = collisionTester;
}

void ModulePuzzlePiecesV3::ProcessInput()
{
	if (!player.locked) {
		PlayerInput& pad = *(player.gamepad);

		//Lee input

		Key_State* keys = App->input->keys;


		// Godmode: activa/desactiva gravedad
		if (keys[SDL_Scancode::SDL_SCANCODE_F9] == Key_State::KEY_DOWN) {
			gravity = (gravity == 0) ? GRAVITY : 0;
		}


		// Rotacion

		if (pad.rotatePiece == Key_State::KEY_DOWN) {
			player.Rotate();
		}



		// Acelera la caída
		if (pad.fastFall == Key_State::KEY_DOWN) {
			dropDelay = MIN_DROP_DELAY;
		}

		fastFall = pad.fastFall == Key_State::KEY_REPEAT;


		//El primer frame en el que intentas moverte a un lado es instantaneo
		if (pad.moveRight == Key_State::KEY_DOWN) moveDelay = 0;
		else if (pad.moveLeft == Key_State::KEY_DOWN) moveDelay = 0;


		// Mueve a la izquierda
		if (pad.moveLeft == Key_State::KEY_REPEAT && pad.moveRight == Key_State::KEY_IDLE) {

			if (CanGoLeft(&playArea, &player)) {

				//if (!WillCollide(PlayerCollisionCheck::LEFT)) { //function being deprecated

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
		if (pad.moveRight == Key_State::KEY_REPEAT && pad.moveLeft == Key_State::KEY_IDLE) {
			if (CanGoRight(&playArea, &player)) {
				//if (!WillCollide(PlayerCollisionCheck::RIGHT)) {

				if (moveDelay == 0) {
					moveDelay = MAX_MOVE_DELAY;
					player.position.x += moveSpeed;
				}
				else {
					moveDelay--;
				}
			}
		}
	}
}

void ModulePuzzlePiecesV3::ApplyPhysics()
{
	if (!player.locked) {
		//Aplica gravedad

		if (dropDelay == 0) {
			if (CanGoDown(&playArea, &player)) {
				//if (!WillCollide(PlayerCollisionCheck::BOTTOM)) {
				dropDelay = fastFall ? MIN_DROP_DELAY : MAX_DROP_DELAY;
				player.position.y += gravity;
			}
			else {
				player.position.y += gravity; //Sin esto las piezas se colocan una celda mas arriba (necesita hacer un ciclo mas de caida antes de fijar las piezas)
				player.locked = true;
				App->puntuation->score = App->puntuation->score + 9; // Suma 9 puntos en el marcador al colocar la pieza
				playArea.state = PlayAreaState::PIECES_PLACED;
				PlacePieces();
				player.position.create(64, 16); // Reset de posicion
				player.position += playArea.position; // Posicion relativa a zona de juego
			}
			//WillCollide(PlayerCollisionCheck::DEBUG); // Curiosamente quitar esto rompe la colision con el borde de abajo (Actualizacion: ya no debería ocurrir, pero esto se queda por visualización de colisiones)
		}
		else {
			dropDelay--;
		}
	}
}

void ModulePuzzlePiecesV3::ApplyLogic()
{
	// No se aplica logica cuando hay una pieza en juego
	if (player.locked) {
		playArea.DropPieces();
		if (playArea.checkGroupedPieces()) {
			playArea.state = PlayAreaState::DELETE_GROUPS;
			return;
		}

		if (pieceQueue.size() < 3)
			GeneratePuzzlePieces(pieceQueue, 12);


		//A partir de aqui solo cuando ya se haya procesado todo el tablero y se pueda sacar una pieza nueva

		PuzzlePiece* newPieces[4];
		newPieces[0] = pieceQueue.top();
		pieceQueue.pop();
		newPieces[1] = pieceQueue.top();
		pieceQueue.pop();
		newPieces[2] = pieceQueue.top();
		pieceQueue.pop();
		newPieces[3] = AddPuzzlePiece(*emptyPiece);
		player.setPieces(newPieces);

		player.locked = false;
		playArea.state = PlayAreaState::GAME_LOOP;
	}
}

void ModulePuzzlePiecesV3::RemoveGroups()
{
	while (playArea.piecesToRemove.size() > 0) {
		PuzzlePiece* p = playArea.piecesToRemove.back();
		*p = *emptyPiece; // Copia parámetros de casilla vacía
		if (playArea.piecesToRemove.size() == 3)
		{
			App->puntuation->score += 50;
		}
		if (playArea.piecesToRemove.size() == 4)
		{
			App->puntuation->score += 75;
		}
		if (playArea.piecesToRemove.size() == 5)
		{
			App->puntuation->score += 150;
		}
		if (playArea.piecesToRemove.size() == 6)
		{
			App->puntuation->score += 300;
		}
		playArea.piecesToRemove.pop_back();
	}
	playArea.DropPieces(); // Aplica gravedad para que no haya piezas flotantes

	playArea.state = PlayAreaState::PIECES_PLACED; // Vuelve a comprobar si hay más grupos que eliminar (cadena)
}

iPoint ModulePuzzlePiecesV3::WorldToLocal(PlayArea& localArea, iPoint sCoordinates)
{
	iPoint nPoint;
	nPoint.create(sCoordinates.x, sCoordinates.y);
	nPoint -= localArea.position;
	nPoint.x /= PIECE_SIZE;
	nPoint.y /= PIECE_SIZE;

	return nPoint;
}

iPoint ModulePuzzlePiecesV3::AreaToWorld(PlayArea& localArea, iPoint lCoordinates)
{
	iPoint nPoint;
	nPoint.create(lCoordinates.x, lCoordinates.y);
	nPoint.x *= PIECE_SIZE;
	nPoint.y *= PIECE_SIZE;
	nPoint += localArea.position;

	return nPoint;
}
