//Ejemplo basedo en https://www.shiftr.io/docs/manuals/arduino
//Works!
#include <WiFi.h>
#include <MQTT.h>
 
const char* ssid = "RamirezGarcia"; //"Necro"; //"RAMIREZ_GARCIA";//"RamirezGarcia"; //IoT-testing // "Necro"
const char* password = "user15371333"; // "frxy8415"; // "83862900";//"user15371333";//12345678 // "frxy8415"
const char* mqttServer = "wemosesp32battery.cloud.shiftr.io";

int start_time;
byte led=0;

const byte interruptPin = 27; //Pin to connect the signal from Geiger detector.
volatile long counts = 0; //counter

WiFiClient espClient;
MQTTClient mqttClient;
 
void setup() {
 
 Serial.begin(115200);
 WiFi.begin(ssid, password);
 
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");

 mqttClient.begin(mqttServer, espClient);

 while (!mqttClient.connect("ESP32", "wemosesp32battery", "public")) {//ClientID, instance name, secret token
    Serial.println("Connecting to mqtt server...");
    delay(1000);
  }
 mqttClient.subscribe("/GeigerCounts");

 attachInterrupt(digitalPinToInterrupt(interruptPin), counter, RISING);
 Serial.print("The counts are published each 10 seconds...");
 pinMode(LED_BUILTIN,OUTPUT);
 start_time=millis();
}
 
void loop() {
  mqttClient.loop();
  if((millis()-start_time)>10000){
    String payload=String(counts);
    mqttClient.publish("/GeigerCounts", payload);
    Serial.println(payload);
    start_time=millis();
  }
}
void counter(){
  counts++;
  led=not(led);
  digitalWrite(LED_BUILTIN,led);
}
