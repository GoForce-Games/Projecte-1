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
{
	// iterate the animation, that have 5 frames per row and 189 rows
	for (int fila = 0; fila < 56; fila++) {
		for (int columna = 0; columna < 5; columna++) {
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			IntroAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}

	IntroAnimation.speed = 0.05f;

	IAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &IntroAnimation);
}
 
Intro::~Intro()
{}

bool Intro::Start()
{
	LOG("Loading intro assets");

	bool ret = true;

	IntroTexture = App->textures->Load("Assets/Sprites/IntroAnimation.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status Intro::Update()
{
	IntroAnimation.Update();
	IntroAnimation.Update();

	// TODO1: pasar al nivel1 cuando se pulse espacioo acabe animacion;
	
	if (this->IsEnabled() && App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack((Module*)App->sceneIntro, (Module*)App->sceneLevel_1, 90);
	}
	

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Intro::PostUpdate()
{
	App->render->Blit(IntroTexture, 0, 0, &(IAnimationPath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);

	return Update_Status::UPDATE_CONTINUE;
}

bool Intro::CleanUp()
{
	// TODO Remove All Memory Leaks
	App->sceneIntro->Disable();

	return true;
}