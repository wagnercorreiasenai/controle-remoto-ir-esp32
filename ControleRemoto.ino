#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <LiquidCrystal_I2C.h>

//Colunas e linhas do Display I2C
int lcdColumns = 16;
int lcdRows = 2;

//Inicializa a lib no endereço 0x27
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

byte SEND_PIN = 23;
IRsend irsend(SEND_PIN);

void setup() {
  irsend.begin();
  Serial.begin(9600);

  //Inicializa o display
  lcd.init();
  lcd.backlight();
  prepararDisplay();
}

void lerSerial() {
  if (Serial.available()) {
    char byteRead = '0';
    byteRead = Serial.read();

    switch (byteRead) {
      case '+':
        aumentarVolume();
        break;
      case '-':
        diminuirVolume();
        break;
      case 'p':
        playPause();
        break;
      case '>':
        proximaMusica();
        break;
      case '<':
        musicaAnterior();
        break;
      default:
        Serial.println("Comando nao reconhecido");
    }
  }
}

void aumentarVolume() {
  uint16_t rawData[77] = { 4420, 4604, 424, 584, 420, 586, 420, 584, 420, 584, 420, 1592, 420, 1588, 422, 586, 418, 612, 394, 1590, 418, 1592, 418, 1590, 422, 1616, 394, 640, 364, 612, 392, 614, 386, 642, 364, 4664, 328, 676, 330, 676, 330, 676, 330, 676, 330, 1708, 302, 1710, 300, 1710, 278, 700, 328, 1708, 302, 1710, 278, 1732, 300, 704, 300, 704, 300, 704, 302, 702, 304, 1708, 302, 676, 328, 704, 302, 704, 302, 1708, 304 };  // UNKNOWN CF2F9DAB
  enviarComandoIR(rawData, 77, 39);
  escreverNoDisplay("Aumentar volume");
}

void diminuirVolume() {
  uint16_t rawData[77] = { 4380, 4674, 328, 678, 326, 678, 328, 678, 330, 674, 332, 1710, 300, 1710, 302, 678, 328, 678, 328, 1710, 300, 1684, 328, 1708, 302, 1682, 328, 678, 330, 674, 330, 676, 328, 678, 328, 4700, 328, 678, 328, 676, 330, 674, 328, 678, 330, 702, 302, 676, 328, 678, 328, 1706, 302, 1712, 302, 1682, 328, 1686, 324, 678, 328, 1708, 302, 1682, 328, 1710, 302, 676, 328, 678, 328, 678, 330, 676, 328, 1710, 302 };  // UNKNOWN B2BBAC69
  enviarComandoIR(rawData, 77, 39);
  escreverNoDisplay("Diminuir volume");
}

void playPause() {
  uint16_t rawData[77] = { 4356, 4698, 302, 676, 328, 676, 332, 676, 328, 652, 354, 1708, 304, 1708, 304, 674, 330, 678, 328, 1708, 302, 1708, 302, 1686, 326, 1684, 328, 676, 330, 676, 328, 678, 330, 652, 354, 4700, 328, 674, 328, 676, 330, 674, 332, 674, 328, 678, 328, 1708, 304, 676, 328, 1708, 302, 678, 328, 678, 328, 674, 328, 1710, 302, 1682, 326, 678, 332, 1704, 304, 676, 330, 1684, 328, 1706, 304, 1680, 330, 676, 328 };  // UNKNOWN 28DE45AA
  enviarComandoIR(rawData, 77, 39);
  escreverNoDisplay("Play / Pause");
}

void proximaMusica() {
  uint16_t rawData[77] = { 4360, 4668, 330, 674, 330, 676, 330, 676, 354, 652, 354, 1656, 356, 1630, 382, 650, 356, 650, 354, 1660, 350, 1656, 354, 1658, 356, 1658, 350, 654, 354, 652, 354, 650, 330, 674, 356, 4674, 330, 672, 330, 676, 330, 676, 330, 676, 328, 1682, 328, 678, 352, 650, 354, 650, 354, 1660, 354, 1654, 356, 650, 354, 1654, 358, 650, 354, 1632, 380, 1654, 356, 1610, 400, 650, 356, 648, 356, 1628, 384, 622, 384 };  // UNKNOWN 46868606
  enviarComandoIR(rawData, 77, 39);
  escreverNoDisplay("Proximo");
}

void musicaAnterior() {
  escreverNoDisplay("Anterior");
  uint16_t rawData[77] = { 4498, 4580, 446, 560, 444, 562, 444, 562, 444, 534, 470, 1568, 446, 1566, 444, 562, 444, 560, 444, 1540, 472, 1566, 446, 1564, 446, 1564, 446, 562, 442, 562, 444, 538, 468, 560, 420, 4608, 420, 586, 418, 586, 418, 586, 422, 584, 420, 586, 420, 584, 420, 586, 420, 586, 420, 1594, 418, 1590, 420, 586, 420, 1590, 420, 1592, 420, 1592, 420, 1590, 420, 1594, 418, 584, 420, 586, 418, 1592, 444, 562, 444 };  // UNKNOWN 879B92C2
  enviarComandoIR(rawData, 77, 39);
  delay(500);
  enviarComandoIR(rawData, 77, 39);
}

void enviarComandoIR(uint16_t rawData[], int tamanhoComando, int frequencia) {
  irsend.sendRaw(rawData, tamanhoComando, frequencia);
  Serial.println("Enviando comando IR");
}

void escreverNoDisplay(char txt[]) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("*Acionado*");

  lcd.setCursor(0, 1);
  lcd.print(txt);
}

void prepararDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*Iniciado*");
}

void loop() {
  lerSerial();
}