#ifndef __MODULE_LOSE_H__
#define __MODULE_LOSE_H__
#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleLose : public Module
{
public:

	ModuleLose(bool startEnabled);


	~ModuleLose();

	bool Start() override;

	Update_Status Update() override;


	Update_Status PostUpdate() override;

	bool CleanUp();

	int* textFont = nullptr;

public:

	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* LoseTexture = nullptr;
};

#endif