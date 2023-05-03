#include "ModuleWinLose.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Globals.h"

WinLose::WinLose(bool startEnabled) : Module(startEnabled)
{

}

WinLose::~WinLose()
{

}

// Load assets
bool WinLose::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	WinTexture = App->textures->Load("Assets/Sprites/win_lose.png");
	LoseTexture = App->textures->Load("Assets/Sprites/lose_win.png");


	return ret;

}

Update_Status WinLose::Update()
{
	if (gameFinish && App->puntuation->score < 1000)
	{
		gameFinish = false;
		ActiveTexture = LoseTexture;
		App->audio->PlayMusic("Assets/Music/Lose.ogg", 1.0f);
	}
	if (gameFinish && App->puntuation->score >= 1000)
	{
		gameFinish = false;
		ActiveTexture = WinTexture;
		App->audio->PlayMusic("Assets/Music/Win.ogg", 1.0f);
	}
	if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN)
	{
		gameFinish = true;

	}
	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background  
Update_Status WinLose::PostUpdate()
{
	if (ActiveTexture != nullptr)
	{
		App->render->Blit(ActiveTexture, 0, 0, NULL, false);
	}

	return Update_Status::UPDATE_CONTINUE;
}
bool WinLose::CleanUp()
{
	if (WinTexture != nullptr)
	{
		SDL_DestroyTexture(WinTexture);
		WinTexture = nullptr;
	}
	if (LoseTexture != nullptr)
	{
		SDL_DestroyTexture(LoseTexture);
		LoseTexture = nullptr;
	}
	if (ActiveTexture != nullptr)
	{
		ActiveTexture = nullptr;
	}
	return true;
}