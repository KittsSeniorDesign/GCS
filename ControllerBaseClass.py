#!/usr/bin/env python
import Queue
import xboxController

class ControllerBaseClass(object):
	"""docstring for controllerBaseClass"""

	REFRESHRATE = "Override"
	SENDRATE = "Override"
	QUEUESIZE = "Override"
	
	def __init__(self, arg):
		super(controllerBaseClass, self).__init__()
		self.connection = self.establishConnection()
		


	def establishConnection(self):
		

    def pollController():

    def addToQueue(controllerData):

    def checkConnection():

    def buildQueueEntry():

 