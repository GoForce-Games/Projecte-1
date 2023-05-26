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
	for (int fila = 0; fila <= 1; fila++) {
		for (int columna = 0; columna <= 10; columna++) {
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			ContinueAnim.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}
	ContinueAnim.speed = 0.02f;
	ContinueAnim.loop = false;
	ContinuePath.PushBack({ 0.0f, 0.0f }, 200, &ContinueAnim);
	
}

Continue::~Continue()
{

}

// Load assets
bool Continue::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	ContinueTexture = App->textures->Load("Assets/Sprites/continueall.png");
	

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;

}

Update_Status Continue::Update()
{
	ContinueAnim.Update();
	if (ContinueAnim.HasFinished())
	{
		App->fade->FadeToBlack((Module*)App->module_continue, (Module*)App->lose_screen, 0);

	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Continue::PostUpdate()
{
	if (ContinueTexture != nullptr)
	{
		App->render->Blit(ContinueTexture, 0, 0, &(ContinuePath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);
		ContinuePath.GetCurrentAnimation()->Update();
	}
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