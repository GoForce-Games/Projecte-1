#ifndef __PUNTUATION_H__
#define __PUNTUATION_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")
class Puntuation : public Module
{
public:
	Puntuation(bool startEnabled);    // default constructor declaration
	~Puntuation();   // destructor declaration
	
	bool Init() override;
	bool Start() override;
	Update_Status Update() override;
	Update_Status PostUpdate() override;

	uint score = 000;
	int scoreFont = -1;
	int textFont = -1;
	char scoreText[10] = { "font test" };
	void BlitText(int x, int y, int fontIndex, const char* text) const;
  
};

#endif //__PUNTUATION_H__