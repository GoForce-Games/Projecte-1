#ifndef __PUNTUATION_H__
#define __PUNTUATION_H__

#include "Module.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")
class Puntuacion
{
public:
	Puntuacion();    // default constructor declaration
	~Puntuacion();   // destructor declaration
	int numeration[9];
    void addPuntuacion(int puntuacion);   // Funci�n para agregar puntuaci�n
    void draw(SDL_Renderer* renderer); // Funci�n para dibujar la puntuaci�n en la ventana
private:
    int puntuacion; // Variable para almacenar la puntuaci�n actual
    SDL_Texture* texture;    // Textura para la imagen de la puntuaci�n
    SDL_Rect rect;  // Rect�ngulo para la imagen de la puntuaci�n
};

#endif //__PUNTUATION_H__