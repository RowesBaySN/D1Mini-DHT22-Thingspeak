/* Drafted by Jordan Mellish (jordan.mellish@townsville.qld.gov.au)
 * MIT License 
 * Description: Basic code for sending DHT22(Temperature and Humidity) data to ThingSpeak with a WeMos D1 Pro or D1 Mini  
 * 
 */

/* Import Libraries  */ 
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <DHT.h>

/* Definitions */ 
#define DHTPIN D4 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
long lastConnectionTime = 0;
const int interval = 3600;

/* Wi-Fi Credentials */ 
char ssid[] = "";    //  your network SSID (name) 
char pswd[] = "";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

/* ThingSpeak Credentials  */
unsigned long myChannelNumber = ;
const char * myWriteAPIKey = "";

void setup() {
  Serial.begin(9600); 
  dht.begin(); 
  ThingSpeak.begin(client);

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) { connectWiFi();}

  if (millis() - lastConnectionTime > (interval*1000)){
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT Sensor");
      return;
    } else { 
    //ThingSpeak.setField(Field No. , data)
    ThingSpeak.setField(1, h); 
    ThingSpeak.setField(2, t);  

    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(interval * 1000);
    }
  }
  delay(5);
}

void connectWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address :");
  Serial.println(WiFi.localIP());
  Serial.println();
}

