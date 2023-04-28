#include "Puntuation.h"
#include "Application.h"
#include "ModuleFonts.h"

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
	App->fonts->BlitText(50, 50, textFont, "this is just a font test");

	return ret;
	
}

Update_Status Puntuation::Update()
{
	App->fonts->BlitText(50, 50, textFont, "this is just a font test");
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Puntuation::PostUpdate()
{
	return Update_Status::UPDATE_CONTINUE;
}


