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

	//bgTexture = App->textures->Load("Assets/Sprites/sprites.png");
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz" };
	textFont = App->fonts->Load("Assets/Fonts/Sprites.png", "abcdefghijklmnopqrstuvwxyz", 1);
	
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
	
	App->render->Blit(bgTexture, 0, 0, NULL);
	App->fonts->BlitText(0, 0, textFont, "Produced by GoForce Game");
	App->fonts->BlitText(0, 0, textFont, "Members");
	App->fonts->BlitText(0, 0, textFont, "Rogue");
	return Update_Status::UPDATE_CONTINUE;
}
bool ModulePresentation::CleanUp() {
	App->player->Disable();
}




