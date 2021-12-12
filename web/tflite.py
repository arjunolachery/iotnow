# Object Detection using Tensorflow Lite
# uses a trained model on which the webcam live feed is evaluated to find any new objects

# Import libs
import argparse
import importlib.util
import os
import time
from threading import Thread
import requests
from getmac import get_mac_address as gma
import RPi.GPIO as GPIO
import cv2
import numpy as np
from tflite_runtime.interpreter import Interpreter # import tflite interpreter from tensorflow library


GPIO.setwarnings(False) # No GPIO warnings
GPIO.setmode(GPIO.BCM) # Set mode of GPIO
GPIO.setup(17, GPIO.IN)  # Set GPIO port 17 to read data from PIR module
motion_state = False # set the state of motion detection to false

# Parameters for TFLite Model
currentDirectory = os.getcwd() # path to current working directory
CurrentModel = 'Sample_TFLite_model'
CurrentGraph = 'detect.tflite'
CurrentLabels = 'labelmap.txt'
thresholdMinimum = 0.5
feedWidth, feedHeight = 1280, 720

# Model path (tflite model)
modelPath = os.path.join(currentDirectory, CurrentModel, CurrentGraph)

# Path to label map file
labelsPath = os.path.join(currentDirectory, CurrentModel, CurrentLabels)


# LiveFeed class to handle camera feed in a separate thread
class LiveFeed:
    # LiveFeed object created 
    def __init__(self, resolution=(640, 480)): 
        self.feed = cv2.VideoCapture(0) # set live web camera as the live feed
        self.feed.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG')) # set format 
        self.feed.set(3, resolution[0]) # set resolution of live feed
        self.feed.set(4, resolution[1]) # set resolution of live feed
        (self.collected, self.frame) = self.feed.read() # first frame collected
        self.exit = False # set a variable that stores whether the thread has to exit

    def begin(self):
        # begin reading the live feed
        Thread(target=self.revise, args=()).start()
        return self

    def revise(self):
        # indefinitely loops until thread is exited
        while True:
           # exit the thread if the camera is closed
            if self.exit:
                self.feed.release() # give back all the camera resources
                return
            # if not exited, continue with the next frame
            (self.collected, self.frame) = self.feed.read()

    def read(self):
        return self.frame # Returns the latest frame

    def stop(self):
        self.exit = True # An indication to exit 


def sendToServer(objectName, probability): # function to send the recognized object name and its probability to the iotnow server
    payload = {"MACAddress": gma().upper(), "objectName": objectName, "probability": probability}
    r = requests.post("http://www.iotnow.co.in/app/surveillance/dataSend.php", data=payload) # through post request
    print(r.text)


def updateToServer(): # Function to register/Update Device onto server
    payload = {"macAddress": gma().upper(), "MCUId": 2}
    r = requests.post("http://www.iotnow.co.in/send.php", data=payload) # to register/update device on iotnow server
    print(r.text)


# Register/Update Device onto server
updateToServer()

# Read the classes from labelmap.txt and store it to labels
with open(labelsPath, 'r') as file:
    labels = [l.strip() for l in file.readlines()]

del (labels[0]) # delete the first label which is not required

# Load TFLite model with interpreter
interpreter = Interpreter(model_path=modelPath)
interpreter.allocate_tensors()

# Get model specs
specsInput = interpreter.get_input_details()
specsOutput = interpreter.get_output_details()
specsHeight = specsInput[0]['shape'][1]
specsWidth = specsInput[0]['shape'][2]

meanInput = 127.5
stdInput = 127.5

# Start live feed
livefeed = LiveFeed(resolution=(feedWidth, feedHeight)).begin()
time.sleep(2) # wait till feed is ready

# Define classes for which detection should happen
classesCustom = ['person','bicycle','car','motorcycle','bus','truck','bird','cat','dog','cow']

while True:

    # Read the frame as raw and size it correctly
    rawFrame = livefeed.read()
    rgbFrame = cv2.cvtColor(rawFrame, cv2.COLOR_BGR2RGB)
    resizedFrame = cv2.resize(rgbFrame, (specsWidth, specsHeight))
    tfDataInput = np.expand_dims(resizedFrame, axis=0)

    # Run the frame over the TFModel
    interpreter.set_tensor(specsInput[0]['index'], tfDataInput)
    interpreter.invoke()

    # Get the results of detection
    containers = interpreter.get_tensor(specsOutput[0]['index'])[0] # The boundary container shapes of the objects
    classes = interpreter.get_tensor(specsOutput[1]['index'])[0]  # The classes of objects detected stored as its index
    accuracies = interpreter.get_tensor(specsOutput[2]['index'])[0]  # The accuracy of the objects detected stored
    
    # Go over each classes that were detected on the frame and draw a frame around it if it is more than accuracy threshold
    for i in range(len(accuracies)):
        if ((accuracies[i] > thresholdMinimum)):
            # Set the coordinates of the container identified for the class
            # transform and set it to the frame's dimensions
            maximumY = int(min(feedHeight, (containers[i][2] * feedHeight)))
            maximumX = int(min(feedWidth, (containers[i][3] * feedWidth)))
            minimumY = int(max(1, (containers[i][0] * feedHeight)))
            minimumX = int(max(1, (containers[i][1] * feedWidth)))
            # draw a rectangle based on calculated container boundaries
            cv2.rectangle(rawFrame, (minimumX, minimumY), (maximumX, maximumY), (0,200,0), 2)
            # Insert the label with class and probabilty 
            object_name = labels[int(classes[i])]  # Get object name of object in frame from the class index found in labels array
            textLabel = 'Class: %s , Probability: %d%%' % (object_name, int(accuracies[i] * 100))  # It would look as follows = Class: Person, Probability: 70%
            textLabelSize, minLine = cv2.getTextSize(textLabel, cv2.FONT_HERSHEY_SIMPLEX, 0.65, 2)  # Get the size of the font
            textLabel_minimumY = max(minimumY, textLabelSize[1] + 9) # Minimum Y of the label position is set so that it is not on the border but close to it 
            # Create a white rectangle where the text can be placed
            cv2.rectangle(rawFrame, (minimumX, textLabel_minimumY - textLabelSize[1] - 9),(minimumX + textLabelSize[0], textLabel_minimumY + minLine - 9), (255, 255, 255),cv2.FILLED)
            # Write the text over the white rectangle
            cv2.putText(rawFrame, textLabel, (minimumX, textLabel_minimumY - 7), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 0, 0),2)
            if any(classC in object_name for classC in classesCustom): # if the class is found in the desired classes then proceed
                classDetected = object_name
            i = GPIO.input(17) # read the PIR motion sensor out data
            if i == 0 and motion_state == 1: # if condition only if there is a change in motion state to low
                print("Motion not detected")
                motion_state = 0 # set motion state to the the new low state
            elif i == 1 and motion_state == 0: # if condition only if there is a change in motion state to high
                print("Motion detected")
                motion_state = 1  # set motion state to the the new high state
                if classDetected: # if class exists then send to the iotnow server
                    sendToServer(classDetected, int(accuracies[i] * 100))

    # Display the live feed (frames) with the objects detected and their probabilities
    cv2.imshow('Live Feed', rawFrame)

    # Quit live feed by pressing 'x'
    if cv2.waitKey(1) == ord('x'):
        break

# End live feed 
cv2.destroyAllWindows()
LiveFeed.stop()
