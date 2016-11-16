#pragma once

#include <stdbool.h>

typedef struct InputStatePrivate InputStatePrivate;

typedef struct InputState {
	bool lights, speakers, autopilot, kill;
	float throttle, steering;
	InputStatePrivate *private;
} InputState;

InputState* GamepadGlue_new( void );
bool GamepadGlue_detect( void );
void GamepadGlue_run( void );
