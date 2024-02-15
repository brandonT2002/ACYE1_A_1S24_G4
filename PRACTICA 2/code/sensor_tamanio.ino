const int sensorPin = 9; //grande
const int sensorPin2 = 10; //mediano
const int sensorPin3 = 11; //pequeño

void cajaGrande() {
  // Acciones para la caja grande
  Serial.println("Caja Grande");
}
void cajaMediana() {
  // Acciones para la caja mediana
  Serial.println("Caja Mediana");
}
void cajaPequenia() {
  // Acciones para la caja pequeña
  Serial.println("Caja Pequeña");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //iniciar puerto serie
  pinMode(sensorPin, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
}

void loop() {
  int grande = 0;
  int mediano = 0;
  int peque = 0;
  grande = digitalRead(sensorPin);
  mediano = digitalRead(sensorPin2);
  peque = digitalRead(sensorPin3);

  if (grande == LOW) {
    cajaGrande();
  } else if (mediano == LOW) {
    cajaMediana();
  } else if (peque == LOW) {
    cajaPequenia();
  }

  delay(1500);
}
