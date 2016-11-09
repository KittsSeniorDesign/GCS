#!/usr/bin/env python
import evdev
import serial
from ControllerBaseClass import ControllerBaseClass

class XBOXController(ControllerBaseClass):
	"""docstring for XBOXController"""


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

    controllerData = 0

    dataQueue = Queue(maxsize = QUEUESIZE)

   def __init__(self, arg):
	super(XBOXController, self).__init__()
	self.connection = self.establishConnection()
	

    #Checks the connection, works with linux	
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
    #Reads events from the USB port, the XBOX controller gives four pieces of data
    #time of event
    #value of motion
    #type of event
    #number ID of input on controller
    #It repacks the data to be in the format ty, number, value         
    def pollController():
        while True:
            try:
                evbuf = self.jsdev.read(8)
                if evbuf:
                    time, value, ty, number = struct.unpack('IhBB', evbuf)

                    controllerData = struct.pack('BBh', ty, number, value)
                    #print(controllerData)
                    addToQueue(controllerData)
                    
            except Exception as msg:
                print(msg)

    #Adds the controller data into the queue without blocking
    def addToQueue(x):
        dataQueue.put_nowait(x)

