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
{}

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
	GamePad& pad = App->input->pads[0];
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN || pad.a)
	{
		App->fade->FadeToBlack((Module*)App->lose_screen, (Module*)App->intro, 90);
	}
	return Update_Status::UPDATE_CONTINUE;
}
Update_Status ModuleLose::PostUpdate()
{
	App->render->Blit(LoseTexture, 0, 0, NULL);
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