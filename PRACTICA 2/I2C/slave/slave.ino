#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 9                    // Dirección del esclavo
String mensaje = "Hola desde el esclavo";  // El string a enviar

// inicio variables sensor de color
#define outPin 8
#define s0 2
#define s1 3
#define s2 4
#define s3 5

int red, green, blue;
String color = "";
// fin variables sensor de color

// inicio variables sensor de tamano
const int sensorPin = 7;   //grande
const int sensorPin2 = 6;  //mediano
const int sensorPin3 = 5;  //pequeño
// fin variables sensor de tamano

//inicio Variables servo
// creadion del los objetos servo
Servo myservo1;
Servo myservo2;
Servo myservo3;

int deg = 42;  // grados a abrir los servos

// piner para los servos
int pinServo1 = 10;
int pinServo2 = 11;
int pinServo3 = 12;

// Definir las variables para el control del tiempo
unsigned long tiempoInicio1 = 0;
unsigned long tiempoInicio2 = 0;
unsigned long tiempoInicio3 = 0;
const unsigned long tiempoDeseado = 20000;  // Tiempo deseado en milisegundos (2 segundos)
//fin variables servo

void setup() {
  Wire.begin(SLAVE_ADDRESS);     // Inicializar el esclavo con la dirección especificada
  Wire.onReceive(receiveEvent);  // Configurar la función para manejar los datos recibidos
  Wire.onRequest(requestEvent);
  Serial.begin(9600);  // Inicializar la comunicación serial

  Serial.println("fuer");

  //sensor de color
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  //servo
  myservo1.attach(pinServo1);
  myservo2.attach(pinServo2);
  myservo3.attach(pinServo3);
}

void loop() {
  // El esclavo no necesita ejecutar nada en el loop ya que responderá a las solicitudes del maestro en receiveEvent
}


void paradaEmergencia() {
  Serial.println("se realizo una parada de emergencia.");
  delay(30000);
}

void servo(int valor) {
  while (true) {
    // si valor = 1
    if (valor == 1) {
      myservo1.write(deg);
      tiempoInicio1 = millis();  // Almacenar el tiempo actual
      valor++;
    }
    // Si han pasado 2 segundos desde que se presionó el botón 1, volver al estado inicial
    if (millis() - tiempoInicio1 >= tiempoDeseado) {
      myservo1.write(0);
      return;
    }

    // si valor = 2
    if (valor == 2) {
      myservo2.write(deg);
      tiempoInicio2 = millis();  // Almacenar el tiempo actual
      valor++;
    }
    // Si han pasado 2 segundos desde que se presionó el botón 2, volver al estado inicial
    if (millis() - tiempoInicio2 >= tiempoDeseado) {
      myservo2.write(0);
      return;
    }


    // si valor = 3
    if (valor == 3) {
      myservo3.write(deg);
      tiempoInicio3 = millis();  // Almacenar el tiempo actual
      valor++;
    }
    // Si han pasado 2 segundos desde que se presionó el botón 2, volver al estado inicial
    if (millis() - tiempoInicio3 >= tiempoDeseado) {
      myservo3.write(0);
      return;
    }
  }
}

void sensorTamano() {
  while (true) {
    Serial.println("entro funcion");
    int grande = 0;
    int mediano = 0;
    int peque = 0;
    grande = digitalRead(sensorPin);
    mediano = digitalRead(sensorPin2);
    peque = digitalRead(sensorPin3);

    if (grande == LOW) {
      mensaje = "Grande";
      Wire.write(mensaje.c_str());
      servo(1);
      return;
    } else if (mediano == LOW) {
      mensaje = "Mediano";
      Wire.write(mensaje.c_str());
      servo(2);
      return;
    } else if (peque == LOW) {
      mensaje = "Pequeno";
      Wire.write(mensaje.c_str());
      servo(3);
      return;
    }
  }
}

void printData(void) {
  Serial.print("red= ");
  Serial.print(red);
  Serial.print("   green= ");
  Serial.print(green);
  Serial.print("   bluee= ");
  Serial.print(blue);
  Serial.print(" - ");
  Serial.print(color);
  Serial.println(" detected!");
}

void readRGB() {
  red = 0, green = 0, blue = 0;

  int n = 10;
  for (int i = 0; i < n; ++i) {
    //read red component
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    red = red + pulseIn(outPin, LOW);

    //read green component
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    green = green + pulseIn(outPin, LOW);

    //let's read bluee component
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    blue = blue + pulseIn(outPin, LOW);
  }
  red = red / n;
  green = green / n;
  blue = blue / n;
}

void getColor() {
  readRGB();
  if (red > 7 && red < 11 && green > 17 && green < 25 && blue > 13 && blue < 19) {
    mensaje = "RED";
    Wire.write(mensaje.c_str());
    servo(3);
  } else if (red > 22 && red < 28 && green > 11 && green < 16 && blue > 6 && blue < 12) {
    mensaje = "BLUE";
    Wire.write(mensaje.c_str());
    servo(3);
  } else if (red > 3 && red < 8 && green > 4 && green < 10 && blue > 9 && blue < 13) {
    mensaje = "YELLOW";
    Wire.write(mensaje.c_str());
    servo(3);
  } else Serial.println("COLOR NOT FOUND");
}

void requestEvent() {
  Wire.write(mensaje.c_str());  // Envía el string al maestro
}

void receiveEvent(int numBytes) {
  Serial.println("fuera del while");
  while (Wire.available() > 0) {  // Mientras haya datos disponibles para leer
    int numReceived = 0;
    for (int i = 0; i < numBytes; i++) {
      numReceived = numReceived * 256 + Wire.read();  // Leer los bytes recibidos y construir el número
    }
    // Ejecutar una función específica basada en el número recibido
    Serial.println("dentro del while");
    switch (numReceived) {
      case 1:
        getColor();
        break;
      case 2:
        Serial.println("recibio 2");
        sensorTamano();
        break;
      case 3:
        paradaEmergencia();
        break;
      // Puedes agregar más casos para ejecutar otras funciones según el número recibido
      default:
        // Manejar caso no reconocido
        break;
    }
  }
}
