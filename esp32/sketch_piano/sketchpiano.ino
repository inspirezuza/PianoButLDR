#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"


#define RED_GPIO       42
#define YELLOW_GPIO    41
#define GREEN_GPIO     40
#define LDR_GPIO       4
#define TOPIC_LIGHT    TOPIC_PREFIX "/light"
#define TOPIC_LED_RED  TOPIC_PREFIX "/led/red"
#define TOPIC_ADJUST  TOPIC_PREFIX "/adjust"
#define TOPIC_INSTRUMENT TOPIC_PREFIX "/instrument"

const int notes = 9;
const int max_notes = 12;
int ldrPins[max_notes+1] = {5, 6, 7,15,16,17,18,8,3};
int ledPins[max_notes+1] = {42,41,40,42,42,42,42,42,42};

int threshold = 35;

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
  // mqtt.subscribe(TOPIC_LED_RED);
  mqtt.subscribe(TOPIC_ADJUST);
  printf("MQTT broker connected.\n");
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, TOPIC_ADJUST) == 0) {
    payload[length] = 0; // null-terminate the payload to treat it as a string
    int value = atoi((char*)payload); 
    threshold = value;
    // Serial.println(threshold);
  }
}


void setup() {
  pinMode(RED_GPIO, OUTPUT);
  digitalWrite(RED_GPIO, 0);
  for (int i = 0; i < notes; i++) {
    pinMode(ldrPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
  connect_wifi();
  connect_mqtt();
  last_publish = 0;
  Serial.begin(115200);
}


void loop() {

  
  // check for incoming subscribed topics

  // publish light value periodically (without using delay)
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
    delay(100);
    Serial.println(ldr_py);
    // Serial.println(ldr_py);
    // int level = 100 - (analogRead(LDR_GPIO)*100/4095);
    // String payload(level);
    // printf("Publishing light value: %d\n", level);

    uint32_t now = millis();
    if (now - last_publish >= 5000) {
      mqtt.publish(TOPIC_LIGHT, String(ldr_status_mqtt).c_str());
      last_publish = now;
      mqtt.loop();    
      Serial.println("threshold " + String(threshold) );
    }
}
