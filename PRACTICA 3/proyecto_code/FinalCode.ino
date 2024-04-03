#include <SoftwareSerial.h>

// Define los pines de los LEDs
const int ledPin1 = 48;
const int ledPin2 = 49;
const int ledPin3 = 50;
const int ledPin4 = 51;
const int ledPin5 = 52;
const int ledPin6 = 53;

// Configura los pines RX y TX del módulo Bluetooth
const int bluetoothTx = 10;
const int bluetoothRx = 11;
int SENSOR = 6;
int VALOR; 
bool selecOp, selecMode;

// Inicializa la biblioteca SoftwareSerial para la comunicación Bluetooth
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);


void encendidoAutomatico() {
  // digitalWrite(ledPin1, 1);
  Serial.println(VALOR);
  if (VALOR == LOW) {
    // cambiarEstado(ILUMINACION_OFF);
    digitalWrite(ledPin1, LOW);  // Enciende el LED
    digitalWrite(ledPin2, LOW);  // Enciende el LED
    digitalWrite(ledPin3, LOW);  // Enciende el LED

    delay(100);  // Espera 1 segundo
  } else {
    // cambiarEstado(ILUMINACION_ON);
    digitalWrite(ledPin1, HIGH);  // Enciende el LED
    digitalWrite(ledPin2, HIGH);  // Enciende el LED
    digitalWrite(ledPin3, HIGH);  // Enciende el LED
  }
}
 
void encendidoManual() {
  // cambiarEstado(ILUMINACION_MANUAL);
  if (selecMode == 0) {
    digitalWrite(ledPin1, HIGH);
  } else {
    digitalWrite(ledPin1, LOW);
  }
}


void iluminacion() {
  //Serial.println(selecOp);
  if (selecOp != 0) {
    encendidoAutomatico();
  }
  delay(100);
}

void Bluetooth() {
  // Comprueba si hay datos disponibles para leer desde el Bluetooth
  if (bluetooth.available()) {
    selecOp = 1;
    char receivedChar = bluetooth.read();  // Lee el carácter recibido

    // Enciende o apaga el LED correspondiente según el carácter recibido
   switch (receivedChar) {
      case '1':
        digitalWrite(ledPin1, !digitalRead(ledPin1));
        break;
      case '2':
        digitalWrite(ledPin2, !digitalRead(ledPin2));
        break;
      case '3':
        digitalWrite(ledPin3, !digitalRead(ledPin3));
        break;
        case '4':
        digitalWrite(ledPin4, !digitalRead(ledPin4));
        break;
        case '5':
        digitalWrite(ledPin5, !digitalRead(ledPin5));
        break;
        case '6':
        digitalWrite(ledPin6, !digitalRead(ledPin6));
        break;
      case 'E':
        // Enciende todas las luces
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        digitalWrite(ledPin4, HIGH);
        digitalWrite(ledPin5, HIGH);
        digitalWrite(ledPin6, HIGH);
        break;
      case 'A':
        // Apaga todas las luces
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
        digitalWrite(ledPin5, LOW);
        digitalWrite(ledPin6, LOW);
        break;
      default:
        // No hace nada si se recibe un carácter no reconocido
        break;
    }
  }
}

void setup() {
  // Configura los pines de los LEDs como salidas
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
iluminacion();
  // Inicia la comunicación serial para el Bluetooth a 9600 baudios
  bluetooth.begin(9600);
  Serial.begin(9600);
}

void loop() {
  VALOR = digitalRead(SENSOR); 
   selecOp = 1;
  Bluetooth();
}