#ifndef __INTRO2_H__
#define __INTRO2_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Intro2 : public Module
{
public:
	Intro2(bool startEnabled);
	~Intro2();

	bool Start() override;

	Update_Status Update() override;
	Update_Status PostUpdate() override;

	SDL_Texture* IntroTexture2 = nullptr;
};

#endif
