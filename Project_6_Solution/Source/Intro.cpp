#include "Intro.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include <SDL/include/SDL_scancode.h>
#include <iostream>
using namespace std;

Intro::Intro(bool startEnabled) : Module(startEnabled)
{}

Intro::~Intro()
{}

bool Intro::Start()
{
	LOG("Loading intro assets");

	bool ret = true;

	IntroBackground = App->textures->Load("Assets/Sprites/IntroBackground.png");
	IntroTexture = App->textures->Load("Assets/Sprites/IntroScreen.png");
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status Intro::Update()
{

	App->fade->FadeToBlack((Module*)this, (Module*)App->intro2, 0);

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Intro::PostUpdate()
{
	App->render->Blit(IntroBackground, 0, 0, NULL);
	App->render->Blit(IntroTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}

