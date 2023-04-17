#include "Puntuation.h"


Puntuacion::Puntuacion()
{
    // Inicializar la puntuación en cero
    puntuacion = 0;
  

    // Carga de la imagen de puntuación
    // SDL_Surface* surface = 
   /* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);*/

    // Establecer la posición y dimensiones del rectángulo de la imagen de la puntuación
    rect.x = 10;
    rect.y = 10;
    rect.w = 50;    // Ancho de la imagen
    rect.h = 50;    // Alto de la imagen
}

void Puntuacion::addPuntuacion(int puntuacion)
{
    // Incrementar la puntuación actual por la cantidad proporcionada
    this->puntuacion += puntuacion;
}

void Puntuacion::draw(SDL_Renderer* renderer)
{
    // Dibujar la imagen de la puntuación y el valor de la puntuación actual
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_Color color = { 255, 255, 255 }; // Color blanco para el texto
   
   /* SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);*/

    SDL_Rect text_rect;
    text_rect.x = rect.x + rect.w + 10; // Posición x del texto a la derecha de la imagen de la puntuación
    text_rect.y = rect.y; // Posición y del texto alineada con la imagen de la puntuación
   // text_rect.w = surface->w; // Ancho del texto
  //  text_rect.h = surface->h; // Alto del texto

    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_DestroyTexture(texture);

}