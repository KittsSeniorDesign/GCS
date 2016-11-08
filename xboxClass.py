#!/usr/bin/env python
import evdev
import serial
from ControllerBaseClass import ControllerBaseClass

class XBOXController(ControllerBaseClass):
	"""docstring for XBOXController"""
     # maxForward - 127 = 127 - maxBackward
    maxForward = 220
    maxBackward = 34
    # maxSm = 255 - maxForward
    maxSm = 35
    # maxSp = maxForward - 127
    maxSp = 93
    lx = 0.0
    ly = 0.0
    rx = 0.0
    ry = 0.0
    leftMotor = 127
    rightMotor = 127
    deadzone = 2300
    lights = False
    speakers = False
    autoMode = True
    throttle = 0
    steering = 0
    kill = False

    leftYValue = 0
    leftXValue = 0

    rightYValue = 0
    rightXValue = 0

    #TY
    BUTTON = 1
    AXIS = 2

    #Numbers Buttons
    A_BUTTON = 0
    B_BUTTON = 1
    X_BUTTON = 2
    Y_BUTTON = 3
    LEFT_BUTTON = 4
    RIGHT_BUTTON = 5
    BACK_BUTTON = 6
    START_BUTTON = 7
    GUIDE_BUTTON = 8
    LEFT_STICK_BUTTON = 9
    RIGHT_STICK_BUTTON = 10

    #Number Axis
    LEFT_X_AXIS = 0
    LEFT_Y_AXIS = 1
    LEFT_TRIGGER = 2

    RIGHT_X_AXIS = 3
    RIGHT_Y_AXIS = 4
    RIGHT_TRIGGER = 5

    D_PAD_X = 6
    D_PAD_Y = 7

	def __init__(self, arg):
		super(XBOXController, self).__init__()
		self.connection = self.establishConnection
		
	def establishConnection():
		  connected = False
        while not connected:
            try:
                self.jsdev = open('/dev/input/js0', 'rb')
                connected = True
            except Exception:
                print('No controller detected, please plug one in.')
                time.sleep(1)
                pass
                
    def pollController():
            while True:
            try:
                evbuf = self.jsdev.read(8)
                if evbuf:
                    time, value, ty, number = struct.unpack('IhBB', evbuf)
                    
            except Exception as msg:
                print(msg)