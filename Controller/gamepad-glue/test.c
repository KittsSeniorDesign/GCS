#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "gamepad-backend/Gamepad.h"

int main( int argc, char **argv ) {
	Gamepad_init();
	while ( Gamepad_numDevices() < 1 ) {
		Gamepad_detectDevices();
		usleep(500000);
	}
	puts("gamepad attached");
	struct Gamepad_device *joystick = Gamepad_deviceAtIndex(0);
	printf("From program %p\n", joystick);
	int j = 0;
	while( true ) {
		printf("%d\n", j++);
		Gamepad_processEvents();
		for (int i = 0; i < joystick->numAxes; i++) {
			printf("axis %d: %g\n", i, joystick->axisStates[i]);
		}
		for (int i = 0; i< joystick->numButtons; i++ ) {
			printf("button %d: %d\n", i, joystick->buttonStates[i]);
		}
		usleep(100000);
	}

	return 0;
}
