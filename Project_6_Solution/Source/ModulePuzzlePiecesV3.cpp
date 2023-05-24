#include "ModulePuzzlePiecesV3.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#include "../External_Libraries/SDL/include/SDL.h"
#include "ModuleInput.h"

ModulePuzzlePiecesV3::ModulePuzzlePiecesV3(bool startEnabled) : Module(startEnabled)
{
	// TODO poner animaciones en el constructor
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

	InitAnims();
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
	return true;
}

bool ModulePuzzlePiecesV3::Start()
{
	EnableDependencies();
	LoadTextures();
	InitTemplates();
	playArea.Init(emptyPiece);
	InitWalls();
	InitPlayers();
	InitMisc();

	return true;
}

Update_Status ModulePuzzlePiecesV3::Update()
{
	player.Update();
	playArea.Update();
	ProcessInput();
	ApplyPhysics();
	ApplyLogic(); // TODO logic devuelve estado de juego (?)
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePuzzlePiecesV3::PostUpdate()
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
	return stack;
}

PuzzlePiece* ModulePuzzlePiecesV3::AddPuzzlePiece(const PuzzlePiece& piece, Collider::Type type)
{
	for (uint i = 0; i < MAX_PIECES; i++) {
		if (pieces[i] == nullptr) {
			//Crea nueva pieza con una caja de colision copiada de la plantilla
			PuzzlePiece* newPiece = new PuzzlePiece(piece);
			newPiece->collider = App->collisions->AddCollider(templateMan->collider->rect, type);
			if (newPiece->collider != nullptr) //TODO solucionar problema de colliders
				newPiece->collider->SetPos(newPiece->position.x, newPiece->position.y);
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
			if (piece->collider != nullptr) {
				piece->collider->pendingToDelete = true;
				piece->collider = nullptr;
			}
			delete piece;
			pieces[i] = nullptr;
			break;
		}

	}
}

bool ModulePuzzlePiecesV3::WillCollide(PlayerCollisionCheck direction)
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
		//Hace que el colisionador ocupe todo el ancho del jugador
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
		//Hace que el colisionador ocupe todo el alto del jugador
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
				else
					LOG("TRIED TO REPLACE EXISTING PIECE AT COORDINATES (x:%i, y:%i)\n", posTablero.x + j, posTablero.y + i);
			}
			player.pieces[i][j] = nullptr;
		}
	}
	playArea.debugPiecePosition();
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
	// TODO textura para probar, hay que recortar el spritesheet
	textureBomberman = App->textures->Load("Assets/testerman.png");
}

void ModulePuzzlePiecesV3::InitAnims()
{
	// Animacion temporal, sacado de la demo de R-type que hicimos en clase
	animDefault.PushBack({ 0, 0, 16, 16 });
	animDefault.PushBack({ 16, 0, 16, 16 });
	animDefault.PushBack({ 32, 0, 16, 16 });
	animDefault.PushBack({ 48, 0, 16, 16 });
	animDefault.speed = 0.08f;

	animNone.PushBack({ 0,0,0,0 });
	animNone.loop = false;
	animNone.speed = 1.0f;
}

void ModulePuzzlePiecesV3::InitTemplates()
{
	templateMan = new PuzzlePiece();

	templateMan->collider = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::NONE);
	templateMan->position.create(-64, -64);
	templateMan->texture = textureBomberman;
	templateMan->SetAnimation(&animDefault);
	templateMan->moving = false;
	templateMan->type = PieceType::WHITE;


	emptyPiece = AddPuzzlePiece(*templateMan, Collider::Type::NONE);
	emptyPiece->currentAnimation = &animNone;
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
		piece->collider->name = "WALL";
		offset.y += PIECE_SIZE;
	}

	// Columna derecha
	offset = playArea.position;
	offset.x += PIECE_SIZE * (PLAY_AREA_W - 1);
	for (size_t i = 0; i < PLAY_AREA_H; i++)
	{
		templateWall->position = offset;
		RemovePuzzlePiece(playArea.table[i][PLAY_AREA_W - 1]);
		PuzzlePiece* piece = playArea.table[i][PLAY_AREA_W - 1] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		piece->collider->name = "WALL";
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
		piece->collider->name = "WALL";
		offset.x += PIECE_SIZE;
	}

	// Techo (por ciertas razones relacionadas con la deteccion de grupos)
	offset = playArea.position;
	offset.x += PIECE_SIZE;
	for (size_t i = 1; i < PLAY_AREA_W - 1; i++)
	{
		templateWall->position = offset;
		RemovePuzzlePiece(playArea.table[0][i]);
		PuzzlePiece* piece = playArea.table[0][i] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		piece->collider->name = "WALL";
		offset.x += PIECE_SIZE;
	}

	delete templateWall;
}

void ModulePuzzlePiecesV3::InitPlayers()
{
	player.gamepad = &(App->input->pads[0]);

	player.position.create(64, 16);

	PuzzlePiece* newPieces[4];
	newPieces[0] = AddPuzzlePiece(*templateMan);
	newPieces[1] = AddPuzzlePiece(*templateMan);
	newPieces[2] = AddPuzzlePiece(*templateMan);
	newPieces[3] = AddPuzzlePiece(*emptyPiece);
	player.setPieces(newPieces);
	player.locked = false;
}

void ModulePuzzlePiecesV3::InitMisc()
{
	collisionTester = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::PLAYER);

	playArea.collisionTester = collisionTester;
}

void ModulePuzzlePiecesV3::ProcessInput()
{
	if (!player.locked) {
		GamePad& pad = *(player.gamepad);

		//Lee input

		Key_State* keys = App->input->keys;


		// Godmode: activa/desactiva gravedad
		if (keys[SDL_Scancode::SDL_SCANCODE_F9] == Key_State::KEY_DOWN) {
			gravity = (gravity == 0) ? GRAVITY : 0;
		}


		// Rotacion

		if ((keys[SDL_Scancode::SDL_SCANCODE_P] == Key_State::KEY_DOWN) || pad.a) {
			player.Rotate();
		}



		// Acelera la caída
		if (keys[SDL_Scancode::SDL_SCANCODE_S] == Key_State::KEY_DOWN) {
			dropDelay = MIN_DROP_DELAY;
		}
		fastFall = keys[SDL_Scancode::SDL_SCANCODE_S] == Key_State::KEY_REPEAT;


		//El primer frame en el que intentas moverte a un lado es instantaneo
		if (keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_DOWN) moveDelay = 0;
		else if (keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_DOWN)	moveDelay = 0;


		// Mueve a la izquierda
		if (keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_REPEAT && keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_IDLE) {

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
	}
}

void ModulePuzzlePiecesV3::ApplyPhysics()
{
	if (!player.locked) {
		//Aplica gravedad

		if (dropDelay == 0) {
			if (!WillCollide(PlayerCollisionCheck::BOTTOM)) {
				dropDelay = fastFall ? MIN_DROP_DELAY : MAX_DROP_DELAY;
				player.position.y += gravity;
			}
			else {
				player.locked = true;
				PlacePieces();
				player.position.create(64, 16);
			}
			WillCollide(PlayerCollisionCheck::DEBUG); // Curiosamente quitar esto rompe la colision con el borde de abajo
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
	}
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
	nPoint += localArea.position;
	nPoint.x *= PIECE_SIZE;
	nPoint.y *= PIECE_SIZE;

	return nPoint;
}
