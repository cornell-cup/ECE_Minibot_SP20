import time 
import spidev

pi = spidev.SpiDev()

h = pi.open(0,1)

pi.mode = 0

pi.max_speed_hz =921600 

stop = time.time() +  120.0

#data = ['\n','u','1','2','\r']


while time.time() < stop:
#x = pi.xfer([1]) #send integers/long using an array
#tx = pi.writebytes([ord('h')]) #send a single character
	x = pi.readbytes(1)
	print(x)
	
# ports = ["ULTRASONIC","3"]
# portname = ports[0]
# portnumber = str(ports[1])
# portsdict = {
# 	"LMOTOR" : "LM",
#         "RMOTOR" : "RM",
#         "MOTOR3" : "M",
#         "LINE" : "L",
#         "INFARED" : "I",
#         "RFID" : "R",
#         "ULTRASONIC" : "U"
#     }
# arr = ['\n'] + list(portnumber) + list(portsdict[portname]) + ['\r']
# for i in range(len(arr)):
# 	tx = pi.writebytes([ord(arr[i])]) #send a single character
# 	print(i)

pi.close()
