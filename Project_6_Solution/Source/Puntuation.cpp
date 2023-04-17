#include "Puntuation.h"


Puntuacion::Puntuacion()
{
    // Inicializar la puntuaci�n en cero
    puntuacion = 0;
  

    // Carga de la imagen de puntuaci�n
    // SDL_Surface* surface = 
   /* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);*/

    // Establecer la posici�n y dimensiones del rect�ngulo de la imagen de la puntuaci�n
    rect.x = 10;
    rect.y = 10;
    rect.w = 50;    // Ancho de la imagen
    rect.h = 50;    // Alto de la imagen
}

void Puntuacion::addPuntuacion(int puntuacion)
{
    // Incrementar la puntuaci�n actual por la cantidad proporcionada
    this->puntuacion += puntuacion;
}

void Puntuacion::draw(SDL_Renderer* renderer)
{
    // Dibujar la imagen de la puntuaci�n y el valor de la puntuaci�n actual
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_Color color = { 255, 255, 255 }; // Color blanco para el texto
   
   /* SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);*/

    SDL_Rect text_rect;
    text_rect.x = rect.x + rect.w + 10; // Posici�n x del texto a la derecha de la imagen de la puntuaci�n
    text_rect.y = rect.y; // Posici�n y del texto alineada con la imagen de la puntuaci�n
   // text_rect.w = surface->w; // Ancho del texto
  //  text_rect.h = surface->h; // Alto del texto

    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_DestroyTexture(texture);

}