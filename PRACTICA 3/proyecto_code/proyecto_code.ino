#include <SoftwareSerial.h>
#include <Servo.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
//const int pinPWM = 22;

#define DHTPIN 22       // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT que estás usando
const int pinPWM = 10;
int SENSOR = 6;

DHT dht(DHTPIN, DHTTYPE);

const int segmentPinsDisplay1[4] = { 22, 23, 24, 25 };  // A, B, C, D para el primer display
const int segmentPinsDisplay2[4] = { 26, 27, 28, 29 };  // A, B, C, D para el segundo display

const int sensorPin = A0;  // Pin analógico al que está conectado el sensor de humedad de suelo
int sensorValue = 0;       // Variable para almacenar el valor leído del sensor
int mq2 = A9;

// Define los pines de los LEDs
const int ledPin1 = 48;
const int ledPin2 = 49;
const int ledPin3 = 50;
const int ledPin4 = 51;
const int ledPin5 = 52;
const int ledPin6 = 53;

//const int pinPWM = 22;
// Define el pin para la bomba de agua
const int bombAguaPin = 12;

// Define el pin para los ventiladores
const int funPin = 10;

// Define para el servomotor
Servo myservo1;
const int pinServo1 = 2;

// Configura los pines RX y TX del módulo Bluetooth
const int bluetoothTx = 10;
const int bluetoothRx = 11;

// Definir los pines para el sensor de efecto Hall y el LED
const int hallSensorPin = 13;

const int hallSensorPin = 46;


// Inicializa la biblioteca SoftwareSerial para la comunicación Bluetooth
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

//define el codigo para la iluminación
int VALOR;                     // almacena valor leido de entrada A0
int selec = 11, btnMode = 12;  //
bool selecOp, selecMode;

enum estadoLCD {
  ILUMINACION_ON,
  ILUMINACION_OFF,
  VENTILADORES_ON,
  VENTILADORES_OFF,
  REGANDO,
  ABRIENDO_PUERTA,
  CERRANDO_PUERTA,
  ENTRADA,
  ILUMINACION_MANUAL,
  SENSOR_PROPANO_ON,
};

estadoLCD estado = ILUMINACION_ON;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void sensorHSuelo() {
  sensorValue = analogRead(sensorPin);  // Lee el valor del sensor

  int humedad = map(sensorValue, 0, 314, 0, 100);  // Convierte el valor leído a un porcentaje

  Serial.print("Humedad del suelo: ");
  Serial.print(humedad);
  Serial.println("%");

  if (humedad < 70) {
    digitalWrite(bombAguaPin, HIGH);
    //cambiarEstado(REGANDO);
  } else {
    digitalWrite(bombAguaPin, LOW);
  }
  // digitalWrite(bombAguaPin, HIGH);
}

void sensorMQ2() {
  int mq2Value = analogRead(mq2);
  Serial.println(mq2Value);
  if (mq2Value >= 600) {
    cambiarEstado(SENSOR_PROPANO_ON);
    analogWrite(pinPWM, 200);
  }
}

void encendidoAutomatico() {
  Serial.println(VALOR);
  if (VALOR == LOW) {
    cambiarEstado(ILUMINACION_OFF);
    digitalWrite(ledPin1, LOW);  // Enciende el LED
    digitalWrite(ledPin2, LOW);  // Enciende el LED
    digitalWrite(ledPin3, LOW);  // Enciende el LED

    delay(100);  // Espera 1 segundo
  } else {
    cambiarEstado(ILUMINACION_ON);
    digitalWrite(ledPin1, HIGH);  // Enciende el LED
    digitalWrite(ledPin2, HIGH);  // Enciende el LED
    digitalWrite(ledPin3, HIGH);  // Enciende el LED
  }
}

void encendidoManual() {
  cambiarEstado(ILUMINACION_MANUAL);
  if (selecMode == 0) {
    digitalWrite(ledPin1, HIGH);
  } else {
    digitalWrite(ledPin1, LOW);
  }
}

