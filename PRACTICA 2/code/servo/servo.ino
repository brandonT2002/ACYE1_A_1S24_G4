
#include <Servo.h>

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
const unsigned long tiempoDeseado = 2000;  // Tiempo deseado en milisegundos (2 segundos)

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

void setup() {
  myservo1.attach(pinServo1);
  myservo2.attach(pinServo2);
  myservo3.attach(pinServo3);


  // se llama a la funcion servo con valor 1,2,3 para mover los respectivos servos.
  servo(1);
}

void loop() {
  //BCSPN
}