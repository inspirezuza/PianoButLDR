import serial
import os
import time
import config
from pygame import mixer

mode = int(config.DEFAULT_MODE)
PORT = config.PORT

ser = serial.Serial(PORT , 115200) # open the serial port with the same baud rate as the Arduino code

mixer.init()
mixer.set_num_channels(50)
instrument = [filename for filename in os.listdir(r"audio")]
notes = ['C4','D4','E4','F4','G4','A4','B4','C5','D5']
cur_instrument = instrument[0]
ser.write(cur_instrument.encode())

def playmusic(key,mode):
    # print(key,mode)
    print(key)
    print(instrument[mode])
    print(notes[key])
    mixer.Channel(key).play(mixer.Sound(f'audio\\{instrument[mode]}\\{notes[key]}.wav'),fade_ms=100)
    # mixer.Sound(f'audio\\{instrument[mode]}\\{note}.mp3').play(fade_ms=100,)

def stopmusic(key):
    mixer.Channel(key).fadeout(500) 

ldrstatus = [False]*50
while True:
    data = ser.readline().decode('utf-8').strip() # read a single byte from the serial port
    if data == '': continue
    print(data)
    if data[0].isnumeric() and data[-1].isnumeric():
        for i in range(len(data)):
            if data[i] == '1' and ldrstatus[i] == False:
                playmusic(i,mode)
                ldrstatus[i] = True
                print(f'play {i}')
            elif data[i] == '0':
                ldrstatus[i] = False
                stopmusic(i)