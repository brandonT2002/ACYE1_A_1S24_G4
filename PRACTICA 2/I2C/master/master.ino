#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define SLAVE_ADDRESS 9  // Dirección del esclavo
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int botonPin = 12;

#define up 3
#define down 2
#define enter 4
#define Clear 5

int modeHistory = 0;
byte pointer[] = { 0x00, 0x00, 0x04, 0x02, 0x1D, 0X02, 0X04, 0X00 };
bool inSubMenu = false;

#define TRIG 10
#define ECO 9
#define LED 8
#define LED2 7

int DURATION;
int DISTANCIA;

int downsel = 0;
int filterMode = 0;

String mensajeRecibido = "";

int hystoryMode = 0;
bool inColorMenu = false;

int numGrande = 0, numMediano = 0, numPeque = 0;



int numToSend = 2;  // Número a enviar al esclavo (puede ser cualquier número)

unsigned long tiempoInicio1 = 0;

void esperar(int segs) {
  tiempoInicio1 = millis();
  while (true) {
    if (millis() - tiempoInicio1 >= segs) {
      return;
    }
  }
}


// Metodo para guardar en la EEPROM
void guardarValor(byte indice) {
  byte direccion;
  byte valor;
  switch (indice) {
    case 0:
      direccion = 0;
      valor = leerValor(direccion).toInt();
      valor += 1;
      break;
    case 1:
      direccion = 1;
      valor = leerValor(direccion).toInt();
      valor += 1;
      break;
    case 2:
      direccion = 2;
      valor = leerValor(direccion).toInt();
      valor += 1;
      break;
    case 3:
      direccion = 3;
      valor = leerValor(direccion).toInt();
      valor += 1;
      break;
    case 4:
      direccion = 4;
      valor = leerValor(direccion).toInt();
      valor += 1;
      break;
    case 5:
      direccion = 5;
      valor = leerValor(direccion).toInt();
      valor += 1;
      break;
    default:
      return;
  }
  EEPROM.write(direccion, valor);
}

// Función para leer un valor de la EEPROM
String leerValor(byte indice) {
  byte direccion;
  switch (indice) {
    case 0:
      direccion = 0;
      break;
    case 1:
      direccion = 1;
      break;
    case 2:
      direccion = 2;
      break;
    case 3:
      direccion = 3;
      break;
    case 4:
      direccion = 4;
      break;
    case 5:
      direccion = 5;
      break;
    default:
      return "-1";
  }
  byte valor = EEPROM.read(direccion);
  return String(valor);
}

void borrarEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}

void UPandDOWN() {
  lcd.clear();
  switch (downsel) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }
  if (downsel == 0 || downsel == 1) {
    lcd.setCursor(1, 0);
    lcd.print("Historial");
    lcd.setCursor(1, 1);
    lcd.print("Modo Filtro");
  } else {
    lcd.setCursor(1, 0);
    lcd.print("Modo Filtro");
    lcd.setCursor(1, 1);
    lcd.print("Reset");
  }

  if (digitalRead(down) == LOW && downsel <= 1) {
    downsel++;
    esperar(200);
  }
  if (digitalRead(up) == LOW && downsel > 0) {
    downsel--;
    esperar(100);
  }
  if (digitalRead(enter) == LOW) {
    if (downsel == 0) {
      inSubMenu = true;
      inColorMenu = true;
      filterMode = 0;
    } else if (downsel == 1) {
      inSubMenu = true;
      inColorMenu = false;
      filterMode = 0;
    } else {
      ENTER();
    }
  }

  esperar(100);
}

void filterMenu() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  switch (filterMode) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }
  lcd.setCursor(1, 0);
  lcd.print("por color");
  lcd.setCursor(1, 1);
  lcd.print("por tamanio");

  if (digitalRead(down) == LOW && filterMode < 1) {
    filterMode++;
    esperar(200);
  }
  if (digitalRead(up) == LOW && filterMode > 0) {
    filterMode--;
    esperar(100);
  }
  if (digitalRead(Clear) == LOW) {
    inSubMenu = false;
  }

  if (digitalRead(enter) == LOW) {
    if (filterMode == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      Wire.write(1);
      lcd.print("Modo por color seleccionado");
      Serial.println("se selecciono por color");
      lcd.scrollDisplayLeft();
      Wire.requestFrom(SLAVE_ADDRESS, 30);  // Solicita datos al esclavo
      while (Wire.available()) {
        byte mensajeRecibido = Wire.read();
      }
      //esperar(2000);  // Espera 2 segundos
    } else if (filterMode == 1) {
      // Aquí puedes agregar el código para manejar la selección de "por tamaño"
      lcd.clear();
      lcd.setCursor(0, 0);
      Wire.write(2);
      lcd.print("Modo por tamanio seleccionado");
      Serial.println("se selecciono por tamanio");
      Wire.requestFrom(SLAVE_ADDRESS, 30);  // Solicita datos al esclavo
      while (Wire.available()) {
        byte mensajeRecibido = Wire.read();
      }
      //esperar(2000);  // Espera 2 segundos
    }
  }

  esperar(100);
}


