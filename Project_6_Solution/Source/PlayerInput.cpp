#include "PlayerInput.h"

void PlayerInput::StoreInput(const Key_State keyboard[MAX_KEYS], const GamePad& gamepad)
{
	if (keyboard[KeyboardSetup::FastFall] != Key_State::KEY_IDLE)
		fastFall = keyboard[KeyboardSetup::FastFall];
	else {
		if (gamepad.a)
			fastFall = (fastFall == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			fastFall = (fastFall == KEY_REPEAT || fastFall == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}
}
