#include "ModuleWinLose.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Globals.h"

WinLose::WinLose(bool startEnabled) : Module(startEnabled)
{

}

WinLose::~WinLose()
{

}

// Load assets
bool WinLose::Start()
{
    LOG("Loading background assets");

    bool ret = true;

    WinTexture = App->textures->Load("Assets/Sprites/win_lose.png");

    LoseTexture = App->textures->Load("Assets/Sprites/lose_win.png");


    if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN)
    {
        App->render->Blit(LoseTexture, 0, 0, NULL);
    }
    return ret;

    if (App->puntuation->score == 1000)
    {
        App->render->Blit(WinTexture, 0, 0, NULL);
    }
}

Update_Status WinLose::Update()
{
    return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background  
Update_Status WinLose::PostUpdate()
{
    return Update_Status::UPDATE_CONTINUE;
}