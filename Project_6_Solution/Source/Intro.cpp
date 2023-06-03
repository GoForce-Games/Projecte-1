#include "Intro.h"
#include "IntroJuego.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include <SDL/include/SDL_scancode.h>
#include "SDL/include/SDL.h"
#include <iostream>

using namespace std;

Intro::Intro(bool startEnabled) : Module(startEnabled)
{
	//bucle para recorrer la spritesheet de la animacion frame por frame
	for (int fila = 0; fila < 16; fila++) 
	{
		for (int columna = 0; columna < 5; columna++)
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			IntroAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}

	IntroAnimation.speed = 0.2f;
	IntroAnimation.loop = false;
	IAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &IntroAnimation);
}
 
Intro::~Intro()
{}

bool Intro::Start()
{
	LOG("Loading intro assets");

	bool ret = true;

	IntroTexture = App->textures->Load("Assets/Sprites/IntroAnimation.png");
	App->audio->PlayMusic("Assets/Music/Neo-Geo Opening Theme.ogg", 1.0f);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status Intro::Update()
{
	IntroAnimation.Update();

	GamePad& pad = App->input->pads[0];

	IAnimationPath.Update();

	if(IntroAnimation.HasFinished() || App->input->control[0].rotatePiece == Key_State::KEY_DOWN || App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)

	{
		App->fade->FadeToBlack((Module*)App->intro, (Module*)App->introJuego, 90);
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
	if (IntroTexture != nullptr)
	{
		SDL_DestroyTexture(IntroTexture);
		IntroTexture = nullptr;
		IAnimationPath.Reset();
		IntroAnimation.Reset();
	}

	return true;
}