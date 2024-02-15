#include <Keypad.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "Calculadora.h"


// =======================================================
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
const unsigned long displayDuration = 30000;
// direccion
const byte switchPin = 13;
bool scrollRight = true;
// animacion
const byte switchPin2 = 12;
int indice = 0;
static bool unaVez;
// velocidad
int potentiometerPin = A0;
int potValue = 0;
// =======================================================

const byte FILAS = 4; 
const byte COLUMNAS = 4;

char Keys[FILAS][COLUMNAS] = {
  {'7','8','9','A'},
  {'4','5','6','B'},
  {'1','2','3','C'},
  {'*','0','#','D'}
};

byte pin_Filas[FILAS] = {18, 19, 20, 21};
byte pin_columnas[COLUMNAS] = {14, 15, 16, 17};

Keypad teclado1 = Keypad(makeKeymap(Keys), pin_Filas, pin_columnas, FILAS, COLUMNAS);

char TECLA;
char CLAVE[9];
byte INDICE = 0;
char CLAVE_DEFECTO[9] = "AC04C124";
const int push_boton1 = 11; // Para el modo seguridad
const int push_boton2 = 10; // Para el modo calculadora
int intentosIncorrectos = 0;
const int maxIntentos = 3;
unsigned long tiempoBloqueo = 0;
const int ledPin1 = 4;  // Pin del primer LED
const int ledPin2 = 3;  // Pin del segundo LED
const int ledPin3 = 2;  // Pin del tercer LED


void setup() {
  Serial.begin(9600);   
  pinMode(push_boton1, INPUT);
  pinMode(push_boton2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  P.begin();
  pinMode(switchPin, INPUT);
  pinMode(switchPin2, INPUT);
  pinMode(potentiometerPin, INPUT);
}

void encenderLEDs(int cantidad) {
  // Enciende la cantidad de LEDs correspondientes al contador
  digitalWrite(ledPin1, cantidad >= 1 ? HIGH : LOW);
  digitalWrite(ledPin2, cantidad >= 2 ? HIGH : LOW);
  digitalWrite(ledPin3, cantidad == 3 ? HIGH : LOW);
}

void apagarLEDs() {
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
}


void calculadora() {
  Serial.println(" "); 
  Serial.println("Cambiando a modo Calculadora"); //estado
  
  while (digitalRead(push_boton1) == HIGH || digitalRead(push_boton2) == HIGH) {
    delay(50);
  }
  while (true) {
    if (digitalRead(push_boton1) == HIGH) {
      sistema();
      delay(300);
      break;  
    } else if (digitalRead(push_boton2) == HIGH) {
      modoSeguridad();
      delay(300);
      break;  
    }
    // ==========================================
    // ========== FUNCIONALIDAD DE CALCU ==========
    Calculadora_();
  }
}


void cambiarClave() {
  Serial.println(" ");
  Serial.println("Esperando contrasenia, ingrese la nueva clave "); //estado

  INDICE = 0;         // Reiniciar el índice antes de ingresar la nueva clave
  int longitud = 0;   // Variable para almacenar la longitud de la clave ingresada

  // Usar una variable temporal para almacenar la nueva clave
  char nuevaClave[9];

  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA) {
      if (TECLA == '#') {
        if (longitud != 6 && longitud != 8) {
          Serial.println(" La clave debe tener 6 u 8 caracteres. Intente de nuevo.");
          modoSeguridad();
          return;
        }

        // Verificar patron para clave de 6
        if (longitud == 6 &&
            !((isdigit(nuevaClave[0]) && isalpha(nuevaClave[1]) &&
               isdigit(nuevaClave[2]) && isalpha(nuevaClave[3]) &&
               isdigit(nuevaClave[4]) && isdigit(nuevaClave[5])) ||
              (isalpha(nuevaClave[0]) && isdigit(nuevaClave[1]) &&
               isalpha(nuevaClave[2]) && isdigit(nuevaClave[3]) &&
               isalpha(nuevaClave[4]) && isdigit(nuevaClave[5])))) {
          Serial.println(" Patron incorrecto para clave de 6 caracteres.");
          modoSeguridad();
          return;
        }

        // Verificar patron para clave de 8
        if (longitud == 8 &&
            !((isalpha(nuevaClave[0]) && isdigit(nuevaClave[1]) &&
               isalpha(nuevaClave[2]) && isalpha(nuevaClave[3]) &&
               isdigit(nuevaClave[4]) && isdigit(nuevaClave[5]) &&
               isalpha(nuevaClave[6]) && isdigit(nuevaClave[7])) ||
              (isdigit(nuevaClave[0]) && isalpha(nuevaClave[1]) &&
               isdigit(nuevaClave[2]) && isdigit(nuevaClave[3]) &&
               isalpha(nuevaClave[4]) && isalpha(nuevaClave[5]) &&
               isdigit(nuevaClave[6]) && isalpha(nuevaClave[7])))) {
          Serial.println(" Patron incorrecto para clave de 8 caracteres.");
          modoSeguridad();
          return;
        }

        // Actualizar CLAVE_DEFECTO con la nueva clave
        strncpy(CLAVE_DEFECTO, nuevaClave, sizeof(CLAVE_DEFECTO) - 1);
        CLAVE_DEFECTO[sizeof(CLAVE_DEFECTO) - 1] = '\0';

        Serial.println(" Nueva clave ingresada correctamente.");

        // Esperando la tecla '#' para confirmar el cambio
        while (teclado1.getKey() != '#') {
          delay(50);
        }

        Serial.println(" Nueva clave almacenada correctamente."); //estado
        delay(300);

        return;
      } else {
        nuevaClave[INDICE] = TECLA;
        INDICE++;
        Serial.print(TECLA);
        longitud++;
      }
    }
  }
}




