#include "ModuleScene.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModulePuzzlePiecesV3.h"
#include "Puntuation.h"
#include "ModuleWinLose.h"
#include "ModuleFadeToBlack.h"
#include "Intro.h"
#include "ModuleLose.h"
#include "ModuleContinue.h"
#include "ModuleEmpezar.h"

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

	bgTexture = App->textures->Load("Assets/Sprites/BattleArena2.png");
	background = App->textures->Load("Assets/Sprites/BattleArenaBackground.png");
	background2 = App->textures->Load("Assets/Sprites/BattleArenaBackground.png"); // Roger: Por que cargar el mismo archivo dos veces?
	App->audio->PlayMusic("Assets/stage1.ogg", 1.0f);
	backgroundRect = { 0,16,128,192 };
	App->puntuation->score = 0;
	//background2.PushBack({ 176,16,32,32 });
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
	App->moduleEmpezar->Disable();
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
	App->render->Blit(background, 16, 16, &backgroundRect);
	App->render->Blit(bgTexture, 0, 0, NULL);
	return Update_Status::UPDATE_CONTINUE;
}