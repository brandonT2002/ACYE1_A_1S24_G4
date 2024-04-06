#include <SoftwareSerial.h>
#include <DHT.h>
// Define los pines de los LEDs
const int ledPin1 = 48;
const int ledPin2 = 49;
const int ledPin3 = 50;
const int ledPin4 = 51;
const int ledPin5 = 52;
const int ledPin6 = 53;
char modeLight = 'S';
// Configura los pines RX y TX del módulo Bluetooth
const int bluetoothTx = 10;
const int bluetoothRx = 11;
int SENSOR = 6;
int VALOR; 
const int pinPWM = 10;
int mq2 = A9;

void sensorMQ2() {
  int mq2Value = analogRead(mq2);
  Serial.println(mq2Value);
  Serial.println(mq2Value);
  if (mq2Value >= 600) {
    // cambiarEstado(SENSOR_PROPANO_ON);
    analogWrite(pinPWM, 200);
  }
}

#define DHTPIN 22       // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT que estás usando
// const int pinPWM = 10;
DHT dht(DHTPIN, DHTTYPE);

bool selecOp, selecMode;

const int segmentPinsDisplay1[4] = { 22, 23, 24, 25 };  // A, B, C, D para el primer display
const int segmentPinsDisplay2[4] = { 26, 27, 28, 29 };  // A, B, C, D para el segundo display
// const int pinPWM = 10;
unsigned long tiempoPrevio = 0;  // Variable para almacenar el último momento en que se ejecutó el código
const unsigned long intervalo = 3000;  // Intervalo deseado (30 segundos)

// void sensorMQ2() {
//   int mq2Value = analogRead(mq2);
//   // Serial.println(mq2Value);
//   if (mq2Value >= 600) {
//     // cambiarEstado(SENSOR_PROPANO_ON);
//     analogWrite(pinPWM, 200);
//   }
// }


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

    delay(10);  // Espera 1 segundo
  } else {
    // cambiarEstado(ILUMINACION_ON);
    digitalWrite(ledPin1, HIGH);  // Enciende el LED
    digitalWrite(ledPin2, HIGH);  // Enciende el LED
    digitalWrite(ledPin3, HIGH);  // Enciende el LED
  }
}

 
void encendidoManual() {
  // cambiarEstado(ILUMINACION_MANUAL);
  switch (modeLight) {
      case '1':
        digitalWrite(ledPin1, HIGH);
        break;
      case '2':
        digitalWrite(ledPin2, HIGH);
        break;
      case '3':
        digitalWrite(ledPin3, HIGH);
        break;
      case '4':
        digitalWrite(ledPin4, HIGH);
        break;
        case '5':
        digitalWrite(ledPin5, HIGH);
        break;
        case '6':
        digitalWrite(ledPin6, HIGH);
        break;
        case '7':
        digitalWrite(ledPin1, LOW);
        break;
        case '8':
        digitalWrite(ledPin2, LOW);
        break;
        case '9':
        digitalWrite(ledPin3, LOW);
        break;
        case 'A':
        digitalWrite(ledPin4, LOW);
        break;
        case 'B':
        digitalWrite(ledPin5, LOW);
        break;
        case 'C':
        digitalWrite(ledPin6, LOW);
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
      default:
        // No hace nada si se recibe un carácter no reconocido
        break;
    }
  }

  void sensorDHT11() {
  // float h = dht.readHumidity();     // Lee la humedad relativa
  float t = dht.readTemperature();  // Lee la temperatura en grados Celsius

  if ( isnan(t)) {
    Serial.println("Error al leer del sensor DHT");
    return;
  }
  // aqui se agrega para enviar por Bluetooth los datos en lugar de solo imprimirlos en consola.
  // Serial.print("Humedad: ");
  // Serial.print(h);
  Serial.print("%\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  if (t >= 27.0) {
  //   digitalWrite(pinPWM, 1);
  //   cambiarEstado(VENTILADORES_ON);
  // } else {
  //   digitalWrite(pinPWM, 0);
  //   cambiarEstado(VENTILADORES_OFF);
  // }
  Serial.println("°C");

  // Redondear y convertir a entero, limitando a 2 dígitos
  int tempInt = min(abs(round(t)), 99);

  // Separar los dígitos
  int decenas = tempInt / 10;
  int unidades = tempInt % 10;

    switch (unidades) {
    case 0:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], LOW);
      digitalWrite(segmentPinsDisplay1[2], LOW);
      digitalWrite(segmentPinsDisplay1[3], LOW);
      break;
    case 1:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], LOW);
      digitalWrite(segmentPinsDisplay1[2], LOW);
      digitalWrite(segmentPinsDisplay1[3], HIGH);
      break;
    case 2:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], LOW);
      digitalWrite(segmentPinsDisplay1[2], HIGH);
      digitalWrite(segmentPinsDisplay1[3], LOW);
      break;
    case 3:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], LOW);
      digitalWrite(segmentPinsDisplay1[2], HIGH);
      digitalWrite(segmentPinsDisplay1[3], HIGH);
      break;
    case 4:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], HIGH);
      digitalWrite(segmentPinsDisplay1[2], LOW);
      digitalWrite(segmentPinsDisplay1[3], LOW);
      break;
    case 5:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], HIGH);
      digitalWrite(segmentPinsDisplay1[2], LOW);
      digitalWrite(segmentPinsDisplay1[3], HIGH);
      break;
    case 6:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], HIGH);
      digitalWrite(segmentPinsDisplay1[2], HIGH);
      digitalWrite(segmentPinsDisplay1[3], LOW);
      break;
    case 7:
      digitalWrite(segmentPinsDisplay1[0], LOW);
      digitalWrite(segmentPinsDisplay1[1], HIGH);
      digitalWrite(segmentPinsDisplay1[2], HIGH);
      digitalWrite(segmentPinsDisplay1[3], HIGH);
      break;
    case 8:
      digitalWrite(segmentPinsDisplay1[0], HIGH);
      digitalWrite(segmentPinsDisplay1[1], LOW);
      digitalWrite(segmentPinsDisplay1[2], LOW);
      digitalWrite(segmentPinsDisplay1[3], LOW);
      break;
    case 9:
      digitalWrite(segmentPinsDisplay1[0], HIGH);
      digitalWrite(segmentPinsDisplay1[1], LOW);
      digitalWrite(segmentPinsDisplay1[2], LOW);
      digitalWrite(segmentPinsDisplay1[3], HIGH);
      break;
  }

  switch (decenas) {
    case 0:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], LOW);
      digitalWrite(segmentPinsDisplay2[2], LOW);
      digitalWrite(segmentPinsDisplay2[3], LOW);
      break;
    case 1:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], LOW);
      digitalWrite(segmentPinsDisplay2[2], LOW);
      digitalWrite(segmentPinsDisplay2[3], HIGH);
      break;
    case 2:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], LOW);
      digitalWrite(segmentPinsDisplay2[2], HIGH);
      digitalWrite(segmentPinsDisplay2[3], LOW);
      break;
    case 3:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], LOW);
      digitalWrite(segmentPinsDisplay2[2], HIGH);
      digitalWrite(segmentPinsDisplay2[3], HIGH);
      break;
    case 4:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], HIGH);
      digitalWrite(segmentPinsDisplay2[2], LOW);
      digitalWrite(segmentPinsDisplay2[3], LOW);
      break;
    case 5:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], HIGH);
      digitalWrite(segmentPinsDisplay2[2], LOW);
      digitalWrite(segmentPinsDisplay2[3], HIGH);
      break;
    case 6:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], HIGH);
      digitalWrite(segmentPinsDisplay2[2], HIGH);
      digitalWrite(segmentPinsDisplay2[3], LOW);
      break;
    case 7:
      digitalWrite(segmentPinsDisplay2[0], LOW);
      digitalWrite(segmentPinsDisplay2[1], HIGH);
      digitalWrite(segmentPinsDisplay2[2], HIGH);
      digitalWrite(segmentPinsDisplay2[3], HIGH);
      break;
    case 8:
      digitalWrite(segmentPinsDisplay2[0], HIGH);
      digitalWrite(segmentPinsDisplay2[1], LOW);
      digitalWrite(segmentPinsDisplay2[2], LOW);
      digitalWrite(segmentPinsDisplay2[3], LOW);
      break;
    case 9:
      digitalWrite(segmentPinsDisplay2[0], HIGH);
      digitalWrite(segmentPinsDisplay2[1], LOW);
      digitalWrite(segmentPinsDisplay2[2], LOW);
      digitalWrite(segmentPinsDisplay2[3], HIGH);
      break;
  }
}

  }

