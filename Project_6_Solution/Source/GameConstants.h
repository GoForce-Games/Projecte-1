#pragma once

#define MAX_PIECES 200

#define MAX_WALLS 6

#define MAX_DROP_DELAY 15
#define MIN_DROP_DELAY 2
#define MAX_MOVE_DELAY 5

#define MOVE_SPEED PIECE_SIZE
#define GRAVITY 4

#define EXPLODE_COUNTDOWN 100

enum PlayerCollisionCheck {
	CENTER,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	DEBUG
};