void iluminacion() {
  //Serial.println(selecOp);
  if (selecOp == 0) {
    encendidoManual();
  } else {
    encendidoAutomatico();
  }
  delay(100);
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
  if (t >= 27.0) {
    digitalWrite(pinPWM, 1);
    cambiarEstado(VENTILADORES_ON);
  } else {
    digitalWrite(pinPWM, 0);
    cambiarEstado(VENTILADORES_OFF);
  }
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



int targetPosition = 0;
int currentPosition = 0;
int pos = 0;

unsigned long previousTime = 0;  // Variable para almacenar el tiempo anterior
bool isOpening = false;          // Variable para controlar si la puerta se está abriendo

void hallSensor() {
  if (digitalRead(hallSensorPin) == LOW) {  // Si el sensor detecta un imán
    isOpening = true;                       // Indica que la puerta se está abriendo
    pos = 120;                              // Posición de la puerta abierta
    previousTime = millis();                // Almacena el tiempo actual
    cambiarEstado(ABRIENDO_PUERTA);
  }
}



void setup() {
  // Configurar el pin del sensor de efecto Hall como entrada
  pinMode(hallSensorPin, INPUT);
  pinMode(mq2, INPUT);
  //configura servomotor
  myservo1.attach(pinServo1);
  lcd.init();
  lcd.backlight();
  // Configura los pines de los LEDs como salidas
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(bombAguaPin, OUTPUT);

  // Configura el pin del sensor de humedad de suelo como salida
  pinMode(segmentPinsDisplay1[0], OUTPUT);  //A
  pinMode(segmentPinsDisplay1[1], OUTPUT);  //B
  pinMode(segmentPinsDisplay1[2], OUTPUT);  //C
  pinMode(segmentPinsDisplay1[3], OUTPUT);  //D


  pinMode(segmentPinsDisplay2[0], OUTPUT);  //A
  pinMode(segmentPinsDisplay2[1], OUTPUT);  //B
  pinMode(segmentPinsDisplay2[2], OUTPUT);  //C
  pinMode(segmentPinsDisplay2[3], OUTPUT);  //D
  // Inicia la comunicación serial para el Bluetooth a 9600 baudios
  bluetooth.begin(9600);
  Serial.begin(9600);
  dht.begin();
}

void cambiarEstado(estadoLCD nuevoEstado) {
  estado = nuevoEstado;
  // Dependiendo del estado actual del LED, realiza diferentes acciones
  switch (estado) {
    case ILUMINACION_ON:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Iluminacion");
      lcd.setCursor(0, 1);
      lcd.print("Activada");
      break;
    case ILUMINACION_OFF:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Iluminacion");
      lcd.setCursor(0, 1);
      lcd.print("Desactivada");
      break;
    case VENTILADORES_ON:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ventiladores");
      lcd.setCursor(0, 1);
      lcd.print("Activados");
      break;
    case VENTILADORES_OFF:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ventiladores");
      lcd.setCursor(0, 1);
      lcd.print("Desactivados");
      break;
    case REGANDO:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Regando");
      break;
    case ABRIENDO_PUERTA:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Abriendo Puerta");
      break;
    case CERRANDO_PUERTA:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cerrando Puerta");
      break;
    case ENTRADA:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Entrada");
      break;
    case ILUMINACION_MANUAL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Iluminacion");
      lcd.setCursor(0, 1);
      lcd.print("manual");
      break;
    case SENSOR_PROPANO_ON:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Propano");
      lcd.setCursor(0, 1);
      lcd.print("Encendido");
      break;
  }
}

void loop() {
  if (isOpening) {
    unsigned long currentTime = millis();       // Obtiene el tiempo actual
    if (currentTime - previousTime >= 25000) {  // Si han pasado 25 segundos
      isOpening = false;                        // Indica que la puerta ha terminado de abrirse
      pos = 0;                                  // Posición de la puerta cerrada
    }
  }

  digitalWrite(ledPin1, LOW);
  VALOR = digitalRead(SENSOR);  // lee valor de entrada A0
  selecOp = 1;
  selecMode = digitalRead(btnMode);
  iluminacion();

  myservo1.writeMicroseconds(map(pos, 0, 180, 1000, 2000));
  sensorDHT11();   // sensor de humedad y temperatura de la casa
  sensorHSuelo();  // sensor de humedad del suelo, patio
  hallSensor();    // sensor de efecto hall para el servomotor

  Bluetooth();  // se reciben datos por Bluetooth
  Serial.println("-----------");
  delay(1000);  // Espera 1 segundos entre mediciones
}
