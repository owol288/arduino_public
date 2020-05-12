//FINGERPRINT - OUTPUT COM LEDS
//bibliotecas
#include <Adafruit_Fingerprint.h>

#define ledVermelho 8
#define ledAzul 9
#define ledAmarelo 10 
#define pushButton 11
SoftwareSerial mySerial(2, 3);
//variaveis
int id,contador,param;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup() {
  //pinos led
  pinMode(ledVermelho,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  pinMode(ledAmarelo,OUTPUT);
  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }else{
      Serial.println("Did not find fingerprint sensor :(");
      while (1) { delay(1); }
    }
  finger.getTemplateCount();
  Serial.print("Existem "); Serial.print(finger.templateCount); Serial.println(" biometrias cadastradas");
  Serial.println("Aguardando...");  
  id = finger.templateCount;  
}

void tempo_execucao(int retorno,int espera){
      contador++;
      if(contador==espera){
      Serial.println("INATIVO TEMPO EXCEDIDO");
      contador=0;
    }
}

void loop() {
  if(digitalRead(pushButton)==HIGH){
    cadastrar();
  }else getFingerprintIDez();
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
int captura_digital(int p,int param){
    while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image capturada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    default:
      Serial.println("Erro desconhecido");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(param);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
}
uint8_t cadastrar() {
  digitalWrite(ledAmarelo,HIGH);
  int p = -1;
  Serial.println("Coloque seu dedo...");
  param=1;
  p = captura_digital(p,param);
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  Serial.println("Coloque o mesmo dedo novamente");
  param=2;
  p = captura_digital(p,param);
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Digitais batem!");
    id++;
  } else {
    Serial.println("Erro desconhecido");
    digitalWrite(ledVermelho,HIGH);
    delay(2000);
    digitalWrite(ledVermelho,LOW);
    Serial.println("Aguardando leitura...");
    
    return p;
  }   

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.print("Digital armazenada com sucesso no ID: "); Serial.println(id);
    digitalWrite(ledAzul,HIGH);
    delay(2000);
    digitalWrite(ledAzul,LOW);
  } else {
    Serial.println("Erro desconhecido!");
    return p;
  }
  digitalWrite(ledAmarelo,LOW); 
  Serial.println("Aguardando leitura...");  
}
