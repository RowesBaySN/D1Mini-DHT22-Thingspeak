/* Drafted by Jordan Mellish (jordan.mellish@townsville.qld.gov.au)
 * MIT License 
 * Description: Basic code for sending DHT11(Temperature and Humidity) data to ThingSpeak with a WeMos D1 Pro or D1 Mini  
 * Including ESP Deep Sleep for 15 minutes.
 */

/* Import Libraries  */ 
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <DHT.h>

/* Definitions */ 
#define DHTPIN D3 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int sleepTimeS = 900;

/* Wi-Fi Credentials */ 
char ssid[] = "YOUR-WIFI-NAME";    //  your network SSID (name) 
char pswd[] = "YOUR-WIFI-PASSWORD";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

/* ThingSpeak Credentials  */
unsigned long myChannelNumber = ; // Enter your channel number from ThingSpeak
const char * myWriteAPIKey = "YOUR-API-KEY"; //Enter your Write API Key from ThingSpeak

void setup() {
  Serial.begin(115200); 
  dht.begin(); 
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED) { connectWiFi();}
    Serial.print("I'm Awake");
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.print(t);
    Serial.print(h);
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT Sensor");
      return;
    } else { 
    //ThingSpeak.setField(Field No. , data)
    ThingSpeak.setField(1, t); 
    ThingSpeak.setField(2, h);  

    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(2000);
    Serial.println("Back to sleep");
    ESP.deepSleep(sleepTimeS * 1000000);
    }

}

void loop() {
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
