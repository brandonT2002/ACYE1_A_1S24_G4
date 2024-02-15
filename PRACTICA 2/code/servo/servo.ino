#include <Servo.h>

Servo myservo;

// Definir las variables para el control del tiempo
unsigned long tiempoInicio1 = 0;
const unsigned long tiempoDeseado = 2000; // Tiempo deseado en milisegundos (2 segundos)

void servo(bool valor)
{
  if (valor)
  { // Si el valor es verdadero, girar a 180 grados
    myservo.write(180);
    tiempoInicio1 = millis();
  }
  else
  { // si el valor es falso, girar a 90 grados
    myservo.write(90);
    tiempoInicio1 = millis();
  }

  while (millis() - tiempoInicio1 < tiempoDeseado)
  {
    // Esperar hasta que se cumpla el tiempo deseado, no quitar 
  }

  myservo.write(0); // regresar el servo en la posición 0
}

void setup()
{
  myservo.attach(10); // pin donde se conecta cable naranja del servo

  servo(true);  // Servo a 180 grados durante x segundos

  servo(false); // Servo a 90 grados durante x segundos
}

void loop()
{
  //sdfsd
}
