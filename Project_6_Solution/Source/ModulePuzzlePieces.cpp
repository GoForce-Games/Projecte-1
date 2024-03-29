#include "ModulePuzzlePieces.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "PuzzlePiece.h"

#include "../External_Libraries/SDL_image/include/SDL_image.h"

#define DEBUG_V2 true

ModulePuzzlePieces::ModulePuzzlePieces(bool startEnabled) : Module()
{
	if (DEBUG_V2)debug = new ModulePuzzlePiecesV2(startEnabled);

}

ModulePuzzlePieces::~ModulePuzzlePieces()
{
	CleanUp();
}

bool ModulePuzzlePieces::Start()
{
	if (DEBUG_V2) return debug->Start();

	textureBomberman = App->textures->Load("Assets/testerman.png");
	if (textureBomberman == nullptr) return false;

	// Animacion temporal, sacado de la demo de R-type que hicimos en clase
	animDefault.PushBack({ 0, 0, 16, 16 });
	animDefault.PushBack({ 16, 0, 16, 16 });
	animDefault.PushBack({ 32, 0, 16, 16 });
	animDefault.PushBack({ 48, 0, 16, 16 });
	animDefault.speed = 0.08f;

	PuzzlePiece* piece = new PuzzlePiece();
	piece->position.create(64, 16);
	piece->collider = App->collisions->AddCollider({ 64,16,16,16 }, Collider::Type::PLAYER, this);
	piece->texture = textureBomberman;
	piece->SetAnimation(&animDefault);
	piece->moving = true;
	currentPiece = AddPuzzlePiece(*piece);

	Collider pCol = *currentPiece->collider;
	collisionTester = new Collider(pCol.rect, pCol.type);





	return true;
}

Update_Status ModulePuzzlePieces::Update()
{
	GamePad& pad = App->input->pads[0];
	if (DEBUG_V2) return debug->Update();
	Update_Status ret = Update_Status::UPDATE_CONTINUE;

	for (uint i = 0; i < MAX_PIECES; i++)
	{
		PuzzlePiece* piece = pieces[i];

		if (piece == nullptr) continue;

		piece->Update();
	}

	if (currentPiece != nullptr && !locked) {
		// Movimiento de jugador


		if (dropDelay == 0) {
			if (!WillCollide({ 0,4 })) {
				dropDelay = fastFall ? MIN_DROP_DELAY : MAX_DROP_DELAY;
				currentPiece->position.y += 4;
			}
			else locked = true;
		}
		else {
			dropDelay--;
		}


		Key_State* keys = App->input->keys;

		// Acelera la ca�da
		if (keys[SDL_Scancode::SDL_SCANCODE_S] == Key_State::KEY_DOWN || pad.a < 0.0f) {
			dropDelay = MIN_DROP_DELAY;
		}

		fastFall = keys[SDL_Scancode::SDL_SCANCODE_S] == Key_State::KEY_REPEAT || pad.b < 0.0f;

		// Mueve a la izquierda
		if (!WillCollide({ -16,0 })) {
			if (keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_DOWN || pad.a < 0.0f) moveDelay = 0;
			if (keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_REPEAT || pad.b < 0.0f && keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_IDLE || pad.x < 0.0f) {

				if (moveDelay == 0) {
					moveDelay = MAX_MOVE_DELAY;
					currentPiece->position.x -= moveSpeed;
				}
				else {
					moveDelay--;
				}
			}
		}

		// Mueve a la derecha
		if (!WillCollide({ 16,0 })) {
			if (keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_DOWN || pad.a < 0.0f) moveDelay = 0;
			if (keys[SDL_Scancode::SDL_SCANCODE_D] == Key_State::KEY_REPEAT || pad.b < 0.0f && keys[SDL_Scancode::SDL_SCANCODE_A] == Key_State::KEY_IDLE || pad.x < 0.0f) {

				if (moveDelay == 0) {
					moveDelay = MAX_MOVE_DELAY;
					currentPiece->position.x += moveSpeed;
				}
				else {
					moveDelay--;
				}
			}
		}

		currentPiece->collider->SetPos(currentPiece->position.x, currentPiece->position.y);

	}


	return ret;
}

Update_Status ModulePuzzlePieces::PostUpdate()
{
	if (DEBUG_V2) return debug->PostUpdate();
	Update_Status ret = Update_Status::UPDATE_CONTINUE;

	for (uint i = 0; i < MAX_PIECES; i++)
	{
		PuzzlePiece* piece = pieces[i];
		if (piece == nullptr) continue;
		SDL_Rect currFrame = piece->currentAnimation->GetCurrentFrame();
		iPoint pos = piece->position;
		SDL_Texture* texture = piece->texture;
		App->render->Blit(texture, pos.x, pos.y, &currFrame);

	}

	return ret;
}

void ModulePuzzlePieces::OnCollision(Collider* c1, Collider* c2)
{

	// Si el listener se guardara en el objeto de colision
	// esto seria mucho mas facil de hacer (y mas rapido de ejecutar)

	/*// Ya no esta en uso ya que se comprueba la colisi�n con las paredes de forma preventiva
	if (c1->type == Collider::Type::PLAYER) {
		switch (c2->type) {
		case Collider::Type::WALL:
		case Collider::Type::PUZZLE_PIECE: {
			break;
		}
		}
	}
	else if (c2->type == Collider::Type::PLAYER) {
		switch (c1->type) {
		case Collider::Type::WALL:
		case Collider::Type::PUZZLE_PIECE: {
			break;
		}
		}
	}*/

}

bool ModulePuzzlePieces::CleanUp()
{
	bool ret = true;

	App->textures->Unload(textureBomb);
	App->textures->Unload(textureBomberman);

	for (uint i = 0; i < MAX_PIECES; i++)
	{
		if (pieces[i] != nullptr) {
			delete pieces[i];
			pieces[i] = nullptr;
		}
	}

	if (DEBUG_V2) return debug->CleanUp();
	return ret;
}

PuzzlePiece* ModulePuzzlePieces::AddPuzzlePiece(const PuzzlePiece& piece)
{


	for (uint i = 0; i < MAX_PIECES; i++) {
		if (pieces[i] == nullptr) {
			PuzzlePiece* newPiece = new PuzzlePiece(piece);
			return pieces[i] = newPiece;
		}
	}
	return nullptr;
}

void ModulePuzzlePieces::RemovePuzzlePiece(PuzzlePiece* piece)
{
	for (size_t i = 0; i < MAX_PIECES; i++)
	{
		if (pieces[i] == piece) {
			delete piece;
			pieces[i] = nullptr;
		}
	}
}

bool ModulePuzzlePieces::WillCollide(iPoint position)
{

	position += currentPiece->position;
	collisionTester->SetPos(position.x, position.y);
	collisionTester->rect.w = PIECE_SIZE;
	collisionTester->rect.h = PIECE_SIZE;

	for (size_t i = 0; i < MAX_WALLS; i++)
	{
		if (walls[i] != nullptr && collisionTester->Intersects(walls[i]->rect)) {
			return true;
		}
	}

	return false;
}
