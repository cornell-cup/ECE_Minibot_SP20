import time
import spidev

pi = spidev.SpiDev()

h = pi.open(0,1)
pi.mode = 0
pi.max_speed_hz = 115200
stop = time.time() +120.0

while (time.time() < stop):
	x = pi.readbytes(1)
	pi.close()
	a = pi.open(0,0)
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
	h = pi.open(0,1)
	pi.mode = 0
	pi.max_speed_hz = 115200





