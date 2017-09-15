/*
Drafted by Dylan VanDerWal (dylan.vanderwal@townsville.qld.gov.au)
MIT License
*/

//Pin that the DHT22 is connected to
#define DHTPIN D4

/* ---WiFi Information--- */
//SSID of Network
const char* ssid = "";
//Password of Network
const char* pswd = "";

/* ---Thingspeak Settings--- */
//Thingspeak API Key
String APIKey = "";
//Update interval in seconds, must be greater than 15
const int interval = 20;

/* -----DO NOT EDIT PAST THIS POINT----- */
//Import libraries
#include <ESP8266WiFi.h>
#include <DHT.h>

//Definitions
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE)
long lastConnectionTime = 0;

//define a function
void connectWiFi();


//Code that runs once at startup
void setup() {
  Serial.begin(115200);
  dht.begin();

}


//Code that repeats itself
void loop() {
  if (WiFi.status() != WL_CONNECTED) { connectWiFi(); }

  if (millis() - lastConnectionTime > (interval*1000)) {

    String postData = (""); //define the initial post data

    //get the data from the DHT22
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    } else {
      String S1 = String(h);
      String S2 = String(t);
      postData += ("field1=" + S1 + "&field2=" + S2);
    }
    WiFiClient client;
    if (client.connect("api.thingspeak.com", 80)) {
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postData.length());
      client.print("\n\n");
      client.print(postData);
      String line = client.readStringUntil('\r');   // read all the lines of the reply from server and print them to Serial
      Serial.println(line);
    }
    client.stop();
    lastConnectionTime = millis();
  }
  delay(5); // delay to let things reset
}
void connectWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
