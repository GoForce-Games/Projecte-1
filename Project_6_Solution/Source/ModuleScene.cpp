#include "ModuleScene.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

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

	bool ret = true;

	//                                      Aquí está la puntuación
	spritesTexture = App->textures->Load("Assets/sprites.png");

	// Colliders ---

	// TODO 2: Add colliders for the first columns of the level

	App->collisions->AddCollider({0,0,70,1024},Collider::Type::WALL_LEFT,(Module*)App->pieces);
	App->collisions->AddCollider({460,0,70,1024},Collider::Type::WALL_RIGHT,(Module*)App->pieces);
	App->collisions->AddCollider({0,690,1024,70},Collider::Type::WALL,(Module*)App->pieces);

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