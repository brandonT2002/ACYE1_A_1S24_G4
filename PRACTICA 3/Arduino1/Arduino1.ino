#include <SoftwareSerial.h>

// Define los pines de los LEDs
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;

// Configura los pines RX y TX del módulo Bluetooth
const int bluetoothTx = 10;
const int bluetoothRx = 11;

// Inicializa la biblioteca SoftwareSerial para la comunicación Bluetooth
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void Bluetooth() {

  // Comprueba si hay datos disponibles para leer desde el Bluetooth
  if (bluetooth.available()) {
    char receivedChar = bluetooth.read();  // Lee el carácter recibido

    // Enciende o apaga el LED correspondiente según el carácter recibido
    switch (receivedChar) {
      case '1':
        digitalWrite(ledPin1, HIGH);
        break;
      case '2':
        digitalWrite(ledPin1, LOW);
        break;
      case '3':
        digitalWrite(ledPin2, HIGH);
        break;
      case '4':
        digitalWrite(ledPin2, LOW);
        break;
      case '5':
        digitalWrite(ledPin3, HIGH);
        break;
      case '6':
        digitalWrite(ledPin3, LOW);
        break;
      default:
        // Apaga todos los LEDs si se recibe un carácter no reconocido
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        break;
    }
  }
}

void setup() {
  // Configura los pines de los LEDs como salidas
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // Inicia la comunicación serial para el Bluetooth a 9600 baudios
  bluetooth.begin(9600);
  Serial.begin(9600);
}

void loop() {
  Bluetooth();
}
