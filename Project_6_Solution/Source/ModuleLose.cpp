#include "ModuleLose.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleWindow.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "Puntuation.h"
#include "../External_Libraries/SDL/include/SDL.h"
#include "Module.h"


ModuleLose::ModuleLose(bool startEnabled) : Module(startEnabled)
{
	for (int fila = 0; fila < 24; fila++)
	{
		for (int columna = 0; columna < 5; columna++)
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			GameOverAnim.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}
	GameOverAnim.speed = 0.25f;
	GameOverPath.PushBack({ 0.0f, 0.0f }, 200, &GameOverAnim);
}

ModuleLose::~ModuleLose()
{}

bool ModuleLose::Start() {
	LOG("Loading gameover assets");

	bool ret = true;
	textFont = App->puntuation->textFont;
	LoseTexture = App->textures->Load("Assets/Sprites/gameover.png");
	App->audio->PlayMusic("Assets/Music/Game Over.ogg", 1.0f);
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	return true;
}
Update_Status ModuleLose::Update() 
{
	GameOverAnim.Update();
	GamePad& pad = App->input->pads[0];
	
	App->fade->FadeToBlack((Module*)App->lose_screen, (Module*)App->introJuego, 200);
	
	return Update_Status::UPDATE_CONTINUE;
}
Update_Status ModuleLose::PostUpdate()
{
	App->render->Blit(LoseTexture, 0, 0, &(GameOverPath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);
	return Update_Status::UPDATE_CONTINUE;
}
bool ModuleLose::CleanUp() 
{
	if (LoseTexture != nullptr)
	{
		SDL_DestroyTexture(LoseTexture);
		LoseTexture = nullptr;
	}

	return true;
}