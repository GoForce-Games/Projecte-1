#include "ModulePuzzlePieces.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

#include "../External_Libraries/SDL_image/include/SDL_image.h"

ModulePuzzlePieces::ModulePuzzlePieces()
{
	for (uint i = 0; i < MAX_PIECES; ++i)
		pieces[i] = nullptr;


}

ModulePuzzlePieces::~ModulePuzzlePieces()
{
	for (uint i = 0; i < MAX_PIECES; i++)
	{
		if (pieces[i] != nullptr) {
			delete pieces[i];
			pieces[i] = nullptr;
		}
	}
}

bool ModulePuzzlePieces::Start()
{


	// TODO textura para probar, hay que recortar el spritesheet
	textureBomberman = App->textures->Load("Assets/option.png");
	if (textureBomberman == nullptr) return false;

	// Animacion temporal, sacado de la demo de R-type que hicimos en clase
	animDefault.PushBack({ 4,8,64,56 });
	animDefault.PushBack({ 72,8,64,56 });
	animDefault.PushBack({ 140,8,64,56 });
	animDefault.PushBack({ 276,8,64,56 });
	animDefault.PushBack({ 344,8,64,56 });
	animDefault.PushBack({ 412,8,64,56 });
	animDefault.PushBack({ 480,8,64,56 });
	animDefault.PushBack({ 548,8,64,56 });
	animDefault.PushBack({ 616,8,64,56 });
	animDefault.PushBack({ 684,8,64,56 });
	animDefault.PushBack({ 752,8,64,56 });
	animDefault.speed = 0.08f;

	PuzzlePiece* piece = new PuzzlePiece();
	piece->position.create(224, 100);
	piece->collider = App->collisions->AddCollider({ 224,100,64,64 }, Collider::Type::PLAYER, this);
	piece->texture = textureBomberman;
	piece->SetAnimation(animDefault);
	piece->moving = true;
	currentPiece = AddPuzzlePiece(*piece);


	return true;
}

update_status ModulePuzzlePieces::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	for (uint i = 0; i < MAX_PIECES; i++)
	{
		PuzzlePiece* piece = pieces[i];

		if (piece == nullptr) continue;

		piece->currentAnimation.Update();
	}

	if (currentPiece != nullptr) {
		// Movimiento de jugador

		if (dropDelay == 0 && collidingWith != Collider::Type::WALL) {
			dropDelay = fastFall ? MIN_DROP_DELAY : MAX_DROP_DELAY;
			currentPiece->position.y += 32;
		}
		else {
			dropDelay--;
		}


		KEY_STATE* keys = App->input->keys;

		// Acelera la caída
		if (keys[SDL_Scancode::SDL_SCANCODE_S] == KEY_STATE::KEY_DOWN) {
			dropDelay = MIN_DROP_DELAY;
		}

		fastFall = keys[SDL_Scancode::SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT;

		if (collidingWith != Collider::Type::WALL_LEFT) {
			if (keys[SDL_Scancode::SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN) moveDelay = 0;
			if (keys[SDL_Scancode::SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT) {

				if (collidingWith == Collider::Type::WALL_RIGHT)
					collidingWith = Collider::Type::NONE;

				if (moveDelay == 0) {
					moveDelay = MAX_MOVE_DELAY;
					currentPiece->position.x -= moveSpeed;
				}
				else {
					moveDelay--;
				}
			}
		}

		if (collidingWith != Collider::Type::WALL_RIGHT) {
			if (keys[SDL_Scancode::SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN) moveDelay = 0;
			if (keys[SDL_Scancode::SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT) {

				if (collidingWith == Collider::Type::WALL_LEFT)
					collidingWith = Collider::Type::NONE;

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

update_status ModulePuzzlePieces::PostUpdate()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	for (uint i = 0; i < MAX_PIECES; i++)
	{
		PuzzlePiece* piece = pieces[i];
		if (piece == nullptr) continue;
		SDL_Rect currFrame = piece->currentAnimation.GetCurrentFrame();
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

	if (c1->type == Collider::Type::PLAYER) {
		switch (c2->type) {
		case Collider::Type::WALL: // TODO poner una funcion para colocar la pieza y hacer aparecer una nueva
		case Collider::Type::WALL_LEFT:
		case Collider::Type::WALL_RIGHT:
		case Collider::Type::PUZZLE_PIECE: {
			collidingWith = c2->type;
			break;
		}
		}
	}
	else if (c2->type == Collider::Type::PLAYER) {
		switch (c1->type) {
		case Collider::Type::WALL_LEFT:
		case Collider::Type::WALL_RIGHT:
		case Collider::Type::WALL:
		case Collider::Type::PUZZLE_PIECE: {
			collidingWith = c2->type;
			break;
		}
		}
	}

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
	delete piece;
}
