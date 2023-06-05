#pragma once
#include "ModuleInput.h"

#include "../External_Libraries/SDL/include/SDL.h"

enum KeyboardSetup {
	FastFall = SDL_Scancode::SDL_SCANCODE_S,
	MoveLeft = SDL_Scancode::SDL_SCANCODE_A,
	MoveRight = SDL_Scancode::SDL_SCANCODE_D,
	RotatePiece = SDL_Scancode::SDL_SCANCODE_P,
	Pause = SDL_Scancode::SDL_SCANCODE_I
};




class PlayerInput
{
	Key_State fastFall;
	Key_State moveLeft;
	Key_State moveRight;
	Key_State rotatePiece;
	Key_State pause;

	void StoreInput(const Key_State keyboard[MAX_KEYS], const GamePad& gamepad);

};

