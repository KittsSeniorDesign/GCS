#!/usr/bin/env python
import Queue
import xboxController

class ControllerBaseClass(object):
	"""docstring for controllerBaseClass"""

	REFRESHRATE = "Override"
	SENDRATE = "Override"
	QUEUESIZE = "Override"
	
	def __init__(self, arg):
		super(ControllerBaseClass, self).__init__()
		self.connection = self.establishConnection()
		
	def establishConnection(self):
		raise NotImplementedError("Override establishConnection in class that inherits ControllerBaseClass")

    def pollController():
		raise NotImplementedError("Override pollController in class that inherits ControllerBaseClass")

    def addToQueue(controllerData):
		raise NotImplementedError("Override addToQueue in class that inherits ControllerBaseClass")

    def checkConnection():
		raise NotImplementedError("Override checkConnection in class that inherits ControllerBaseClass")

    def buildQueueEntry():
		raise NotImplementedError("Override buildQueueEntry in class that inherits ControllerBaseClass")