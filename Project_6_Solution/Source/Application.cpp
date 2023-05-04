#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleScene.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModulePuzzlePiecesV2.h"
#include "Puntuation.h"
#include "ModulePresentation.h"
#include "Intro.h"
#include "Intro2.h"
#include "ModuleWinLose.h"

Application::Application()
{
	// The order in which the modules are added is very important.
	// It will define the order in which Pre/Update/Post will be called
	// Render should always be last, as our last action should be updating the screen
	int i = 0;
	modules[0] = window = new ModuleWindow(true);
	modules[1] = input = new ModuleInput(true);
	modules[2] = textures = new ModuleTextures(true);
	modules[3] = audio = new ModuleAudio(true);
	modules[4] = fonts = new ModuleFonts(true);
	modules[5] = presentation = new ModulePresentation(true);

	modules[6] = intro = new Intro(false);
	modules[7] = intro2 = new Intro2(false);
	//modules[i++] = sceneIntro = new SceneIntro(true); // Hay que aumentar el maximo de modulos para poner esto, ademas de "mover" los modulos de debajo un espacio mas adelante (sumar 1 a los indices)

	modules[8] = sceneLevel_1 = new SceneLevel1(false);
	modules[9] = puntuation = new Puntuation(false);
	modules[10] = win_lose = new WinLose(false);
	// AVISO: una vez se ponga la sceneIntro hay que poner un false en los parámetros de SceneLevel1() o sino va a cargar ambas escenas al mismo tiempo
	// Si esto peta al añadir un modulo quedais avisados
	modules[11] = pieces = new ModulePuzzlePiecesV2(false);
	modules[12] = particles = new ModuleParticles(true);

	modules[13] = collisions = new ModuleCollisions(true);
	modules[14] = fade = new ModuleFadeToBlack(true);

	modules[15] = render = new ModuleRender();
}

Application::~Application()
{
	for (int i = 0; i < NUM_MODULES; ++i)
	{

		//Important: when deleting a pointer, set it to nullptr afterwards
		//It allows us for null check in other parts of the code
		delete modules[i];
		modules[i] = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	for (int i = 0; i < NUM_MODULES && ret; ++i)
		if (modules[i]->IsEnabled())
			ret = modules[i]->Init();



	//By now we will consider that all modules are always active
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		if (modules[i]->IsEnabled())
			ret = modules[i]->Start();

	return ret;
}

Update_Status Application::Update()
{
	Update_Status ret = Update_Status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		if (modules[i]->IsEnabled())
			ret = modules[i]->PreUpdate();

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		if (modules[i]->IsEnabled())
			ret = modules[i]->Update();

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		if (modules[i]->IsEnabled())
			ret = modules[i]->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret; --i)
		if (modules[i]->IsEnabled())
			ret = modules[i]->CleanUp();

	return ret;
}
