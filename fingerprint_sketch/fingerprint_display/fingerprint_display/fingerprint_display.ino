//FINGERPRINT - OUTPUT COM LEDS
//bibliotecas
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

#define ledVermelho 8
#define ledAzul 9
#define ledAmarelo 10 
#define pushButton 11
SoftwareSerial mySerial(2, 3);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
//variaveis
int id,contador,param,retorno;
String mensagem;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup() {
  //pinos led
  pinMode(ledVermelho,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  pinMode(ledAmarelo,OUTPUT);
  Serial.begin(9600);
  finger.begin(57600);
  lcd.begin (16,2);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }else{
      Serial.println("Did not find fingerprint sensor :(");
      while (1) { delay(1); }
    }
  finger.getTemplateCount();
  Serial.print("Existem "); Serial.print(finger.templateCount); Serial.println(" biometrias cadastradas");
  Serial.println("Aguardando...");
  printar("Aguardando...");  
  id = finger.templateCount;  

  lcd.clear();
  lcd.setBacklight(HIGH);
  lcd.setCursor(1,0);
  lcd.print("ARDUINO BRASIL");
}


void loop() {
  if(digitalRead(pushButton)==HIGH){
    cadastrar();
  }else getFingerprintIDez();
  delay(50); 
  
}

int tempo_execucao(int retorno){
      contador++;
      Serial.println(contador);
      if(contador==500){
      Serial.println("INATIVO TEMPO EXCEDIDO");
      printar("TEMPO EXCEDIDO");
      delay(2000);  
      Serial.println("Aguardando...");
      printar("Aguardando...");
      contador=0;
      return 1;
    }
    return 0;
}

void printar(String mensagem){
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(mensagem);
}

int captura_digital(int p,int param){
    while (p != FINGERPRINT_OK && retorno == 0) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image capturada");
      break;
    case FINGERPRINT_NOFINGER:
      retorno = tempo_execucao(0);
      break;
    default:
      Serial.println("Erro desconhecido");
      printar("ERRO!");
      break;
    }
  }
  retorno=0;
  contador=0;
  if(p!=FINGERPRINT_OK) return p;
   p = finger.image2Tz(param);
   switch (p) {
     case FINGERPRINT_OK:
       Serial.println("Image converted");
       return p;
       break;
     default:
       Serial.println("Unknown error");
       printar("ERRO!");
       return p;
   }

  
}
uint8_t cadastrar() {
  printar("MODO ADM");
  delay(2000);
  digitalWrite(ledAmarelo,HIGH);
  int p = -1;
  Serial.println("Aguardando leitura...");
  printar("COLOQUE O DEDO");
  param=1;
  p = captura_digital(p,param);
  if(p!=FINGERPRINT_OK) return -1;
  Serial.println("Remove finger");
  printar("RETIRE O DEDO");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  Serial.println("Coloque o mesmo dedo novamente");
  printar("COLOQUE O DEDO");
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
    printar("OK ID: ");
    lcd.setCursor(8,1);
    lcd.print(id);
    digitalWrite(ledAzul,HIGH);
    delay(2000);
    digitalWrite(ledAzul,LOW);
  } else {
    Serial.println("Erro desconhecido!");
    return p;
  }
  digitalWrite(ledAmarelo,LOW); 
  Serial.println("Aguardando leitura..."); 
  printar("Aguardando..."); 
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
    printar("NAO CADASTRADA");
    delay(3000);
    Serial.println("Coloque seu dedo...");
    printar("Coloque seu dedo");
    digitalWrite(ledVermelho,LOW);
    return -1;
  }else{
    if(p != FINGERPRINT_OK) return -1; 
  }
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  printar("OK ID: ");
  lcd.setCursor(8,1);
  lcd.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite(ledAzul,HIGH);
  delay(3000);
  Serial.println("Coloque seu dedo...");
  printar("Coloque seu dedo");
  digitalWrite(ledAzul,LOW);
  return finger.fingerID; 
}
