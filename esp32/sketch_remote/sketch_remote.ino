#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SW_MODE 5
#define SW_NEXT 7
#define SW_BACK 6
#define TOPIC_DIS      TOPIC_PREFIX "/text"
#define TOPIC_THRESHOLD_UPDATE TOPIC_PREFIX "/thresholdupate"
#define TOPIC_INSTRUMENT_UPDATE TOPIC_PREFIX "/instrumentupdate"
#define TOPIC_SUB_THRESHOLD TOPIC_PREFIX "/sub_threshold"
#define TOPIC_GET_INSTRUMENT TOPIC_PREFIX "/get_instrument"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int threshold = 35;
const int ninstru = 7;
int curpos = 2;
String instruments[ninstru+5] = {"acoustic_guitar_steel","electric_guitar_clean","piano","tinkle_bell","trumpet","tubular_bells","violin"};
String cur_instrument = "piano";
String mode = "sensitivity";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiClient wifiClient;
PubSubClient mqtt(MQTT_BROKER, 1883, wifiClient);
uint32_t last_publish;


void connect_wifi() {
  printf("WiFi MAC address is %s\n", WiFi.macAddress().c_str());
  printf("Connecting to WiFi %s.\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    printf(".");
    fflush(stdout);
    delay(500);
  }
  printf("\nWiFi connected.\n");
}


void connect_mqtt() {
  printf("Connecting to MQTT broker at %s.\n", MQTT_BROKER);
  if (!mqtt.connect("", MQTT_USER, MQTT_PASS)) {
    printf("Failed to connect to MQTT broker.\n");
    for (;;) {} // wait here forever
  }
  mqtt.setCallback(mqtt_callback);
  mqtt.subscribe(TOPIC_SUB_THRESHOLD);
  mqtt.subscribe(TOPIC_GET_INSTRUMENT);
  printf("MQTT broker connected.\n");
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, TOPIC_SUB_THRESHOLD) == 0) {
    payload[length] = 0; 
    threshold = atoi((char*)payload);
    Serial.println(threshold);
  }
  if (strcmp(topic, TOPIC_GET_INSTRUMENT) == 0) {
    payload[length] = 0; 
    cur_instrument = (char*)payload;
    Serial.println(cur_instrument);
  }
  displayupdate();
}

void displayupdate(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Mode:");
  display.println(mode); 
  // display.setCursor(0,30);
  display.println("Sensitivity: ");
  display.println(String(threshold));
  // display.setCursor(0,50); 
  display.println("Current Instrument:");
  // display.setCursor(0,40);
  display.println(cur_instrument);
  display.display();
  Serial.println(threshold);
}

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  pinMode(SW_MODE, INPUT_PULLUP);
  pinMode(SW_NEXT, INPUT_PULLUP);
  pinMode(SW_BACK, INPUT_PULLUP);
  connect_wifi();
  connect_mqtt();
  last_publish = 0;
  Serial.begin(115200);
  Wire.begin(48, 47);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  analogSetAttenuation(ADC_11db);
  displayupdate();
}


void loop() {
  mqtt.loop();

  if (digitalRead(SW_NEXT)==0){
    if (mode == "sensitivity"){
        threshold += 1;
        mqtt.publish(TOPIC_THRESHOLD_UPDATE, String(threshold).c_str());
      }
    else if (mode == "instrument"){
        curpos += 1;
        if(curpos > ninstru-1){
          curpos = 0;
        }
        cur_instrument = instruments[curpos];
        mqtt.publish(TOPIC_INSTRUMENT_UPDATE, instruments[curpos].c_str());
        Serial.println(instruments[curpos]);
      }
      
    while(digitalRead(SW_NEXT) == 0){
      continue;}
    delay(100);
    Serial.println("next");
  }

  if (digitalRead(SW_BACK)==0){
    if (mode == "sensitivity"){
      threshold -= 1;
        mqtt.publish(TOPIC_THRESHOLD_UPDATE, String(threshold).c_str());
      }
    else if (mode == "instrument"){
        curpos -= 1;
          if(curpos < 0){
            curpos = ninstru-1;
          }
        cur_instrument = instruments[curpos];
        mqtt.publish(TOPIC_INSTRUMENT_UPDATE, instruments[curpos].c_str());
        }
        
      
    while(digitalRead(SW_BACK) == 0){
      continue;}
    delay(100);
    Serial.println("back");
  }

  if (digitalRead(SW_MODE)==0){
    if (mode == "sensitivity"){
        mode = "instrument";
      }
    else if (mode == "instrument"){
        mode = "sensitivity";
      }
    while(digitalRead(SW_MODE) == 0){
      continue;}
    delay(100);
    displayupdate();
    Serial.println("mode");
  }
    if (digitalRead(2)==0){
    if (mode == "sensitivity"){
        mode = "instrument";
      }
    else if (mode == "instrument"){
        mode = "sensitivity";
      }
    while(digitalRead(2) == 0){
      continue;}
    delay(100);
    displayupdate();
    Serial.println("debug mode");
  }
}