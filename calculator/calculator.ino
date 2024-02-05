#include <Keypad.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "Calculadora.h"

const byte filas = 4;
const byte columnas = 4;

const char tecla[filas][columnas] = {
  { '7', '8', '9', '+' },
  { '4', '5', '6', '-' },
  { '1', '2', '3', '*' },
  { '$', '0', '#', '/' }
};

byte pinFilas[filas] = { 18, 19, 20, 21 };        //conectarse a las patillas de salida de fila del
byte pinColumnas[columnas] = { 14, 15, 16, 17 };  //conectarse a las patillas de las
Keypad keypad = Keypad(makeKeymap(tecla), pinFilas, pinColumnas, filas, columnas);

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEV 3
#define CLK 8
#define DATA 6
#define CS 5
#define BUF_SIZE 90
#define BUFFER_SIZE 5
// Variable que controla la matriz
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA, CLK, CS, MAX_DEV);
char curMessage[BUF_SIZE] = { "" };
char buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;
// temporizador
unsigned long displayStartTime = 0;
const unsigned long displayDuration = 15000;
// direccion
const byte switchPin = 13;
bool scrollRight = true;
// animacion
const byte switchPin2 = 12;
// velocidad
int potentiometerPin = A0;
int potValue = 0;

String printBuffer() {
  String aux = "";
  for (int i = head; i != tail; i = (i + 1) % BUFFER_SIZE) {
    // P.print(buffer[i]);
    aux += buffer[i];
  }
  return aux;
}

void addCharToBuffer(char c) {
  buffer[tail] = c;
  tail = (tail + 1) % BUFFER_SIZE;
  if (tail == head) {
    head = (head + 1) % BUFFER_SIZE;  // Descartar el caracter más antiguo si el buffer está lleno
  }
}

void clearBuffer() {
  head = 0;
  tail = 0;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = '\0';  // Llena el buffer con caracteres vacíos
  }
}

void displayScrollingText(const char* text, bool scrollRight, bool animation_, int speed_) {
  displayStartTime = millis();
  int scrollDirection = (scrollRight) ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT;
  P.setSpeed(speed_);

  while (millis() - displayStartTime < displayDuration) {
    P.displayClear();
    if (animation_) {
      animation(text, animation_, scrollDirection, speed_);
    } else {
      P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
    }
    while (!P.displayAnimate()) {
      if (millis() - displayStartTime >= displayDuration) {
        break;
      }
    }
  }

  memset(curMessage, 0, sizeof(curMessage));
  clearBuffer();
  P.displayClear();
  P.displayReset();
}

void animation(const char* text, bool animation_, int scrollDirection, int speed_) {
  if (animation_) {
    int indice = random(1, 22);

    P.setSpeed(speed_);
    switch (indice) {
      case 1:
        P.displayText(text, PA_LEFT, 50, 0, PA_PRINT & scrollDirection);
        break;

      case 2:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_UP & scrollDirection);
        break;

      case 3:
        P.displayText(text, PA_LEFT, 50, 0, PA_SLICE & scrollDirection);
        break;

      case 4:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCAN_HORIZ & scrollDirection);
        break;

      case 5:
        P.displayText(text, PA_LEFT, 50, 0, PA_OPENING_CURSOR & scrollDirection);
        break;

      case 6:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_DOWN_RIGHT & scrollDirection);
        break;

      case 7:
        P.displayText(text, PA_LEFT, 50, 0, PA_WIPE & scrollDirection);
        break;

      case 8:
        P.displayText(text, PA_LEFT, 50, 0, PA_GROW_UP & scrollDirection);
        break;

      case 9:
        P.displayText(text, PA_LEFT, 50, 0, PA_CLOSING_CURSOR & scrollDirection);
        break;

      case 10:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_UP_LEFT & scrollDirection);
        break;

      case 11:
        P.displayText(text, PA_LEFT, 50, 0, PA_MESH & scrollDirection);
        break;

      case 12:
        P.displayText(text, PA_LEFT, 50, 0, PA_OPENING & scrollDirection);
        break;

      case 13:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_UP_RIGHT & scrollDirection);
        break;

      case 14:
        P.displayText(text, PA_LEFT, 50, 0, PA_BLINDS & scrollDirection);
        break;

      case 15:
        P.displayText(text, PA_LEFT, 50, 0, PA_DISSOLVE & scrollDirection);
        break;

      case 16:
        P.displayText(text, PA_LEFT, 50, 0, PA_CLOSING & scrollDirection);
        break;

      case 17:
        P.displayText(text, PA_LEFT, 50, 0, PA_RANDOM & scrollDirection);
        break;

      case 18:
        P.displayText(text, PA_LEFT, 50, 0, PA_WIPE_CURSOR & scrollDirection);
        break;

      case 19:
        P.displayText(text, PA_LEFT, 50, 0, PA_GROW_DOWN & scrollDirection);
        break;

      case 20:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCAN_VERT & scrollDirection);
        break;

      case 21:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_DOWN_LEFT & scrollDirection);
        break;

      default:
        break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  P.begin();

  pinMode(switchPin, INPUT);
  pinMode(switchPin2, INPUT);

  pinMode(potentiometerPin, INPUT);
}

void loop() {
  P.displayAnimate();

  // Leer el estado del switch
  bool switchState = digitalRead(switchPin);
  bool switch2State = digitalRead(switchPin2);

  potValue = analogRead(potentiometerPin);

  // Ajustar la velocidad de la animación en función del valor del potenciómetro
  int animationSpeed = 100 - map(potValue, 0, 1023, 10, 100);

  static String input = "";
  char key = keypad.getKey();
  if (key) {
    if (isdigit(key)) {
      input += key;
      addCharToBuffer(key);
      String a = printBuffer();
      // P.setCharSpacing(P.getCharSpacing() - 1);
      P.print(a.c_str());
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
      input += key;
      addCharToBuffer(key);
      String a = printBuffer();
      P.print(a.c_str());
    } else if (key == '#') {
      Serial.println("-> Calculando");
      Calculadora calc;
      String resultado = calc.calcular(input);
      strcpy(curMessage, resultado.c_str());
      P.displayClear();
      P.displayReset();
      // P.print(curMessage);
      Serial.println("-> Mostrando Resultado");
      displayScrollingText(curMessage, switchState, switch2State, animationSpeed);
      input = "";
      resultado = 0.0;
       Serial.println("-> Esperando Operacion");
    } else if (key == '$') {
      input = "";
      clearBuffer();
      P.displayClear();
      P.displayReset();
    }
  }

  static bool lastSwitchState = switchState;
  if (switchState != lastSwitchState) {
    P.displayReset();  // Reiniciar la animación para un cambio inmediato
    lastSwitchState = switchState;
  }
}
