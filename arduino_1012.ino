/*
  This a simple example of the aREST Library for the ESP8266 WiFi chip.
  This example illustrate the cloud part of aREST that makes the board accessible from anywhere
  See the README file for more details.

  Written in 2015 by Marco Schwartz under a GPL license.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>


// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

char* device_id = "vrtest";
const char* ssid = "IOTAP";
const char* password = "raspberry";
int infrared_VR_Pin = 5;
int infrared_LED_Pin = 0;
int BuzzerPin = 4;
boolean infrared_var = LOW;
String vr_val="";

// Unique ID to identify the device for cloud.arest.io
//char* device_id = "vrtest";
//
//// WiFi parameters
//const char* ssid = "your_wifi_network_name";
//const char* password = "your_wifi_network_password";

// Variables to be exposed to the API
int temperature;
int humidity;

// Functions
void callback(char* topic, byte* payload, unsigned int length);

void vrs()
{
infrared_var = digitalRead(infrared_VR_Pin);
 
  //Serial.print(infrared_var);
  if (infrared_var == HIGH)
  {
    digitalWrite(infrared_LED_Pin, HIGH);
   
    vr_val="abnormal";
    
    Alarm();
  }
  else if (infrared_var == LOW)
  {
    digitalWrite(infrared_LED_Pin, LOW);
    
    noTone(BuzzerPin);
    vr_val="Safety!!";
  }

  delay(1000);
}

void Alarm() //bz副程式
{


  tone(BuzzerPin, 2000);
  delay(250);
  tone(BuzzerPin, 1000);
  delay(250);
}

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id(device_id);
  rest.set_name("esp8266");
  rest.variable("vr",&vr_val);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Set output topic
  char* out_topic = rest.get_topic();

}

void loop() {

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)

void callback(char* topic, byte* payload, unsigned int length) {
  vrs();
  rest.handle_callback(client, topic, payload, length);

}
