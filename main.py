import paho.mqtt.client as mqtt
import serial
import os
import time
import config
import pygame

client = mqtt.Client()
client.username_pw_set(config.MQTT_USER, config.MQTT_PASS)
client.connect(config.MQTT_BROKER, config.IOT_PORT)

instruments = "" 
for i in [filename for filename in os.listdir(r"audio")]:
    # print(i)
    instruments += i + ","
print(instruments[:-1])
client.publish(config.MQTT_USER + "/instruments", instruments[:-1])
cur_instrument = config.DEFAULT_INSTRUMENT
def subscribe(client: mqtt):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        print('hello')
        global cur_instrument
        cur_instrument = msg.payload.decode()
        
    client.subscribe(config.MQTT_USER + "/get_instrument")
    client.on_message = on_message


PORT = config.PORT
ser = serial.Serial(PORT , 115200) # open the serial port with the same baud rate as the Arduino code

# displaypygame.display_setup()
pygame.mixer.init()
pygame.mixer.set_num_channels(50)

notes = ['C4','D4','E4','F4','G4','A4','B4','C5','D5']

def playmusic(key,cur_instrument):
    # print(key,cur_instrument)
    print(key)
    print(cur_instrument)
    print(notes[key])
    pygame.mixer.Channel(key).play(pygame.mixer.Sound(f'audio\\{cur_instrument}\\{notes[key]}.wav'),fade_ms=100)
    # pygame.mixer.Sound(f'audio\\{instrument[cur_instrument]}\\{note}.mp3').play(fade_ms=100,)

def stopmusic(key):
    pygame.mixer.Channel(key).fadeout(1500) 

ldrstatus = [False]*50
subscribe(client)
while True:
    client.loop(timeout=0)
    data = ser.readline().decode('utf-8').strip() # read a single byte from the serial port
    if data == '': continue
    print(data)
    if data[0].isnumeric() and data[-1].isnumeric():
        for i in range(len(data)):
            if data[i] == '1' and ldrstatus[i] == False:
                playmusic(i,cur_instrument)
                ldrstatus[i] = True
                print(f'play {i}')
            elif data[i] == '0':
                ldrstatus[i] = False
                stopmusic(i)