void iluminacion() {
  //Serial.println(selecOp);
  if (selecOp == 0) {
    encendidoManual();
  } else {
    encendidoAutomatico();
  }
  delay(10);
}

void Bluetooth() {
  // Comprueba si hay datos disponibles para leer desde el Bluetooth
  if (bluetooth.available()) {
    char receivedChar = bluetooth.read();  // Lee el carácter recibido
    // Serial.println(receivedChar);
    // Enciende o apaga el LED correspondiente según el carácter recibido
   switch (receivedChar) {
      case 'S':
        selecOp=1;
        break;
      case 'M':
        selecOp = 0;
        break;
      default:
        modeLight = receivedChar;
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

  pinMode(segmentPinsDisplay1[0], OUTPUT);  //A
  pinMode(segmentPinsDisplay1[1], OUTPUT);  //B
  pinMode(segmentPinsDisplay1[2], OUTPUT);  //C
  pinMode(segmentPinsDisplay1[3], OUTPUT);  //D


  pinMode(segmentPinsDisplay2[0], OUTPUT);  //A
  pinMode(segmentPinsDisplay2[1], OUTPUT);  //B
  pinMode(segmentPinsDisplay2[2], OUTPUT);  //C
  pinMode(segmentPinsDisplay2[3], OUTPUT);  //D

  pinMode(mq2, INPUT);  
  selecOp= 1;
  // Inicia la comunicación serial para el Bluetooth a 9600 baudios
  bluetooth.begin(9600);
  Serial.begin(9600);
}

void loop() {
  VALOR = digitalRead(SENSOR); 
  Bluetooth();
  iluminacion();
   unsigned long tiempoActual = millis();  // Obtener el tiempo actual
  if (tiempoActual - tiempoPrevio >= intervalo) {  // Verificar si han pasado 30 segundos
    tiempoPrevio = tiempoActual;  // Actualizar el último momento en que se ejecutó el código
    sensorDHT11();
    // Coloca aquí el código que deseas ejecutar cada 30 segundos
  }
  sensorMQ2();
}