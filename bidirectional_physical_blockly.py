import time
import spidev

cs0 = 0
cs1 = 1

pi = spidev.SpiDev()
#spi.open(bus, device)
#cs1 is RFID
#cs0 is motor
pi.open(0,cs1)
pi.mode = 0
pi.max_speed_hz = 115200
stop = time.time() +120.0

while (time.time() < stop):
#if there is input, read from device 1
	x = pi.readbytes(1)
	pi.close()
	pi.open(0,cs0)
	pi.mode = 0
	pi.max_speed_hz = 115200
	if x == [0]:
		data = ['\n','s']
	elif x == [1]:
		data = ['\n','t','l']
	elif x == [2]:
		data = ['\n','t','r']
	elif x == [3]:
		data = ['\n','t','a']
	else:
		data = []
	for i in range(len(data)):
 		tx = pi.writebytes([ord(data[i])]) #send a single character

	pi.close()
	pi.open(0,cs1)
	pi.mode = 0
	pi.max_speed_hz = 115200





