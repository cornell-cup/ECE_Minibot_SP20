import binascii
import spidev
import time
import RPi.GPIO as GPIO

spi = spidev.SpiDev()
GPIO.setmode(GPIO.BOARD)

def setSlave(ardSelect):
    device = 0
    if (ardSelect == 1):
	spi.open(device,1)
    

def transmit(msg):
    try:
	spi.open(0,0)
	spi.mode=0
	spi.max_speed_hz = 115200
	while True:
	    #print(msg)
	    tx=spi.writebytes([msg])
	    time.sleep(0.3)
    finally:
	spi.close()

def setupGPIO():
    GPIO.setup(7, GPIO.OUT)
    GPIO.setup(11, GPIO.OUT)
    

def fwd0():
    GPIO.output(7,0)
    GPIO.output(11,0)
    cmd=ord('F')
    print cmd
    transmit(cmd)


def fwd1():
    GPIO.output(7,1)
    GPIO.output(11,0)
    cmd=ord('F')
    print cmd
    transmit(cmd)

def fwd2():
    GPIO.output(7,0)
    GPIO.output(11,1)
    cmd=ord('F')
    print cmd
    transmit(cmd)

def fwd3():
    GPIO.setup(7, GPIO.OUT)
    GPIO.setup(11, GPIO.OUT)
    GPIO.output(7,1)
    GPIO.output(11,1)
    cmd=ord('F')
    print cmd
    transmit(cmd)

def back(A, B, msg):
    GPIO.output(7,A)
    GPIO.output(11,B)
    cmd=ord(msg)
    #print cmd
    transmit(cmd)

def fwd(A,B, msg):
    GPIO.output(7,A)
    GPIO.output(11,B)
    cmd=ord(msg)
    #print cmd
    transmit(cmd)

