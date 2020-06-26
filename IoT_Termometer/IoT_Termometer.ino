#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "string.h"
#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE); 
int umidade, temperatura;

//--- WIFI ---
const char* ssid = "IEEE 802.11";
const char* password = "EAEA939308";
const char* mqtt_broker = "mqtt.eclipse.org";
const char* mqtt_ClientID = "termometroIOT";
WiFiClient nodemcuClient;
PubSubClient client(nodemcuClient);

const char* topicoTemp = "lab-sergio/temp";
const char* topicoUmid = "lab-sergio/umid";

void setup() {
  Serial.begin(9600);
  conectarWifi();
  client.setServer(mqtt_broker,1883);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  read_temp_umid();
  publishMQTT();
}

void conectarWifi(){
  delay(10);
  Serial.println("CONECTANDO...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void reconectarMQTT(){
    while (!client.connected()) {
      client.connect(mqtt_ClientID);
    }
}

void read_temp_umid() {
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature(false);
  Serial.print("Temperatura:  ");
  Serial.print(temperatura);
  Serial.print(" *C  |");
  Serial.print("  Humidade: ");
  Serial.print(umidade);
  Serial.println("%");
  delay(5000);
}

void publishMQTT(){
  client.publish(topicoTemp, String(temperatura).c_str(), true);
  client.publish(topicoUmid, String(umidade).c_str(), true);
  Serial.println(String(temperatura));
  Serial.println(String(umidade));
}
