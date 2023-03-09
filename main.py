import paho.mqtt.client as mqtt
import serial
import os
import time
import config
import pygame

#display part
pygame.init()
window_width = 550
window_height = 300
window = pygame.display.set_mode((window_width, window_height))
pygame.display.set_caption("PianoButLDR")

white = (255, 255, 255)
black = (0, 0, 0)
light_blue = (173, 216, 230)

font = pygame.font.SysFont(None, 30)

keys = [
    {'text': 'C', 'rect': pygame.Rect(10, 10, 50, 180)},
    {'text': 'D', 'rect': pygame.Rect(70, 10, 50, 180)},
    {'text': 'E', 'rect': pygame.Rect(130, 10, 50, 180)},
    {'text': 'F', 'rect': pygame.Rect(190, 10, 50, 180)},
    {'text': 'G', 'rect': pygame.Rect(250, 10, 50, 180)},
    {'text': 'A', 'rect': pygame.Rect(310, 10, 50, 180)},
    {'text': 'B', 'rect': pygame.Rect(370, 10, 50, 180)},
    {'text': 'C2', 'rect': pygame.Rect(430, 10, 50, 180)},
    {'text': 'D2', 'rect': pygame.Rect(490, 10, 50, 180)},
]

#mqtt part
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
        # print('hello')
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
    # print(key)
    # print(cur_instrument)
    # print(notes[key])
    pygame.mixer.Channel(key).play(pygame.mixer.Sound(f'audio\\{cur_instrument}\\{notes[key]}.wav'),fade_ms=100)
    # pygame.mixer.Sound(f'audio\\{instrument[cur_instrument]}\\{note}.mp3').play(fade_ms=100,)

def stopmusic(key):
    pygame.mixer.Channel(key).fadeout(1000) 

ldrstatus = [False]*50
subscribe(client)
while True:
    client.loop(timeout=0)
    data = ser.readline().decode('utf-8').strip() # read a single byte from the serial port
    if data == '': continue
    if data[0].isnumeric() and data[-1].isnumeric():
        for i in range(len(data)):
            if data[i] == '1' and ldrstatus[i] == False:
                playmusic(i,cur_instrument)
                ldrstatus[i] = True
                # print(f'play {i}')
                keys[i]['color'] = light_blue
            elif data[i] == '0' and ldrstatus[i] == True:
                ldrstatus[i] = False
                stopmusic(i)
                keys[i]['color'] = white
    else: print(data)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            for i,key in enumerate(keys):
                if key['rect'].collidepoint(event.pos):
                    playmusic(i,cur_instrument)
                    key['color'] = light_blue
        elif event.type == pygame.MOUSEBUTTONUP:
            for i,key in enumerate(keys):
                key['color'] = white
                stopmusic(i)

    
    window.fill(black)
    for key in keys:
        key_surface = pygame.Surface((key['rect'].width, key['rect'].height))
        key_surface.fill(key.get('color', white))
        pygame.draw.rect(key_surface, black, key_surface.get_rect(), 1)
        text_surface = font.render(key['text'], True, black)
        text_rect = text_surface.get_rect(center=key_surface.get_rect().center)
        key_surface.blit(text_surface, text_rect)
        window.blit(key_surface, key['rect'])

    pygame.display.update()