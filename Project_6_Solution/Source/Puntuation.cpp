#include "Puntuation.h"
#include "Application.h"
#include "ModuleFonts.h"
#include <stdio.h>

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

	char lookupTable[] = { " !\"#$%&\'()*+,-./0123456789:;<=>?çabcdefghijklmnopqrstuvwxyz´`¨_|" };
	char specialNumbers[] = { "0123456789:PT+-" };
	textFont = App->fonts->Load("Assets/Fonts/fonts2.png", lookupTable, 4);
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
	App->fonts->BlitText(14, 22, textFont, scoreText);
	App->fonts->BlitText(14, 22, textFont, "font test");
	return Update_Status::UPDATE_CONTINUE;
}


