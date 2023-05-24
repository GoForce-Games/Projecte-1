#pragma once

#include "Module.h"
#include "Animation.h"
#include "Path.h"

struct SDL_Texture;

class IntroJuego : public Module
{
public:
	IntroJuego(bool startEnabled);
	~IntroJuego();

	bool Start() override;

	bool CleanUp();

	Update_Status Update() override;
	Update_Status PostUpdate() override;

	SDL_Texture* IntroJuegoTexture = nullptr;

	Animation IntroJuegoAnimation;

	Path IAnimationPath;
};