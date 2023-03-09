#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SW      2
#define TOPIC_LIGHT    TOPIC_PREFIX "/light"
#define TOPIC_THRESHOLD TOPIC_PREFIX "/threshold"
#define TOPIC_SUB_THRESHOLD TOPIC_PREFIX "/sub_threshold"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int notes = 9;
const int max_notes = 12;
int ldrPins[max_notes+1] = {5, 6, 7,15,16,17,18,8,3};
int ledPins[max_notes+1] = {42,41,40,42,42,42,42,42,42};
int threshold = 35;

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
  printf("MQTT broker connected.\n");
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, TOPIC_SUB_THRESHOLD) == 0) {
    payload[length] = 0; 
    threshold = atoi((char*)payload);
    // display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.clearDisplay();
    display.println("Sensitivity:");
    display.setCursor(0,20);
    display.println(threshold);
    display.display();
  }
}


void setup() {
  for (int i = 0; i < notes; i++) {
    pinMode(ldrPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(115200);
  pinMode(SW, INPUT_PULLUP);
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
  mqtt.publish(TOPIC_THRESHOLD, String(threshold).c_str());
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Sensitivity:");
  display.setCursor(0,20);
  display.println(threshold);
  display.display();
}


void loop() {
  // mqtt.loop();
  // check for incoming subscribed topics
  // mqtt.loop();

  String ldr_status_mqtt = "";
  String ldr_py = "";
  for(int i=0;i<notes;i++){
    int level = 100 - (analogRead(ldrPins[i])*100/4095);
    // String payload(level);
    // printf("Publishing light %d value: %d\n",i, level);
    ldr_status_mqtt += String(level) + ' ';
    
    if (level < threshold){
      digitalWrite(ledPins[i],1);
      ldr_py += "1";
    }
    else{
      digitalWrite(ledPins[i],0);
      ldr_py += "0";
    }
  }
  delay(50);
  Serial.println(ldr_py);
  // Serial.println(ldr_py);
  // int level = 100 - (analogRead(LDR_GPIO)*100/4095);
  // String payload(level);
  // printf("Publishing light value: %d\n", level);
  mqtt.loop();
  uint32_t now = millis();
  if (now - last_publish >= 2000) {
    mqtt.publish(TOPIC_LIGHT, String(ldr_status_mqtt).c_str());
    last_publish = now;
    // mqtt.loop();    
    // Serial.println("threshold " + String(threshold));
  }

  // publish light value periodically (without using delay)
  if (digitalRead(SW)==0){
      threshold += 1;
      mqtt.publish(TOPIC_THRESHOLD, String(threshold).c_str());
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.println("Sensitivity:");
      display.setCursor(0,20);
      display.println(threshold);
      display.display();
      
    while(digitalRead(SW) == 0){
      continue;}
    // mqtt.publish(TOPIC_SWITCH_NEXT, "0");
  }
}