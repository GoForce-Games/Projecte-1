#include "ModuleScene.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModulePuzzlePieces.h"
#include "Puntuation.h"
#include "ModuleWinLose.h"
#include "ModuleFadeToBlack.h"
#include "Intro.h"
#include "ModuleLose.h"

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

	// Las paredes se instancian en ModulePuzzlePieces porque requieren que se cree primero un objeto de dicha clase

	//Asegurate de que el modulo de juego este activo
	App->pieces->Enable();
	App->puntuation->Enable();
	App->win_lose->Enable();
	//App->lose_screen->Enable();

	return ret;
}

bool SceneLevel1::CleanUp()
{
	App->textures->Unload(spritesTexture);

	App->pieces->Disable();
	App->puntuation->Disable();
	App->win_lose->Disable();
	/*App->intro->Disable();*/
	//App->intro2->Disable();
	//App->lose_screen->Disable();

	return false;
}

Update_Status SceneLevel1::Update()
{
	if (App->win_lose->gameFinish)
	{
		App->fade->FadeToBlack(this, (Module*)App->intro, 90);
	}
	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}