#include "ModuleScene.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModulePuzzlePieces.h"

SceneLevel1::SceneLevel1(bool startEnabled) : Module(startEnabled)
{

}

SceneLevel1::~SceneLevel1()
{

}

// Load assets
bool SceneLevel1::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/BattleArena.png");
	App->audio->PlayMusic("Assets/stage1.ogg", 1.0f);

	LOG("Loading number assets");

	

	//                                      Aquí está la puntuación
	spritesTexture = App->textures->Load("Assets/Sprites/sprites.png");

	// Colliders ---

	// TODO 2: Add colliders for the first columns of the level
	ModulePuzzlePieces& pieces = *App->pieces;
	pieces.walls[0] = App->collisions->AddCollider({9,9,7,206},Collider::Type::WALL_LEFT,(Module*)App->pieces);
	pieces.walls[1] = App->collisions->AddCollider({144,9,7,206},Collider::Type::WALL_RIGHT,(Module*)App->pieces);
	pieces.walls[2] = App->collisions->AddCollider({9,208,142,7},Collider::Type::WALL,(Module*)App->pieces);

	return ret;
}

Update_Status SceneLevel1::Update()
{
	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}