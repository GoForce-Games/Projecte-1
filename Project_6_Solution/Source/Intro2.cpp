#include "Intro2.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include <SDL/include/SDL_scancode.h>
#include <iostream>
using namespace std;

Intro2::Intro2(bool startEnabled) : Module(startEnabled)
{}

Intro2::~Intro2()
{}

bool Intro2::Start()
{
	LOG("Loading intro assets");

	bool ret = true;

	IntroTexture2 = App->textures->Load("Assets/Sprites/IntroScreen2.png");
	App->audio->PlayMusic("Assets/Music/Title.ogg", 1.0f);
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status Intro2::Update()
{

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack((Module*)this, (Module*)App->sceneLevel_1, 0);
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Intro2::PostUpdate()
{
	App->render->Blit(IntroTexture2, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}