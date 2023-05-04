#ifndef __INTRO_H__
#define __INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Intro : public Module
{
public:
	Intro(bool startEnabled);
	~Intro();

	bool Start() override;

	Update_Status Update() override;
	Update_Status PostUpdate() override;

	SDL_Texture* IntroTexture = nullptr;
};

#endif
