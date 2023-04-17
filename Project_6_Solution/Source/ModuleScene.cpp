#include "ModuleScene.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModulePuzzlePieces.h"

ModuleScene::ModuleScene()
{

}

ModuleScene::~ModuleScene()
{

}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/background.png");
	App->audio->PlayMusic("Assets/stage1.ogg", 1.0f);

	LOG("Loading number assets");

	

	//                                      Aquí está la puntuación
	spritesTexture = App->textures->Load("Assets/sprites.png");

	// Colliders ---

	// TODO 2: Add colliders for the first columns of the level
	ModulePuzzlePieces& pieces = *App->pieces;
	pieces.walls[0] = App->collisions->AddCollider({9,9,7,206},Collider::Type::WALL_LEFT,(Module*)App->pieces);
	pieces.walls[1] = App->collisions->AddCollider({144,9,7,206},Collider::Type::WALL_RIGHT,(Module*)App->pieces);
	pieces.walls[2] = App->collisions->AddCollider({9,208,142,7},Collider::Type::WALL,(Module*)App->pieces);

	return ret;
}

update_status ModuleScene::Update()
{
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleScene::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return update_status::UPDATE_CONTINUE;
}