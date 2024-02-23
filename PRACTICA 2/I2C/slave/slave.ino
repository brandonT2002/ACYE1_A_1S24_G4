#include <Wire.h>

#define SLAVE_ADDRESS 9 // Dirección del esclavo

void function1(); // Declaración de función
void paradaEmergencia(); // Declaración de función

void setup() {
  Wire.begin(SLAVE_ADDRESS); // Inicializar el esclavo con la dirección especificada
  Wire.onReceive(receiveEvent); // Configurar la función para manejar los datos recibidos
  Serial.begin(9600); // Inicializar la comunicación serial
}

void loop() {
  // El esclavo no necesita ejecutar nada en el loop ya que responderá a las solicitudes del maestro en receiveEvent
}

void receiveEvent(int numBytes) {
  while(Wire.available() > 0) { // Mientras haya datos disponibles para leer
    int numReceived = Wire.read(); // Leer el número enviado por el maestro
    // Ejecutar una función específica basada en el número recibido
    switch(numReceived) {
      case 1:
        function1();
        break;
      case 2:
        paradaEmergencia();
        break;
      case 3:
        function1();
        break;
      // Puedes agregar más casos para ejecutar otras funciones según el número recibido
      default:
        // Manejar caso no reconocido
        break;
    }
  }
}

void function1() {
  Serial.println("Función 1 ejecutada.");
  // Código de la función 1
}

void paradaEmergencia(){
  Serial.println("se realizo una perada de merfgencia.");
  delay(1000);  
}
