#include "Puntuation.h"
#include "Application.h"
#include "ModuleFonts.h"
#include <stdio.h>
#include "..//External_Libraries/SDL_image/include/SDL_image.h"
#include <sstream>
#include <string.h>
using namespace std;

Puntuation::Puntuation(bool startEnabled) : Module(startEnabled)
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
	sprintf_s(scoreText, MAX_SCORE_LENGTH, "%7d", score);
	App->fonts->BlitText(0, 16, textFont, scoreText);
	return Update_Status::UPDATE_CONTINUE;
}


