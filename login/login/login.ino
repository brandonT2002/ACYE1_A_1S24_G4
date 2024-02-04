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


void loop() {
  sistema();
  delay(100);
}
