#include "Puntuation.h"
#include "Application.h"
#include "ModuleFonts.h"
#include <stdio.h>
#include "..//External_Libraries/SDL_image/include/SDL_image.h"
#include <sstream>
#include <string.h>
#include "ModuleFonts.h"
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

	scoreText = new char[MAX_SCORE_LENGTH];

	score = 000;
	numeracion = 0;
	scoreFont = &(App->fonts->scoreFont);
	textFont = &(App->fonts->textFont);
	
	


	return ret;

}

Update_Status Puntuation::Update()
{
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Puntuation::PostUpdate()
{
	sprintf_s(scoreText, MAX_SCORE_LENGTH, "%7d", score);
	App->fonts->BlitText(0, 16, *textFont, scoreText);
	return Update_Status::UPDATE_CONTINUE;
}

bool Puntuation::CleanUp()
{
	if (scoreText != nullptr) {
		delete scoreText;
		scoreText = nullptr;
	}
	return true;
}


