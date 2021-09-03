import serial
import time

s = serial.Serial('/dev/pts/7')

for line in file("lecturas.txt"):
	s.write(line) 
	time.sleep(1)