#!/usr/bin/env python
import pygame
import serial

from multiprocessing import Manager
from mulitprocessing import Queue

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

    manager = Manager()
    dataQueue = manager.Queue(maxsize = QUEUESIZE)

    def __init__(self,
                 controllerNumber):
        self.controllerNumber = controllerNumber
        self.connection = establishConnection(controllerNumber)
	
	

    #Checks the connection, works with linux	
    def establishConnection():
        os.environ['SDL_VIDEODRIVER'] = 'dummy'
	    connected = False
        while not connected:
            try:

                #Sets up Pygame
                pygame.init()
                screen = pygame.display.set_mode((1, 1))
                pygame.joystick.init()

                #Inits the Controller
                connected = pygame.joystick.get_init()
                self.device = pygame.joystick.Joystick(controllerNumber)
                self.device.init()
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
            for event in pygame.event.get():
                

    #Adds the controller data into the queue without blocking
    def addToQueue(x):
        dataQueue.put_nowait(x)

