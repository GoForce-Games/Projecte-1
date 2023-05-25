#include "ModuleContinue.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "Puntuation.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Globals.h"
#include "ModuleFadeToBlack.h"
#include <iostream>

using namespace std;


Continue::Continue(bool startEnabled) : Module(startEnabled)
{
	
}

Continue::~Continue()
{

}

// Load assets
bool Continue::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	ContinueTexture = App->textures->Load("Assets/Sprites/continue.png");
	

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;

}

Update_Status Continue::Update()
{
	
	App->fade->FadeToBlack((Module*)this, (Module*)App->lose_screen, 90);
	
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Continue::PostUpdate()
{
	
		App->render->Blit(ContinueTexture, 0, 0, NULL);
	

	return Update_Status::UPDATE_CONTINUE;
}
bool Continue::CleanUp()
{
	
	if (ContinueTexture != nullptr)
	{
		SDL_DestroyTexture(ContinueTexture);
		ContinueTexture = nullptr;
	}
	return true;
}