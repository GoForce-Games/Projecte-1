#include "Puntuation.h"
#include "Application.h"
#include "ModuleFonts.h"
#include <stdio.h>
#include "..//External_Libraries/SDL_image/include/SDL_image.h"

Puntuation::Puntuation(bool startEnabled)
{
}

Puntuation::~Puntuation()
{
}

bool Puntuation::Init()
{
	return true;
}

bool Puntuation::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	char lookupTable[] = { " !\"#$%&\'()*+,-./0123456789:;<=>?çabcdefghijklmnopqrstuvwxyz´`¨_|º" };
	char specialNumbers[] = { "0123456789:PT+-" };
	textFont = App->fonts->Load("Assets/Fonts/fonts4.png", lookupTable, 4);
	scoreFont = App->fonts->Load("Assets/Fonts/fonts3.png", specialNumbers, 1);
	

	return ret;
	
}

Update_Status Puntuation::Update()
{
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Puntuation::PostUpdate()
{
	sprintf_s(scoreText, 10, "%7d", score);
	App->fonts->BlitText(25, 16, textFont, "0000");
	return Update_Status::UPDATE_CONTINUE;
}


