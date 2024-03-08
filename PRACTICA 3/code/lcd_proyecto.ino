#include <LiquidCrystal_I2C.h>

// Declaración del enum para los diferentes estados del LED
enum EstadoLED {
  ILUMINACION_ON,
  ILUMINACION_OFF,
  VENTILADORES_ON,
  VENTILADORES_OFF,
  REGANDO,
  ABRIENDO_PUERTA,
  CERRANDO_PUERTA,
  ENTRADA,
  ILUMINACION_MANUAL,
  SENSOR_PROPANO_ON,
};

// Variable para almacenar el estado actual del LED
EstadoLED estado = ILUMINACION_ON;

// Inicialización del objeto LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Inicialización del LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Ciclo para cambiar entre los diferentes estados
  for (int i = 0; i < 10; i++) {
    // Cambiar al siguiente estado
    cambiarEstado(EstadoLED(i));
    // Esperar un momento para ver el estado en el LCD
    delay(2000);
  }
}

// Función para cambiar el estado del LED
void cambiarEstado(EstadoLED nuevoEstado) {
  estado = nuevoEstado;
  // Dependiendo del estado actual del LED, realiza diferentes acciones
  switch (estado) {
    case ILUMINACION_ON:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Iluminacion");
      lcd.setCursor(0, 1);
      lcd.print("Activada");
      break;
    case ILUMINACION_OFF:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Iluminacion");
      lcd.setCursor(0, 1);
      lcd.print("Desactivada");
      break;
    case VENTILADORES_ON:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ventiladores");
      lcd.setCursor(0, 1);
      lcd.print("Activados");
      break;
    case VENTILADORES_OFF:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ventiladores");
      lcd.setCursor(0, 1);
      lcd.print("Desactivados");
      break;
    case REGANDO:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Regando");
      break;
    case ABRIENDO_PUERTA:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Abriendo Puerta");
      break;
    case CERRANDO_PUERTA:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cerrando Puerta");
      break;
    case ENTRADA:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Entrada");
      break;
    case ILUMINACION_MANUAL:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Iluminacion");
      lcd.setCursor(0, 1);
      lcd.print("manual");
      break;
    case SENSOR_PROPANO_ON:
      delay(500);
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Propano");
       lcd.setCursor(0, 1);
      lcd.print("Encendido");
      break;
  }
}
