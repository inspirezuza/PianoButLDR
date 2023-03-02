const int notes = 3;
int ldrPins[notes+1] = {5, 6, 7};
int ledPins[notes+1] = {42,41,40}; 
// char ldrStatus[notes+1] = {'0','0','0','\n'};

int threshold = 20;

void setup() {
  for (int i = 0; i < notes; i++) {
    pinMode(ldrPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
  // analogSetAttenuation(ADC_11db);
  Serial.begin(115200);
}

void loop() {
  String ldr_string = "";
  for (int i=0;i<notes;i++){
    if (analogRead(ldrPins[i]) < threshold){
      digitalWrite(ledPins[i],1);
      ldr_string += "1";
      // ldrStatus[i] = '1';
    }
    else{
      digitalWrite(ledPins[i],0);
      ldr_string += "0";
      // ldrStatus[i] = '0';
    }
  }
  Serial.println(ldr_string);
  delay(100);
}
