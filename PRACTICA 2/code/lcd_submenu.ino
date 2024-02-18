#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define up 3
#define down 2
#define enter 4
#define Clear 5
int modeHistory = 0;
byte pointer[] = { 0x00, 0x00, 0x04, 0x02, 0x1D, 0X02, 0X04, 0X00 };
bool inSubMenu = false;
int downsel = 0;
int filterMode = 0;

int hystoryMode = 0;
bool inColorMenu = false;

int numGrande = 0, numMediano = 0, numPeque = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
  pinMode(Clear, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("<GRUPO04_A_1S24>");
  lcd.setCursor(5, 1);
  lcd.print("Practica 1");
  delay(3000);
  lcd.clear();

  lcd.createChar(5, pointer);
}

void loop() {
  if (!inSubMenu) {
    UPandDOWN();
  } else if (!inColorMenu) {
    filterMenu();
  } else {
    historyMenu();
  }
}

void UPandDOWN() {
  lcd.clear();
  switch (downsel) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }
  if (downsel == 0 || downsel == 1) {
    lcd.setCursor(1, 0);
    lcd.print("Historial");
    lcd.setCursor(1, 1);
    lcd.print("Modo Filtro");
  } else {
    lcd.setCursor(1, 0);
    lcd.print("Modo Filtro");
    lcd.setCursor(1, 1);
    lcd.print("Reset");
  }

  if (digitalRead(down) == LOW && downsel < 2) {
    downsel++;
    delay(200);
  }
  if (digitalRead(up) == LOW && downsel > 0) {
    downsel--;
    delay(100);
  }
  if (digitalRead(enter) == LOW) {
    if (downsel == 0) {
      inSubMenu = true;
      inColorMenu = true;
      filterMode = 0;
    } else if (downsel == 1) {
      inSubMenu = true;
      inColorMenu = false;
      filterMode = 0;
    } else {
      ENTER();
    }
  }

  delay(100);
}

void filterMenu() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  switch (filterMode) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }
  lcd.setCursor(1, 0);
  lcd.print("por color");
  lcd.setCursor(1, 1);
  lcd.print("por tamanio");

  if (digitalRead(down) == LOW && filterMode < 1) {
    filterMode++;
    delay(200);
  }
  if (digitalRead(up) == LOW && filterMode > 0) {
    filterMode--;
    delay(100);
  }
  if (digitalRead(Clear) == LOW) {
    inSubMenu = false;
  }

  if (digitalRead(enter) == LOW) {
    if (filterMode == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Modo por color seleccionado");
      lcd.scrollDisplayLeft();
      delay(2000);  // Espera 2 segundos
    } else if (filterMode == 1) {
      // Aquí puedes agregar el código para manejar la selección de "por tamaño"
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Modo por tamanio seleccionado");
      delay(2000);  // Espera 2 segundos
    }
  }

  delay(100);
}

void historyMenu() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  switch (hystoryMode) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }
  lcd.setCursor(1, 0);
  lcd.print("Por color");
  lcd.setCursor(1, 1);
  lcd.print("Por tamanio");

  if (digitalRead(down) == LOW && hystoryMode < 1) {
    hystoryMode++;
    delay(200);
  }
  if (digitalRead(up) == LOW && hystoryMode > 0) {
    hystoryMode--;
    delay(100);
  }
  if (digitalRead(Clear) == LOW) {
    inColorMenu = false;
  }

  if (digitalRead(enter) == LOW) {
    inColorMenu = false;
    if (hystoryMode == 0) {
      modeHistory = 0;
      hystoryMode = 0;
    } else if (hystoryMode == 1) {
      modeHistory = 1;
    }

    while (1) {
      enterData();
      inSubMenu = true;
      if (digitalRead(Clear) == LOW) {
        lcd.clear();
        inColorMenu=true;
        break;
      }
    }
  }
  delay(100);
}

void ENTER() {
  switch (downsel) {
    case 2:
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Reiniciando...");
      delay(1000);
      break;
  }
}

int dataI = 0;
void enterData() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  switch (dataI) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.write(5);
      break;
  }

  if (modeHistory == 1) {
    printSize();
  } else if (modeHistory == 0) {
    printColor();
  }

  if (digitalRead(down) == LOW  && dataI < 2) {
    dataI++;
    delay(200);
  }
  if (digitalRead(up) == LOW  && dataI > 0) {
    dataI--;
    delay(100);
  }
  delay(100);
}

void printSize() {
  if (dataI == 0 || dataI == 1) {
    lcd.setCursor(1, 0);
    lcd.print("Cajas Grandes: ");
    lcd.setCursor(1, 1);
    lcd.print("Cajas Medianas: ");
  } else {
    lcd.setCursor(1, 0);
    lcd.print("Cajs Medianas: ");
    lcd.setCursor(1, 1);
    lcd.print("Cajas Pequenias: ");
  }
}

void printColor() {
  if (dataI == 0 || dataI == 1) {
    lcd.setCursor(1, 0);
    lcd.print("Cajas Color X: ");
    lcd.setCursor(1, 1);
    lcd.print("Cajas Color Y: ");
  } else {
    lcd.setCursor(1, 0);
    lcd.print("Cajs Color Y: ");
    lcd.setCursor(1, 1);
    lcd.print("Cajas Color Z: ");
  }
}
