#ifndef __PUNTUATION_H__
#define __PUNTUATION_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#include "ModuleInput.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")
#define MAX_SCORE_LENGTH 10

class Puntuation : public Module
{
public:
	// Constructor
	Puntuation(bool startEnabled);    

	// Destructor
	~Puntuation();   
	
	bool Init() override;

	bool Start() override;

	Update_Status Update() override;

	Update_Status PostUpdate() override;

	bool CleanUp() override;



	int score;
	int numeracion;
	int scoreFont;
	int textFont;
	char* scoreText = nullptr;
	
  
};

#endif //__PUNTUATION_H__