#include "ModulePuzzlePieces.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleTextures.h"

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


	// textura para probar, hay que recortar el spritesheet
	textureBomberman = App->textures->Load("Assets/option.png");
	if (textureBomberman == nullptr) return false;

	// Animacion temporal, sacado de la demo de R-type que hicimos en clase
	animDefault.PushBack({4,8,64,56});
	animDefault.PushBack({72,8,64,56});
	animDefault.PushBack({140,8,64,56});
	animDefault.PushBack({276,8,64,56});
	animDefault.PushBack({344,8,64,56});
	animDefault.PushBack({412,8,64,56});
	animDefault.PushBack({480,8,64,56});
	animDefault.PushBack({548,8,64,56});
	animDefault.PushBack({616,8,64,56});
	animDefault.PushBack({684,8,64,56});
	animDefault.PushBack({752,8,64,56});
	animDefault.speed = 0.08f;

	PuzzlePiece* piece = new PuzzlePiece();
	piece->position.create(224, 100);
	piece->collider = App->collisions->AddCollider({ 32,32,32,32 }, Collider::Type::PUZZLE_PIECE, this);
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

		if (dropDelay == 0) {
			dropDelay = MAX_DROP_DELAY;
			currentPiece->position.y += 32;
		}
		else {
			dropDelay--;
		}



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
