#include <MCUFRIEND_kbv.h>
#include <String.h>
#include <Wire.h>
#include "RTClib.h"
#include <TouchScreen.h>
MCUFRIEND_kbv tft;
RTC_DS1307 rtc;

//declaração de variaveis globais
int dia = 3,dia_ant=-1;
int i = 0; int d = 0;
char daysOfTheWeek[7][14] = {"Domingo", "Segunda-feira", "Terca-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sabado"};

//função que vai printar frases
void frase(int dia){
  char *frases[]={
    "Frase 1",
    "Frase 2",
    "Frase 3",
    "Frase n"
  };
  tft.setCursor(20,70);
  tft.print(frases[dia]);
}

//Definir cores que poderao ser utilizadas
#define VERDE 0x07E0
#define PRETO 0x0
#define BRANCO 0xFFFF
#define AZUL 0x07FF
#define AMARELO 0xFFE0
#define RED 0xF800
//pinos touch
#define XP 6
#define XM A2
#define YP A1
#define YM 7
// limites do touch
#define TS_LEFT 185
#define TS_RT 208
#define TS_TOP 934
#define TS_BOT 185
#define MINPRESSURE 5 // Pressao min
#define MAXPRESSURE 2000 // Pressao max

TouchScreen ts = TouchScreen(XP,YP,XM,YM,300);

bool pressionado = false;
int posX, posY,bot=0;

//funcao booleana para obter o toque
bool obterToque(void){
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE);
  if (pressed) {
        posX = map(p.y, TS_LEFT, TS_RT, 0, 320); 
        posY = map(p.x, TS_TOP, TS_BOT , 0, 240);
        return true;
    }
    return false;
}

void setup() {
  Serial.begin(57600);
  if(! rtc.begin()){
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //A linha abaixo ajusta o relogio, descomente e atualize apenas na primeira vez que carregar o codigo.
  //rtc.adjust(DateTime(2019, 10, 8, 18, 11, 0)); 
  Serial.begin(9600);
  tft.begin(0x1289);
  tft.setRotation(1);
  tft.fillScreen(PRETO);
  tft.setCursor(30,70);
  tft.setTextColor(BRANCO);
  tft.setTextSize(3);
  tft.print(" INICIALIZANDO...  ");
  delay(500);
  tft.fillScreen(PRETO);
}

//funcao que recebe o local do toque e mostra mensagens anteriores e proximas
void local_toque(int toque){
  if(toque){
    if(posX<1999)
        bot = 1;
    if(posX>7001)
        bot = 2;
    if(posX<7000 && posX>2000)
        bot = 3; 
  }
  if(bot==3){
    tft.fillScreen(PRETO);
    frase(dia);
  }
  if(bot==1){
    tft.fillScreen(PRETO);
    dia_ant+=1;
    tft.setCursor(20,60);
    if(dia_ant<dia)
      frase(dia_ant);
    else
      tft.print("Calma, a proxima mensagem so pode ser vista amanha");
    while(obterToque()!=true)
      toque = obterToque();   
  }
  if(bot==2){
    tft.fillScreen(PRETO);
    tft.setCursor(20,60);
    dia_ant-=1;
    if(dia_ant>=0)
      frase(dia_ant);
    while(obterToque()!=true)
      toque = obterToque();
  }
  bot=0;
}

void loop() {
  bool toque; 
  int last;
  tft.fillScreen(PRETO);
  tft.setTextSize(2);
  DateTime now = rtc.now();
  last = now.second();
  if(dia_ant<0 || now.hour()==00)
    dia_ant=dia;
    tft.setCursor(56,7);
    tft.print(now.day());
    tft.print('/');
    tft.print(now.month());
    tft.print('/');
    tft.print(now.year());
    tft.print(" | ");
    tft.print(now.hour());
    tft.print(':');
    tft.print(now.minute());
    tft.setCursor(52,30);
    tft.print(" (");
    tft.print(daysOfTheWeek[now.dayOfTheWeek()]);
    tft.println(") ");
    tft.setCursor(10,40);
    tft.print("_________________________");
    tft.println();
    if(now.hour() == 00)
     dia +=1;
   frase(dia);
    while(last<59){
      DateTime now = rtc.now();
      last = now.second();
      toque = obterToque();
      message(toque);
      if(toque==true)
      break; 
  }   
}
