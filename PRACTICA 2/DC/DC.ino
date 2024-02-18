#define MOTOR 3
int i = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR, OUTPUT);
  Serial.begin(9600);
  analogWrite(MOTOR, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i = 0; i <= 255; i++){
    analogWrite(MOTOR, i);
    Serial.println(i);
    delay(500);
  }
  for(i = 255; i >= 0; i--) {
    analogWrite(MOTOR, i);
    Serial.println(i);
    delay(500);
  }
}
