#include "ModuleWinLose.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "Puntuation.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Globals.h"
#include "ModuleFadeToBlack.h"

WinLose::WinLose(bool startEnabled) : Module(startEnabled)
{
	idleAnimation.PushBack({ 399, 98, 213, 57 });

	WinAnimation.PushBack({ 399, 98, 213, 57 });
	WinAnimation.PushBack({ 400, 260, 211, 69 });
	WinAnimation.PushBack({ 400, 424, 217, 65 });
	WinAnimation.PushBack({ 399, 98, 213, 57 });
	WinAnimation.PushBack({ 400, 260, 211, 69 });
	WinAnimation.PushBack({ 400, 424, 217, 65 });
	WinAnimation.speed = 0.02f;
	
	LoseAnimation.PushBack({ 50, 98, 207, 57 });
	LoseAnimation.PushBack({ 51, 270, 212, 63 });
	LoseAnimation.PushBack({ 51, 425, 211, 58 });
	LoseAnimation.PushBack({ 50, 98, 207, 57 });
	LoseAnimation.PushBack({ 51, 270, 212, 63 });
	LoseAnimation.PushBack({ 51, 425, 211, 58 });
	LoseAnimation.speed = 0.01f;
}

WinLose::~WinLose()
{

}

// Load assets
bool WinLose::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	WinLoseTexture = App->textures->Load("Assets/Sprites/SpritesWinLose.png");
	//WinTexture = App->textures->Load("Assets/Sprites/win_lose.png");
	//WinTexture2 = App->textures->Load("Assets/Sprites/win_lose2.png");
	//WinTexture3 = App->textures->Load("Assets/Sprites/win_lose3.png");
	//LoseTexture = App->textures->Load("Assets/Sprites/lose_win.png");
	//LoseTexture2 = App->textures->Load("Assets/Sprites/lose_win2.png");
	//LoseTexture3 = App->textures->Load("Assets/Sprites/lose_win3.png");

	currentAnimation = &idleAnimation;

	return ret;

}

Update_Status WinLose::Update()
{

	if (gameFinish && App->puntuation->score < 1000)
	{
		gameFinish = false;
		currentAnimation = &LoseAnimation;
		App->audio->PlayMusic("Assets/Music/Lose.ogg", 1.0f);
		App->fade->FadeToBlack(this, (Module*)App->lose_screen ,90);
	}
	if (gameFinish && App->puntuation->score >= 1000)
	{
		gameFinish = false;
		currentAnimation = &WinAnimation;
		App->audio->PlayMusic("Assets/Music/Win.ogg", 1.0f);
	}
	if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN)
	{
		gameFinish = true;

	}

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background  
Update_Status WinLose::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(WinLoseTexture, 152, 112, NULL, false);
	SDL_Delay(10);

	return Update_Status::UPDATE_CONTINUE;
}
bool WinLose::CleanUp()
{
	if (WinLoseTexture != nullptr)
	{
		SDL_DestroyTexture(WinLoseTexture);
		WinLoseTexture = nullptr;
	}

	/*if (LoseTexture != nullptr)
	{
		SDL_DestroyTexture(LoseTexture);
		LoseTexture = nullptr;
	}*/
	/*if (ActiveTexture != nullptr)
	{
		ActiveTexture = nullptr;
	}*/
	return true;
}