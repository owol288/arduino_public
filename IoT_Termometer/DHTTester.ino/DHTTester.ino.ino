#include "DHT.h"

//DHT Area
#define DHTPIN D3 // set the pin that DHT is connected to
#define DHTTYPE DHT11 // set the DHT version
DHT dht(DHTPIN, DHTTYPE); // declares the DHT object

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 test!"));

  dht.begin(); // initialize dht
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  DHT_readings();
}

// Function that takes dht readings and show results
void DHT_readings(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // A series of prints to show readings
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
}
