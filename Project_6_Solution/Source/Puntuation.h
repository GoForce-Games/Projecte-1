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
	Puntuation(bool startEnabled);    // default constructor declaration
	~Puntuation();   // destructor declaration
	
	bool Init() override;
	bool Start() override;
	Update_Status Update() override;
	Update_Status PostUpdate() override;

	int score = 000;
	int numeracion = 0;
	int scoreFont = -1;
	int textFont = -1;
	char scoreText[MAX_SCORE_LENGTH + sizeof(char)];
	
  
};

#endif //__PUNTUATION_H__