void historyMenu() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  switch (hystoryMode) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }
  lcd.setCursor(1, 0);
  lcd.print("Por color");
  lcd.setCursor(1, 1);
  lcd.print("Por tamanio");

  if (digitalRead(down) == LOW && hystoryMode < 1) {
    hystoryMode++;
    esperar(200);
  }
  if (digitalRead(up) == LOW && hystoryMode > 0) {
    hystoryMode--;
    esperar(100);
  }
  if (digitalRead(Clear) == LOW) {
    inColorMenu = false;
  }

  if (digitalRead(enter) == LOW) {
    inColorMenu = false;
    if (hystoryMode == 0) {
      modeHistory = 0;
      hystoryMode = 0;
    } else if (hystoryMode == 1) {
      modeHistory = 1;
    }

    while (1) {
      enterData();
      inSubMenu = true;
      if (digitalRead(Clear) == LOW) {
        lcd.clear();
        inColorMenu = true;
        break;
      }
    }
  }
  esperar(100);
}

void ENTER() {
  switch (downsel) {
    case 2:
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Reiniciando...");
      esperar(1000);
      break;
  }
}

int dataI = 0;
void enterData() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  switch (dataI) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }

  if (modeHistory == 1) {
    printSize();
  } else if (modeHistory == 0) {
    printColor();
  }

  if (digitalRead(down) == LOW && dataI < 2) {
    dataI++;
    esperar(200);
  }
  if (digitalRead(up) == LOW && dataI > 0) {
    dataI--;
    esperar(100);
  }
  esperar(100);
}

void printSize() {
  if (dataI == 0 || dataI == 1) {
    lcd.setCursor(1, 0);
    lcd.print("Grandes: " + leerValor(3));
    lcd.setCursor(1, 1);
    lcd.print("Medianas: " + leerValor(4));
  } else {
    lcd.setCursor(1, 0);
    lcd.print("Medianas: " + leerValor(4));
    lcd.setCursor(1, 1);
    lcd.print("Pequenias: " + leerValor(5));
  }
}

void printColor() {
  if (dataI == 0 || dataI == 1) {
    lcd.setCursor(1, 0);
    lcd.print("Color AZUL: " + leerValor(0));
    lcd.setCursor(1, 1);
    lcd.print("Color ROJO: " + leerValor(1));
  } else {
    lcd.setCursor(1, 0);
    lcd.print("Color ROJO: " + leerValor(1));
    lcd.setCursor(1, 1);
    lcd.print("Color AMARILLO: " + leerValor(2));
  }
}

void setup() {

  Wire.begin();  // Inicializar el bus I2C
  Serial.begin(9600);         // Inicializar la comunicación serial

  lcd.init();  //inicio lcd
  lcd.backlight();
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
  pinMode(Clear, INPUT_PULLUP);
  pinMode(botonPin, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("<GRUPO04_A_1S24>");
  lcd.setCursor(5, 1);
  lcd.print("Practica 2");
  esperar(2000);
  lcd.clear();

  lcd.createChar(5, pointer);

  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  digitalWrite(LED, HIGH);
  digitalWrite(LED2, HIGH);
}

void loop() {
  if (digitalRead(botonPin) == LOW) {
    esperar(30000);
  }

  /*
  digitalWrite(TRIG, HIGH);
  esperar(1);
  digitalWrite(TRIG, LOW);
  DURATION = pulseIn(ECO, HIGH);
  DISTANCIA = DURATION / 58.2;
  //esperar(200);

  //distancia permitida para encender el led
  if (DISTANCIA <= 70 && DISTANCIA >= 5) {
    digitalWrite(LED, HIGH);
    digitalWrite(LED2, HIGH);
    Serial.println("objeto detectado");
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(LED2, LOW);
    Serial.println("objeto no detectado");
  }
  */

  Wire.beginTransmission(SLAVE_ADDRESS);  // Iniciar transmisión con el esclavo
  Serial.println("eererer");
  Wire.write(2);
  if (!inSubMenu) {
    UPandDOWN();
  } else if (!inColorMenu) {
    filterMenu();
  } else {
    historyMenu();
  }
  Wire.endTransmission();  // Finalizar transmisión
  //esperar(1000);             // Espera
}
