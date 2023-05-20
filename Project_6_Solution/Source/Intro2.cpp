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

	IntroTexture2 = App->textures->Load("Assets/Sprites/IntroBackground.png");
	App->audio->PlayMusic("Assets/Music/Title.ogg", 1.0f);
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	titleFX = App->audio->LoadFx("Assets/SFX/titlescreen.wav");

	return ret;
}

Update_Status Intro2::Update()
{
	GamePad& pad = App->input->pads[0];
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN || pad.a)
	{
		App->audio->PlayFx(titleFX);
		App->fade->FadeToBlack((Module*)this, (Module*)App->sceneLevel_1, 90);
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Intro2::PostUpdate()
{
	App->render->Blit(IntroTexture2, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}