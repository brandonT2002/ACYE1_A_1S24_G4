#include <Keypad.h>

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
  Serial.println("MODO CALCULADORA");
  
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
  }
}


void cambiarClave() {
  Serial.println("Ingrese la nueva clave:");

  INDICE = 0;         // Reiniciar el índice antes de ingresar la nueva clave
  int longitud = 0;   // Variable para almacenar la longitud de la clave ingresada

  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA) {
      if (TECLA == '#') { 
        if (longitud != 6 && longitud != 8) {
          Serial.println("\n La clave debe tener 6 u 8 caracteres. Intente de nuevo.");
          modoSeguridad();
          return;
        }

        // Verificar patron para clave de 6 
        if (longitud == 6 &&
            !((isdigit(CLAVE_DEFECTO[0]) && isalpha(CLAVE_DEFECTO[1]) && 
               isdigit(CLAVE_DEFECTO[2]) && isalpha(CLAVE_DEFECTO[3]) && 
               isdigit(CLAVE_DEFECTO[4]) && isdigit(CLAVE_DEFECTO[5])) ||
              (isalpha(CLAVE_DEFECTO[0]) && isdigit(CLAVE_DEFECTO[1]) &&
               isalpha(CLAVE_DEFECTO[2]) && isdigit(CLAVE_DEFECTO[3]) && 
               isalpha(CLAVE_DEFECTO[4]) && isdigit(CLAVE_DEFECTO[5])))) {
          Serial.println("\n Patron incorrecto para clave de 6 caracteres.");
          modoSeguridad();
          return;
        }

        // Verificar patron para clave de 8
        if (longitud == 8 &&
            !((isalpha(CLAVE_DEFECTO[0]) && isdigit(CLAVE_DEFECTO[1]) && 
               isalpha(CLAVE_DEFECTO[2]) && isalpha(CLAVE_DEFECTO[3]) && 
               isdigit(CLAVE_DEFECTO[4]) && isdigit(CLAVE_DEFECTO[5]) &&
               isalpha(CLAVE_DEFECTO[6]) && isdigit(CLAVE_DEFECTO[7])) ||
              (isdigit(CLAVE_DEFECTO[0]) && isalpha(CLAVE_DEFECTO[1]) &&
               isdigit(CLAVE_DEFECTO[2]) && isdigit(CLAVE_DEFECTO[3]) && 
               isalpha(CLAVE_DEFECTO[4]) && isalpha(CLAVE_DEFECTO[5]) &&
               isdigit(CLAVE_DEFECTO[6]) && isalpha(CLAVE_DEFECTO[7])))) {
          Serial.println("\n Patron incorrecto para clave de 8 caracteres.");
          modoSeguridad();
          return;
        }

        Serial.println("\n Nueva clave ingresada correctamente.");

        // Esperando la tecla '#' para confirmar el cambio
        while (teclado1.getKey() != '#') {
          delay(50);
        }

        Serial.println(" Cambio de clave completado.");
        delay(300); 
         
        return;  
      } else {
        CLAVE_DEFECTO[INDICE] = TECLA;
        INDICE++;
        Serial.print(TECLA);
        longitud++;
      }
    }
  }
}



void modoSeguridad() {
  Serial.println("MODO SEGURIDAD");

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
  }
}



void sistema() {
  static bool claveCorrecta = false;

  Serial.println(" Ingrese password");

  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA == '#') {
      CLAVE[INDICE] = '\0';

      if (strcmp(CLAVE, CLAVE_DEFECTO) == 0) {
        // Contraseña correcta
        claveCorrecta = true;
        Serial.println(" Correcta");
        intentosIncorrectos = 0;  // Restablecer el contador en caso de éxito
        encenderLEDs(intentosIncorrectos);
        modoSeguridad();
      } else {// Contraseña incorrecta
        Serial.println(" Incorrecta");
        intentosIncorrectos++;
        encenderLEDs(intentosIncorrectos);

        if (intentosIncorrectos >= maxIntentos) {// bloquear sistema
          Serial.println("Demasiados intentos incorrectos. Bloqueando...");
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
      Serial.print(TECLA);
    }
  }
}


void loop() {
  sistema();
  delay(100);
}
