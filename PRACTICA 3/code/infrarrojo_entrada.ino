const int sensorEntrada = 7;  //grande

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorEntrada, INPUT);
  Serial.begin(9600);
}

void entrada(){
  if (entradaValue == LOW) {
    Serial.println("Enviando notificación");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  entradaValue = digitalRead(sensorEntrada);
  entrada();
}
