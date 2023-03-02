# # import serial
# # import playsound

# # ser = serial.Serial('COM4', 115200) # Change COM3 to the port that your Arduino is connected to
# # sound_file = 'notes\piano-C4.wav' # Change sound.mp3 to the path of your sound file

# # print(ser)
# # print(ser.name)
# # a = ser.read()
# # ser.write(b'hello')     # write a string
# # ser.close()  
# # # while True:
# # #     if ser.readable():
# # #         print('hello')
# #         # command = ser.read().decode('ascii')
# #         # if command == 'p':
# #         #     playsound.playsound(sound_file)

# # import time
# # from pygame import mixer


# # mixer.init()
# # mixer.music.load("notes\piano-C4.wav")
# # mixer.music.play()
# # while mixer.music.get_busy():  # wait for music to finish playing
# #     time.sleep(1)


import serial
import winsound
import time
from pygame import mixer

mode = 0
ser = serial.Serial('COM4', 115200) # open the serial port with the same baud rate as the Arduino code

mixer.pre_init()
mixer.init()
allmode = ['piano']
def playmusic(key,mode):
    mixer.Channel(key).play(mixer.Sound(f'audio\\{allmode[mode]}\\{key}.mp3'),fade_ms=100)

def stopmusic(key):
    mixer.Channel(key).fadeout(500)

ldrstatus = [False]*50
while True:
    data = ser.readline().decode('utf-8').strip() # read a single byte from the serial port
    print(data)
    for i in range(len(data)):
        if data[i] == '1' and ldrstatus[i] == False:
            playmusic(i,mode)
            ldrstatus[i] = True
            print(f'play {i}')
        elif data[i] == '0':
            ldrstatus[i] = False
            print(i)
            stopmusic(i)
    # if data:
    #     # try:
    #     freq = int.from_bytes(data, byteorder='big') # convert the byte to an integer frequency value
    #     print(data,freq)
    #     if freq == 0:
    #         pass
    #         # winsound.Beep(262, 500) # stop playing the sound
    #         # print('ya')
    #     if freq == 1:
            
    #         print(freq)
    #         # while mixer.music.get_busy():  # wait for music to finish playing
    #         #     time.sleep(0.1)
    #     # except ValueError:
    #     #         print("Error: invalid frequency value:", freq)