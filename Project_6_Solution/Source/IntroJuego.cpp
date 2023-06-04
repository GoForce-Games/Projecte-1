#include "Intro.h"
#include "IntroJuego.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleEmpezar.h"
#include "ModuleFadeToBlack.h"
#include <SDL/include/SDL_scancode.h>
#include "SDL/include/SDL.h"
#include <iostream>

using namespace std;

IntroJuego::IntroJuego(bool startEnabled) : Module(startEnabled)
{
	//bucle para recorrer la spritesheet de la animacion frame por frame
	for (int fila = 17; fila < 56; fila++)
	{
		for (int columna = 0; columna < 5; columna++)
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			IntroJuegoAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}

	IntroJuegoAnimation.speed = 0.135f;
	IntroJuegoAnimation.loop = false;
	IAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &IntroJuegoAnimation);
}

IntroJuego::~IntroJuego()
{}

bool IntroJuego::Start()
{
	LOG("Loading intro assets");
	

	bool ret = true;

	IntroJuegoTexture = App->textures->Load("Assets/Sprites/IntroAnimation.png");
	App->audio->PlayMusic("Assets/Music/Title_2.ogg", 1.0f);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status IntroJuego::Update()
{
	IntroJuegoAnimation.Update();
	IAnimationPath.Update();


	if (IntroJuegoAnimation.HasFinished() || App->input->control[0].rotatePiece == Key_State::KEY_DOWN || App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->moduleEmpezar->Enable();
		App->moduleEmpezar->ReadyAnim.Reset();
		App->moduleEmpezar->ReadyPath.Reset();
		App->fade->FadeToBlack((Module*)App->introJuego, (Module*)App->moduleEmpezar, 0);
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status IntroJuego::PostUpdate()
{
	App->render->Blit(IntroJuegoTexture, 0, 0, &(IAnimationPath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);

	return Update_Status::UPDATE_CONTINUE;
}

bool IntroJuego::CleanUp()
{
	if (IntroJuegoTexture != nullptr)
	{
		SDL_DestroyTexture(IntroJuegoTexture);
		IntroJuegoTexture = nullptr;
	}

	return true;
}