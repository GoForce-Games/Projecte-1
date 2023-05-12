#include "ModulePresentation.h"
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


ModulePresentation::ModulePresentation(bool startEnabled) : Module(startEnabled)
{

}

ModulePresentation::~ModulePresentation()
{

}

bool ModulePresentation::Start() {
	LOG("Loading background assets");

	bool ret = true;
	textFont = App->puntuation->textFont;
	PresentationTexture = App->textures->Load("Assets/Sprites/presentation.png");

	return ret;
}
Update_Status ModulePresentation::Update() {

	if (App->input->keys[SDL_Scancode::SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->intro, 90);
	}
	return Update_Status::UPDATE_CONTINUE;
}
Update_Status ModulePresentation::PostUpdate()
{
	App->render->Blit(PresentationTexture, 0, 0, NULL);
	return Update_Status::UPDATE_CONTINUE;
}
bool ModulePresentation::CleanUp() {
	
	return true;
}




