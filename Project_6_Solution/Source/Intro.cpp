#include "Intro.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include <SDL/include/SDL_scancode.h>

Intro::Intro(bool startEnabled) : Module(startEnabled)
{}

Intro::~Intro()
{}

bool Intro::Start()
{
	LOG("Loading intro assets");

	bool ret = true;

	IntroTexture = App->textures->Load("Assets/Sprites/IntroScreen.png");
	App->audio->PlayMusic("Assets/Music/Title.ogg", 1.0f);
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status Intro::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_1, 90);
	}
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Intro::PostUpdate()
{
	App->render->Blit(IntroTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}