#define TRIG 10
#define ECO 9
#define LED 3

int DURATION;
int DISTANCIA;

void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG,HIGH);
  delay(1);
  digitalWrite(TRIG,LOW);
  DURATION = pulseIn(ECO,HIGH);
  DISTANCIA = DURATION/58.2;
  delay(200);

  //distancia permitida para encender el led
  if(DISTANCIA <=10 && DISTANCIA >=0){
    digitalWrite(LED,HIGH);
    delay(1000);
    digitalWrite(LED,LOW);
    Serial.println("objeto detectado");
  }
}
