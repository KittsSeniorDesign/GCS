#include "gamepad-glue.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "gamepad-backend/Gamepad.h"

struct InputStatePrivate {
	uint8_t buttonMap[64];
	uint8_t axisMap[64];
	Gamepad *device;
};

enum GamepadGlue_MappedAxes {
	AXIS_UNMAPPED = 0,
	AXIS_THROTTLE,
	AXIS_STEERING,
};

enum GamepadGlue_MappedButtons {
	BUTTON_UNMAPPED = 0,
	BUTTON_LIGHT, // A
	BUTTON_SPEAKER,   // B
	BUTTON_AUTOPILOT, // Y
	BUTTON_KILL,      // XBOX button
};

static bool GamepadGlue_deviceSupported( Gamepad *device );
static void GamepadGlue_setButtonMap( InputState *self );
static void GamepadGlue_deviceAttached( Gamepad *device, void *context );
static void GamepadGlue_deviceRemoved( Gamepad *device, void *context );
static void GamepadGlue_buttonUp( Gamepad *device, unsigned int buttonID,
                                  double timestamp, void *context );
static void GamepadGlue_axisMoved( Gamepad *device, unsigned int axisID,
                                   float value, float lastValue,
                                   double timestamp, void *context );

InputState *GamepadGlue_new( void ) {
	InputState *self = malloc( sizeof(*self) );
	memset( self, 0, sizeof(*self) );
	self->private = malloc( sizeof(*self->private) );
	memset( self->private, 0, sizeof(*self->private) );
	Gamepad_deviceAttachFunc( GamepadGlue_deviceAttached, self );
	Gamepad_deviceRemoveFunc( GamepadGlue_deviceRemoved, self );
	// Gamepad_buttonDownFunc( GamepadGlue_buttonDown, self );
	Gamepad_buttonUpFunc( GamepadGlue_buttonUp, self );
	Gamepad_axisMoveFunc( GamepadGlue_axisMoved, self );
	Gamepad_init( );
	return self;
}

void GamepadGlue_free( InputState *self ) {
	free( self->private );
	free( self );
}

bool GamepadGlue_detect( void ) {
	Gamepad_detectDevices( );
	for (int i = 0; i < Gamepad_numDevices( ); i++) {
		if ( GamepadGlue_deviceSupported( Gamepad_deviceAtIndex( i ) ) ) {
			return true;
		}
	}
	return false;
}

void GamepadGlue_run( void ) {
	Gamepad_processEvents( );
}

static void GamepadGlue_deviceAttached( Gamepad *device, void *context ) {
	InputState *self = context;
	if ( self->private->device != NULL || !GamepadGlue_deviceSupported( device ) ) {
		return;
	}
	self->private->device = device;
	GamepadGlue_setButtonMap( self );
}

static bool GamepadGlue_deviceSupported( Gamepad *device ) {
	switch ( (device->vendorID << 16) | device->productID ) {
#if defined(__APPLE__) || defined(__linux__)
		case 0x045E028E: // Xbox 360 wired
		case 0x045E028F: // Xbox 360 wireless
		case 0x045E02D1: // Xbox 1
		case 0x054C05C4: // Dualshock 4
			return true;
#endif
	}
	return false;
}

// A somewhat useful URL for vid/pids: http://www.linux-usb.org/usb.ids
static void GamepadGlue_setButtonMap( InputState *self ) {
	Gamepad *device = self->private->device;
	uint8_t *buttonMap = self->private->buttonMap,
	        *axisMap = self->private->axisMap;

	printf( "%s connected.\n", device->description );
	switch ( (device->vendorID << 16) | device->productID ) {
		// 0x045E is Microsoft
		// case 0x045E0202: // Xbox
		case 0x045E028E: // Xbox 360 wired
		case 0x045E028F: // Xbox 360 wireless
		case 0x045E02D1: // Xbox One
#if defined(__APPLE__)
			buttonMap[16] = BUTTON_LIGHT;
			buttonMap[17] = BUTTON_SPEAKER;
			buttonMap[19] = BUTTON_AUTOPILOT;
			buttonMap[15] = BUTTON_KILL;
			axisMap[1] = AXIS_THROTTLE;
			axisMap[2] = AXIS_STEERING;
#elif defined(__linux__)
			buttonMap[0] = BUTTON_LIGHT;
			buttonMap[1] = BUTTON_SPEAKER;
			buttonMap[3] = BUTTON_AUTOPILOT;
			buttonMap[8] = BUTTON_KILL;
			axisMap[1] = AXIS_THROTTLE;
			axisMap[3] = AXIS_STEERING;
#endif
			break;
		// 0x054C is Sony
		case 0x054C05C4: // Dualshock 4 (DS3 is bluetooth only afaik)
#if defined(__APPLE__) || defined(__linux__)
			buttonMap[1] = BUTTON_LIGHT;
			buttonMap[2] = BUTTON_SPEAKER;
			buttonMap[3] = BUTTON_AUTOPILOT;
			buttonMap[12] = BUTTON_KILL;
			axisMap[1] = AXIS_THROTTLE;
			axisMap[2] = AXIS_STEERING;
#endif
	}
}

static void GamepadGlue_deviceRemoved( Gamepad *device, void *context ) {
	InputState *self = context;
	self->throttle = 0;
	self->steering = 0;
	// Sets self->private->device pointer to NULL and resets joystick map.
	memset( self->private, 0, sizeof(*self->private) );
}

static void GamepadGlue_buttonUp( Gamepad *device, unsigned int buttonID,
                                  double timestamp, void *context ) {
	InputState *self = context;
	if ( self->private->device == NULL ) {
		return;
	}
	switch ( self->private->buttonMap[buttonID] ) {
	case BUTTON_KILL:
		self->kill = !self->kill;
		break;
	case BUTTON_LIGHT:
		self->lights = !self->lights;
		break;
	case BUTTON_SPEAKER:
		self->speakers = !self->speakers;
		break;
	case BUTTON_AUTOPILOT:
		self->autopilot = !self->autopilot;
	}
}

static void GamepadGlue_axisMoved( Gamepad *device, unsigned int axisID,
                                   float value, float lastValue,
                                   double timestamp, void *context ) {
	InputState *self = context;
	if ( self->private->device == NULL ) {
		return;
	}
	if ( fabsf(value) < 0.15f ) {
		value = 0.f;
	}
	switch ( self->private->axisMap[axisID] ) {
	case AXIS_STEERING:
		self->steering = value;
		break;
	case AXIS_THROTTLE:
		self->throttle = value;
	}
}
