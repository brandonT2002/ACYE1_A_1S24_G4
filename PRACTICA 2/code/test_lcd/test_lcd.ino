/*
 Board I2C / TWI pins
Uno       =   A4 (SDA), A5 (SCL)
Mega2560  =   20 (SDA), 21 (SCL)
Leonardo  =   2 (SDA), 3 (SCL)
Due       =   20 (SDA), 21 (SCL), SDA1, SCL1

esta configuracion de estos pines se encuentran dentro de la librería "wire" mas info: https://www.arduino.cc/en/Reference/Wire
 */

#include <LiquidCrystal_I2C.h>  //DESCARGAR LIBRERÍA:https://github.com/ELECTROALL/Codigos-arduino/blob/master/LiquidCrystal_I2C.zip

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Opciones-> (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2)

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("==> Hola <==");
  lcd.setCursor(0, 1);
  lcd.print("==> Mundo <");
}

void loop() {
  lcd.display();
  delay(500);
  lcd.noDisplay();
  delay(500);
}