#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 18

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ModulePlayer;
class ModulePresentation;
class SceneLevel1;
class ModuleParticles;
class ModuleCollisions;
class ModuleFadeToBlack;
class ModuleFonts;
class ModuleRender;
class ModulePuzzlePiecesV3;
class Puntuation;
class Intro;
class IntroJuego;
class WinLose;
class Continue;
class ModuleLose;

class Application
{

public:

	//Constructor. Creates all necessary modules for the application
	Application();

	//Destructor. Removes all module objects
	~Application();

	//Initializes all modules
	bool Init();

	//Updates all modules (PreUpdate, Update and PostUpdate)
	Update_Status Update();

	//Releases all the application data
	bool CleanUp();

public:
	// An array to store all modules
	Module* modules[NUM_MODULES];

	// All the modules stored individually
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;

	ModulePlayer* player = nullptr;
	ModulePresentation* presentation = nullptr;
	Intro* intro = nullptr;
	IntroJuego* introJuego = nullptr;

	SceneLevel1* sceneLevel_1 = nullptr;

	ModuleParticles* particles = nullptr;

	ModuleCollisions* collisions = nullptr;
	ModuleFadeToBlack* fade = nullptr;
	ModuleFonts* fonts = nullptr;

	ModuleRender* render = nullptr;

	ModulePuzzlePiecesV3* pieces = nullptr;
	Puntuation* puntuation = nullptr;
	WinLose* win_lose = nullptr;
	Continue* module_continue = nullptr;
	ModuleLose* lose_screen = nullptr;

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__