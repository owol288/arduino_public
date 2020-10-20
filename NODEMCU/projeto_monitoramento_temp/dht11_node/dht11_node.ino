#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE); 
int umidade, temperatura;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {

  read_temp_umid();
}


void read_temp_umid() {
  int umidade = dht.readHumidity();
  int temperatura = dht.readTemperature(false);
  Serial.print("Temperatura:  ");
  Serial.print(temperatura);
  Serial.print(" *C  |");
  Serial.print("  Humidade: ");
  Serial.print(umidade);
  Serial.println("%");
  delay(5000);
}
