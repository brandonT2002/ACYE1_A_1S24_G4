#include <SoftwareSerial.h>

// Define los pines de los LEDs
const int ledPin1 = 48;
const int ledPin2 = 49;
const int ledPin3 = 50;
const int ledPin4 = 51;
const int ledPin5 = 52;
const int ledPin6 = 53;
char modeLight = 'N';
// Configura los pines RX y TX del módulo Bluetooth
const int bluetoothTx = 10;
const int bluetoothRx = 11;
int SENSOR = 6;
int VALOR; 
bool selecOp, selecMode;

const int segmentPinsDisplay1[4] = { 22, 23, 24, 25 };  // A, B, C, D para el primer display
const int segmentPinsDisplay2[4] = { 26, 27, 28, 29 };  // A, B, C, D para el segundo display

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
    Serial.println(receivedChar);
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
  selecOp= 1;
  // Inicia la comunicación serial para el Bluetooth a 9600 baudios
  bluetooth.begin(9600);
  Serial.begin(9600);
}

void loop() {
  VALOR = digitalRead(SENSOR); 
  Bluetooth();
  iluminacion();
}