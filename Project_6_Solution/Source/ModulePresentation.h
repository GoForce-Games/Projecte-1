#ifndef __MODULE_PRESENTATION_H__
#define __MODULE_PRESENTATION_H__
#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModulePresentation : public Module
{
public:
	
	ModulePresentation(bool startEnabled);

	
	~ModulePresentation();

	bool Start() override;

	Update_Status Update() override;

	
	Update_Status PostUpdate() override;

	bool CleanUp();
		
	int *textFont = nullptr;

public:
	
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* PresentationTexture = nullptr;
};

#endif