import time
import spidev

cs0 = 0
cs1 = 1

pi = spidev.SpiDev()
#spi.open(bus, device)
#cs1 is RFID
#cs0 is motor
pi.open(0,1)
pi.mode = 0
pi.max_speed_hz = 115200
stop = time.time() +120.0

while (time.time() < stop):
#if there is input, read from device 1
	x = pi.readbytes(1)
	if x != [0]:
	    print(x)
	pi.close()
	pi.open(0,cs0)
	pi.mode = 0
	pi.max_speed_hz = 115200
	if x == [1]:
		data = ['\n','c','c']
	elif x == [2]:
		data = ['\n','c','w']
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

pi.close()




