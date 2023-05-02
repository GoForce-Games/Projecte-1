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
	return ret;
}
Update_Status ModulePresentation::Update() {
	if (App->input->keys[SDL_Scancode::SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_1, 90);
	}
	return Update_Status::UPDATE_CONTINUE;
}
Update_Status ModulePresentation::PostUpdate()
{
	
	App->render->Blit(bgTexture, 0, 0, NULL); // TODO cargar el fondo
	App->fonts->BlitText(0, 10, textFont, "Produced by GoForce Game");
	App->fonts->BlitText(0, 10, textFont, "Members");
	App->fonts->BlitText(0, 10, textFont, "Rogue");
	
	return Update_Status::UPDATE_CONTINUE;
}
bool ModulePresentation::CleanUp() {
	
	return true;
}




