#include <Wire.h>

#define SLAVE_ADDRESS 9 // Dirección del esclavo

int numToSend = 2;  // Número a enviar al esclavo (puede ser cualquier número)

void setup() {
  Wire.begin(); // Inicializar el bus I2C
  Serial.begin(9600); // Inicializar la comunicación serial
}

void loop() {
  Wire.beginTransmission(SLAVE_ADDRESS); // Iniciar transmisión con el esclavo
  Wire.write(numToSend); // Enviar el número al esclavo
  Wire.endTransmission(); // Finalizar transmisión
  delay(1000); // Esperar un segundo antes de enviar el próximo número
}
