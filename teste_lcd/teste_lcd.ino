#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#define VERDE   0x07E0
#define BRANCO 0xFFFF
 
void setup() {
  Serial.begin(9600);
  uint16_t ID = 0x1289; 
  Serial.print("Identificador do display: ");
  Serial.println(ID,HEX);
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BRANCO);
  tft.setCursor(20,100);
  tft.setTextColor(VERDE);
  tft.setTextSize(3);
  tft.print("Arduino Brasil!");
}
 
void loop() {
 
}
