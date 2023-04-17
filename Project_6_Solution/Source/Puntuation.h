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
    void addPuntuacion(int puntuacion);   // Función para agregar puntuación
    void draw(SDL_Renderer* renderer); // Función para dibujar la puntuación en la ventana
private:
    int puntuacion; // Variable para almacenar la puntuación actual
    SDL_Texture* texture;    // Textura para la imagen de la puntuación
    SDL_Rect rect;  // Rectángulo para la imagen de la puntuación
};

#endif //__PUNTUATION_H__