void modoSeguridad() {

  Serial.println(" ");
  Serial.println("Cambiando a modo Seguridad"); //estado
  Serial.println("=== ARMADO ==="); //estado

  while (digitalRead(push_boton1) == HIGH || digitalRead(push_boton2) == HIGH) {
    delay(50);
  }
  
  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA) {
      if (TECLA == '*') {
        cambiarClave();
        return;  // Salir de la función si se cambió la clave
      } else {
        Serial.print(TECLA);
      }
    }

    if (digitalRead(push_boton2) == HIGH) {
      calculadora();
      return;
    }
     if (digitalRead(push_boton1) == HIGH) {
      sistema();
      return;
    }
  }
}



void sistema() {
  static bool claveCorrecta = false;
  
  Serial.println(" ");
  Serial.println("Ingrese contrasenia");

  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA == '#') {
      CLAVE[INDICE] = '\0';
      Serial.println(" ");

      if (strcmp(CLAVE, CLAVE_DEFECTO) == 0) {
        // Contraseña correcta
        claveCorrecta = true;
        Serial.println("Validando contrasenia... "); //estado
        Serial.println("La contrasenia se ha ingresado correctamente"); 
        Serial.println(" ");
        Serial.println("Desbloqueado..."); //estado
        intentosIncorrectos = 0;  // Restablecer el contador en caso de éxito
        encenderLEDs(intentosIncorrectos);
        modoSeguridad();
      } else {// Contraseña incorrecta
        Serial.println("Validando contrasenia... "); //estado
        Serial.println("La contrasenia no se ha ingresado correctamente"); 
        intentosIncorrectos++;
        encenderLEDs(intentosIncorrectos);

        if (intentosIncorrectos >= maxIntentos) {// bloquear sistema
          Serial.println(" ");
          Serial.println("Maximo de intentos fallidos");  //estado
          Serial.println("Bloqueando...");
          tiempoBloqueo = millis(); // Establecer el tiempo de bloqueo actual
          while (millis() - tiempoBloqueo < 15000) { // Esperar hasta que pase el tiempo de bloqueo (15 segundos)
            delay(1000); 
          }

          // Restablecer variables y apagar LEDs despues del bloqueo
          intentosIncorrectos = 0;
          apagarLEDs();
          tiempoBloqueo = 0; // Restablecer la variable de tiempo de bloqueo
        }
      }
      
      INDICE = 0;  
      break;       
    } else if (TECLA) {
      CLAVE[INDICE] = TECLA;
      INDICE++;
      Serial.print("*"); //encripta la contrasenia
    }
  }
}


