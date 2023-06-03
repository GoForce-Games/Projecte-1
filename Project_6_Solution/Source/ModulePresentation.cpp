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
	for (int fila = 0; fila < 61; fila++)
	{
		for (int columna = 0; columna < 5; columna++)
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			presentationAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}

	presentationAnimation.speed = 0.2f;
	presentationAnimation.loop = false;
	presentationPath.PushBack({ 0.0f, 0.0f }, 200, &presentationAnimation);
}

ModulePresentation::~ModulePresentation()
{

}

bool ModulePresentation::Start() {
	LOG("Loading background assets");

	bool ret = true;
	textFont = App->puntuation->textFont;
	PresentationTexture = App->textures->Load("Assets/Sprites/longintro.png");
	App->audio->PlayMusic("Assets/Music/Presentation.ogg", 1.0f);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}
Update_Status ModulePresentation::Update() {
	presentationAnimation.Update();

	GamePad& pad = App->input->pads[0];
	if (this->IsEnabled() && (App->input->control[0].rotatePiece == Key_State::KEY_DOWN || App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN))
	{
		App->fade->FadeToBlack(this, (Module*)App->intro, 90);
	}
	if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN || pad.y)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_1, 90);
	}
	return Update_Status::UPDATE_CONTINUE;
}
Update_Status ModulePresentation::PostUpdate()
{
	App->render->Blit(PresentationTexture, 0, 0, &(presentationPath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);
	return Update_Status::UPDATE_CONTINUE;
}
bool ModulePresentation::CleanUp() {
	if (PresentationTexture != nullptr)
	{
		SDL_DestroyTexture(PresentationTexture);
		PresentationTexture = nullptr;
	}
	
	return true;
}




