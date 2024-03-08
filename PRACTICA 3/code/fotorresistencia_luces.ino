int SENSOR = 0;                // pin S de modulo a entrada analogica A0
int LED = 3;                   // LED en pin 3
int VALOR;                     // almacena valor leido de entrada A0
int selec = 11, btnMode = 12;  //
bool selecOp, selecMode;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(selec, INPUT);
  pinMode(btnMode, INPUT);
  Serial.begin(9600);
}

void encendidoAutomatico() {
  if (VALOR < 100) {
    digitalWrite(LED, LOW);  // Enciende el LED
    delay(600);             // Espera 1 segundo
  } else {
    digitalWrite(LED, HIGH);  // Apaga el LED si no hay oscuridad
  }
}

void encendidoManual() {

  if (selecMode == 0) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
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

void loop() {
  VALOR = analogRead(SENSOR);  // lee valor de entrada A0
  selecOp = digitalRead(selec);
  selecMode = digitalRead(btnMode);

  iluminacion();
}
