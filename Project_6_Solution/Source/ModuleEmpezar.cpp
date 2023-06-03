#pragma once
#include "ModuleEmpezar.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "Puntuation.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Globals.h"
#include "ModuleFadeToBlack.h"
#include <iostream>
#include "ModulePuzzlePiecesV3.h"
#include "ModuleScene.h"

using namespace std;


ModuleEmpezar::ModuleEmpezar(bool startEnabled) : Module(startEnabled)
{
    for (int fila = 3; fila <= 15; fila++) {
        for (int columna = 0; columna <= 10; columna++) {
            int frameX = columna * SCREEN_WIDTH;
            int frameY = fila * SCREEN_HEIGHT;
            ReadyAnim.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
        }
    }   
    ReadyAnim.speed = 0.16f;
    ReadyAnim.loop = false;
    ReadyPath.PushBack({ 0.0f, 0.0f }, 200, &ReadyAnim);

}

ModuleEmpezar::~ModuleEmpezar()
{

}

// Load assets
bool ModuleEmpezar::Start()
{
    App->sceneLevel_1->Enable();
    App->pieces->Disable();
    LOG("Loading background assets");

    bool ret = true;

    ReadyTexture = App->textures->Load("Assets/Sprites/ReadyGo.png");
    //ReadyFX = App->audio->LoadFx("Assets/SFX/continue.wav");
    App->audio->PlayMusic(NULL, NULL);
    App->render->camera.x = 0;
    App->render->camera.y = 0;
    
    return ret;

}

Update_Status ModuleEmpezar::Update()
{
    ReadyAnim.Update();
    ReadyPath.Update();
   if (ReadyAnim.HasFinished()) 
   {
       App->pieces->Enable();
       App->fade->FadeToBlack((Module*)App->moduleEmpezar, (Module*)App->sceneLevel_1, 0);
       App->audio->PlayMusic("Assets/stage1.ogg");
   }
    return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleEmpezar::PostUpdate()
{
    if (ReadyTexture != nullptr)
    {
        App->render->Blit(ReadyTexture, 0, 0, &(ReadyPath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);
        ReadyPath.GetCurrentAnimation()->Update();
    }
    return Update_Status::UPDATE_CONTINUE;
}
bool ModuleEmpezar::CleanUp()
{
    if (ReadyTexture != nullptr)
    {
        App->textures->Unload(ReadyTexture);
        ReadyTexture = nullptr;
        ReadyAnim.Reset();
        ReadyPath.Reset();
    }

    return true;
}



