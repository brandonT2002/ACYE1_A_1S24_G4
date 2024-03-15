#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 2       // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT que estás usando

DHT dht(DHTPIN, DHTTYPE);

// Define los pines de los LEDs
const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;

// Configura los pines RX y TX del módulo Bluetooth
const int bluetoothTx = 10;
const int bluetoothRx = 11;

// Inicializa la biblioteca SoftwareSerial para la comunicación Bluetooth
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void sensorDHT11() {
  float h = dht.readHumidity();     // Lee la humedad relativa
  float t = dht.readTemperature();  // Lee la temperatura en grados Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT");
    return;
  }
  // aqui se agrega para enviar por Bluetooth los datos en lugar de solo imprimirlos en consola.
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("°C");
}

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
  dht.begin();
}

void loop() {
  sensorDHT11(); // sensor de humedad y temperatura de la casa
  Bluetooth(); // se reciben datos por Bluetooth 
  delay(1000);  // Espera 1 segundos entre mediciones
}