// =======================================================
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

  if(scrollRight) {
    Serial.println("  -> Direccion: Derecha");
  } else {
    Serial.println("  -> Direccion: Izquierda");
  }
  if (animation_) {
    indice = random(1, 22);
  } else {
    Serial.println("  -> Sin efecto");
  }

  while (millis() - displayStartTime < displayDuration) {
    P.displayClear();
    if (animation_) {
      if(scrollRight) {
        animation(text, indice, PA_SCROLL_RIGHT, speed_);
      } else {
        animation(text, indice, PA_SCROLL_LEFT, speed_);
      }
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

void animation(const char* text, int indice, int scrollDirection, int speed_) {
    P.setSpeed(speed_);

    if (!unaVez) {
      switch (indice) {
      case 1:
        Serial.println("  -> PA_PRINT");
        break;

      case 2:
        Serial.println("  -> PA_SCROLL_UP");
        break;

      case 3:
        Serial.println("  -> PA_SLICE");
        break;

      case 4:
        Serial.println("  -> PA_SCAN_HORIZ");
        break;

      case 5:
        Serial.println("  -> PA_OPENING_CURSOR");
        break;

      case 6:
        Serial.println("  -> PA_SCROLL_DOWN_RIGHT");
        break;

      case 7:
        Serial.println("  -> PA_WIPE");
        break;

      case 8:
        Serial.println("  -> PA_GROW_UP");
        break;

      case 9:
        Serial.println("  -> PA_CLOSING_CURSOR");
        break;

      case 10:
        Serial.println("  -> PA_SCROLL_UP_LEFT");
        break;

      case 11:
        Serial.println("  -> PA_MESH");
        break;

      case 12:
        Serial.println("  -> PA_OPENING");
        break;

      case 13:
        Serial.println("  -> PA_SCROLL_UP_RIGHT");
        break;

      case 14:
        Serial.println("  -> PA_BLINDS");
        break;

      case 15:
        Serial.println("  -> PA_DISSOLVE");
        break;

      case 16:
        Serial.println("  -> PA_CLOSING");
        break;

      case 17:
        Serial.println("  -> PA_RANDOM");
        break;

      case 18:
        Serial.println("  -> PA_WIPE_CURSOR");
        break;

      case 19:
        Serial.println("  -> PA_GROW_DOWN");
        break;

      case 20:
        Serial.println("  -> PA_SCAN_VERT");
        break;

      case 21:
        Serial.println("  -> PA_SCROLL_DOWN_LEFT");
        break;

      default:
        break;
      }
      unaVez = true;
    }
    
    switch (indice) {
      case 1:
        P.displayText(text, PA_LEFT, speed_, 0, PA_PRINT);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 2:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SCROLL_UP);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 3:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SLICE);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 4:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SCAN_HORIZ);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 5:
        P.displayText(text, PA_LEFT, speed_, 0, PA_OPENING_CURSOR);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 6:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SCROLL_DOWN_RIGHT);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 7:
        P.displayText(text, PA_LEFT, speed_, 0, PA_WIPE);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 8:
        P.displayText(text, PA_LEFT, speed_, 0, PA_GROW_UP);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 9:
        P.displayText(text, PA_LEFT, speed_, 0, PA_CLOSING_CURSOR);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 10:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SCROLL_UP_LEFT);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 11:
        P.displayText(text, PA_LEFT, speed_, 0, PA_MESH);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 12:
        P.displayText(text, PA_LEFT, speed_, 0, PA_OPENING);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 13:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SCROLL_UP_RIGHT);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 14:
        P.displayText(text, PA_LEFT, speed_, 0, PA_BLINDS);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 15:
        P.displayText(text, PA_LEFT, speed_, 0, PA_DISSOLVE);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 16:
        P.displayText(text, PA_LEFT, speed_, 0, PA_CLOSING);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 17:
        P.displayText(text, PA_LEFT, speed_, 0, PA_RANDOM);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 18:
        P.displayText(text, PA_LEFT, speed_, 0, PA_WIPE_CURSOR);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 19:
        P.displayText(text, PA_LEFT, speed_, 0, PA_GROW_DOWN);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 20:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SCAN_VERT);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      case 21:
        P.displayText(text, PA_LEFT, speed_, 0, PA_SCROLL_DOWN_LEFT);
        //while (!P.displayAnimate()) {}
        //P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
        break;

      default:
        break;
    }
}
// =======================================================


void Calculadora_() {
  P.displayAnimate();

  // Leer el estado del switch
  bool switchState = digitalRead(switchPin);
  bool switch2State = digitalRead(switchPin2);

  potValue = analogRead(potentiometerPin);

  // Ajustar la velocidad de la animación en función del valor del potenciómetro
  int animationSpeed = 100 - map(potValue, 0, 1023, 10, 100);

  static String input = "";
  char key = teclado1.getKey();
  if (key) {
    if (isdigit(key)) {
      input += key;
      addCharToBuffer(key);
      String a = printBuffer();
      // P.setCharSpacing(P.getCharSpacing() - 1);
      P.print(a.c_str());
    } else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      char temp;
      if (key == 'A') {
        temp = '/';
      } else if (key == 'B') {
        temp = '*';
      } else if (key == 'C') {
        temp = '-';
      } else if (key == 'D') {
        temp = '+';
      }
      input += temp;
      addCharToBuffer(temp);
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
      unaVez = false;
      displayScrollingText(curMessage, switchState, switch2State, animationSpeed);
      unaVez = false;
      input = "";
      resultado = 0.0;
       Serial.println("-> Esperando Operacion");
    } else if (key == '*') {
      input = "";
      clearBuffer();
      P.displayClear();
      P.displayReset();
    }
  }
}


void loop() {
  sistema();
  delay(100);
}
