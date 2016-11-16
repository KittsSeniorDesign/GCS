from __future__ import print_function
import cffi
import os, socket, sys, time

ffi = cffi.FFI()
ffi.cdef("""
    typedef struct InputStatePrivate InputStatePrivate;

    typedef struct InputState {
        bool lights, speakers, autopilot, kill;
        float throttle, steering;
        InputStatePrivate *private;
    } InputState;

    InputState* GamepadGlue_new( void );
    bool GamepadGlue_detect( void );
    void GamepadGlue_run( void );
""")

if sys.platform == 'darwin':
    libExtension = '.dylib'
elif sys.platform == 'win32':
    libExtension = '.dll'
else:
    libExtension = '.so'

scriptDir = os.path.dirname(os.path.realpath(__file__))
glue = ffi.dlopen('{}/gamepad-glue/build/libGamepadGlue{}'.format(scriptDir, libExtension))

def throttle_and_steering_to_differential(throttle, steering):
    thrustL    = thrustR    = 127
    rawThrustL = rawThrustR = 0
    if throttle < 0: # forward
        rawThrustR = rawThrustL = 69
        if steering < 0: # left
            rawThrustL += 69*steering # 127 + (69 + 69*(-)steering)*(-)thrust
            rawThrustR -= 58*steering
        else: # right
            rawThrustL += 58*steering
            rawThrustR -= 69*steering
    else: # backward
        rawThrustR = rawThrustL = 90
        if steering < 0: # left
            rawThrustL -= 38*steering # 127 + (90 + 90*(-)steering)*thrust
            rawThrustR += 90*steering
        else: # right
            rawThrustL -= 90*steering
            rawThrustR += 38*steering

    thrustL += int(round(rawThrustL*throttle))
    thrustR += int(round(rawThrustR*throttle))
    return (thrustL, thrustR)

class Joystick:
    HOST = '192.168.1.50'
    PORT = 12345

    def __init__(self):
        self.clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.inputState = glue.GamepadGlue_new()

    def open_joystick(self):
        if not bool(glue.GamepadGlue_detect()):
            print('Waiting for supported gamepad (Xbox 360/One, Dualshock 4).')
            while not bool(glue.GamepadGlue_detect()):
                time.sleep(0.5)

    def connect_to_server(self):
        while True:
            try:
                self.clientsocket.connect((self.HOST, self.PORT))
            except socket.error as err:
                print('Could not connect to joystick server. Is {}:{} on the network and listening?'.format(self.HOST, self.PORT))
                time.sleep(1)
            else:
                break

    def send_packet(self):
        (thrustL, thrustR) = throttle_and_steering_to_differential(
            self.inputState.throttle,
            self.inputState.steering
        )
        packet = '{:1}{:1}{:1}{:1}{}{}'.format(
            self.inputState.lights,
            self.inputState.speakers,
            self.inputState.autopilot,
            self.inputState.kill,
            chr(thrustR),
            chr(thrustL)
        )
        self.clientsocket.send(packet)

    def run(self):
        while True:
            glue.GamepadGlue_run()
            self.send_packet()
            time.sleep(0.1)

if __name__ == '__main__':
    try:
        js = Joystick()
        js.open_joystick()
        js.connect_to_server()
        js.run()
    except KeyboardInterrupt:
        sys.exit()
