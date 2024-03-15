#include <SoftwareSerial.h>
#include <Servo.h>

// Define los pines de los LEDs
const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;

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
        digitalWrite(ledPin1, !digitalRead(ledPin1));
        break;
      case '2':
        digitalWrite(ledPin2, !digitalRead(ledPin2));
        break;
      case '3':
        digitalWrite(ledPin3, !digitalRead(ledPin3));
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

  // Inicia la comunicación serial para el Bluetooth a 9600 baudios
  bluetooth.begin(9600);
  Serial.begin(9600);
}

void loop() {
  Bluetooth(); // se reciben datos por Bluetooth 
  delay(1000);  // Espera 1 segundos entre mediciones
}
