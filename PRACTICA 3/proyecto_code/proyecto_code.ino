#include <SoftwareSerial.h>
#include <Servo.h>
#include <DHT.h>

#define DHTPIN 2       // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT que estás usando

DHT dht(DHTPIN, DHTTYPE);

const int sensorPin = A0;  // Pin analógico al que está conectado el sensor de humedad de suelo
int sensorValue = 0;       // Variable para almacenar el valor leído del sensor


// Define los pines de los LEDs
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 5;
const int ledPin4 = 6;
const int ledPin5 = 7;
const int ledPin6 = 8;

// Define el pin para la bomba de agua
const int bombAguaPin = 9;

// Define el pin para los ventiladores
const int funPin = 10;

// Define para el servomotor
Servo myservo1;
const int pinServo1 = 11;

// Configura los pines RX y TX del módulo Bluetooth
const int bluetoothTx = 10;
const int bluetoothRx = 11;

// Definir los pines para el sensor de efecto Hall y el LED
const int hallSensorPin = 12;

// Inicializa la biblioteca SoftwareSerial para la comunicación Bluetooth
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void sensorHSuelo() {
  sensorValue = analogRead(sensorPin);  // Lee el valor del sensor

  int humedad = map(sensorValue, 0, 314, 0, 100);  // Convierte el valor leído a un porcentaje

  Serial.print("Humedad del suelo: ");
  Serial.print(humedad);
  Serial.println("%");

  if (humedad < 70) {
    digitalWrite(bombAguaPin, HIGH);
  } else {
    digitalWrite(bombAguaPin, LOW);
  }
}

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

void hallSensor() {
  // Leer el estado del sensor de efecto Hall
  int hallState = digitalRead(hallSensorPin);

  // Si se detecta un campo magnético
  if (hallState == HIGH) {
    myservo1.write(100);
  } else {
    // Si no se detecta un campo magnético
    myservo1.write(10);
  }
}

void setup() {
  // Configurar el pin del sensor de efecto Hall como entrada
  pinMode(hallSensorPin, INPUT);

  //configura servomotor
  myservo1.attach(pinServo1);

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
  sensorHSuelo(); // sensor de humedad del suelo, patio
  hallSensor(); // sensor de efecto hall para el servomotor
  Bluetooth(); // se reciben datos por Bluetooth 
  delay(1000);  // Espera 1 segundos entre mediciones
}
