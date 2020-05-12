//FINGERPRINT - OUTPUT COM LEDS
//bibliotecas
#include <Adafruit_Fingerprint.h>

#define ledVermelho 8
#define ledAzul 9
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup() {
  //pinos led
  pinMode(ledVermelho,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }else{
      Serial.println("Did not find fingerprint sensor :(");
      while (1) { delay(1); }
    }
}


void loop() {
  getFingerprintIDez();
  delay(50); 
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_NOTFOUND){
    digitalWrite(ledVermelho,HIGH);
    Serial.println("DIGITAL NAO CADASTRADA");
    delay(3000);
    Serial.println("Coloque seu dedo...");
    digitalWrite(ledVermelho,LOW);
    return -1;
  }else{
    if(p != FINGERPRINT_OK) return -1; 
  }
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite(ledAzul,HIGH);
  delay(3000);
  Serial.println("Coloque seu dedo...");
  digitalWrite(ledAzul,LOW);
  return finger.fingerID; 
}
