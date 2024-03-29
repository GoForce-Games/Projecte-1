#include "ModuleWinLose.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "Puntuation.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Globals.h"
#include "ModuleEmpezar.h"
#include "ModuleFadeToBlack.h"
#include "ModulePuzzlePiecesV3.h"
#include "PuzzlePiece.h"


#include <iostream>

using namespace std;


WinLose::WinLose(bool startEnabled) : Module(startEnabled)
{
	for (int fila = 0; fila <= 9; fila++) {
		for (int columna = 0; columna <= 3; columna++) {
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			WinAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}
	WinAnimation.speed = 0.25f;
	WAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &WinAnimation);
	
	for (int fila = 0; fila <= 6; fila++) {
		for (int columna = 0; columna <= 3; columna++) 
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			LoseAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}
	LoseAnimation.speed = 0.2f;
	LAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &LoseAnimation);

}

WinLose::~WinLose()
{

}

// Load assets
bool WinLose::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	WinTexture = App->textures->Load("Assets/Sprites/SpritesWin.png");
	LoseTexture = App->textures->Load("Assets/Sprites/SpritesLose.png");

	LoseFX = App->audio->LoadFx("Assets/SFX/Lose.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;

}

Update_Status WinLose::Update()
{
	AAnimationPath.Update();
	
	

	if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN)
	{
		gameFinish = true;
	}
	if (gameFinish == true)
	{
		App->pieces->Disable();
		
		
	}
	if ((gameFinish && App->puntuation->score < 1000) || App->pieces->playArea.state == PlayAreaState::GAME_END)
	{
		ActiveTexture = LoseTexture;
		AAnimationPath = LAnimationPath;
		gameFinish = false;
		App->audio->PlayFx(LoseFX);
		App->audio->PlayMusic(NULL, NULL);
		App->moduleEmpezar->Disable();
		App->fade->FadeToBlack((Module*)App->win_lose, (Module*)App->module_continue, 200);
		
	}
	if (gameFinish && App->puntuation->score >= 1000)
	{
		ActiveTexture = WinTexture;
		AAnimationPath = WAnimationPath;
		gameFinish = false;
		App->audio->PlayMusic("Assets/Music/Win.ogg", 1.0f);
		App->puntuation->Disable();
		App->moduleEmpezar->Disable();
		App->fade->FadeToBlack((Module*)App->win_lose, (Module*)App->introJuego, 400);

		
	}
	
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status WinLose::PostUpdate()
{
	if (ActiveTexture != nullptr)
	{
		App->render->Blit(ActiveTexture, 0, 0, &(AAnimationPath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);
		AAnimationPath.GetCurrentAnimation()->Update();
	}

	return Update_Status::UPDATE_CONTINUE;
}
bool WinLose::CleanUp()
{
	if (WinTexture != nullptr)
	{
		App->textures->Unload(WinTexture);
		WinTexture = nullptr;
	}
	if (LoseTexture != nullptr)
	{
		App->textures->Unload(LoseTexture);
		LoseTexture = nullptr;
	}
	if (ActiveTexture != nullptr)
	{
		App->textures->Unload(ActiveTexture);
		ActiveTexture = nullptr;
	}
	return true;
}