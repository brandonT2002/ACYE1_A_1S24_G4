#include <Keypad.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#define MAX_SIZE 100  // Define el tamaño máximo de la pila
struct Valor {
  int valor;
  char simbolo;
};
class Pila {
private:
  Valor datos[MAX_SIZE];  // Array para almacenar los datos
  int top;                // Índice del elemento superior de la pila

public:
  Pila() {
    top = -1;  // Inicializa la pila vacía
  }

  // Función para agregar un elemento a la pila
  void push(Valor valor) {
    if (top >= MAX_SIZE - 1) {
      //Serial.println("Error: la pila está llena");
    } else {
      datos[++top] = valor;
    }
  }

  // Función para quitar un elemento de la pila
  Valor pop() {
    if (top < 0) {
      ;
      //Error: No se tienen operaciones
      return { 404.0f, '$' };
    } else {
      return datos[top--];
    }
  }

  Valor peek() {  // Cambiado a Valor
    if (top == -1) {
      //Serial.println("Error: pila vacía");
      return { 404.0f, '$' };
    } else {
      return datos[top];
    }
  }

  int getTop() {
    return top;
  }

  void clear() {
    top = -1;
  }

  bool estaVacia() {
    return top == -1;
  }

  int size() {
    return top + 1;
  }
};

class Calculadora {
public:
  String infija;
  String postfija;
  Pila pila1;
  String error;

  Calculadora() {
    infija = "";
    postfija = "";
    error = "";
  }

  String calcular(String exp) {
    agregarExp(exp);
    infijaPostFija();
    if (error.length() != 0) {
      return error;
    } else {
      int resultado = evaluarExp();
      if (error.length() != 0) {
        return error;
      }
      String format = "RESULTADO = {" + String(resultado) + "}";
      Serial.println(exp + " = " + resultado);
      return format;
    }
  }

  void agregarExp(String exp) {
    infija = exp;
    postfija = "";
  }

  bool esOperador(char dato) {
    return dato == '+' || dato == '-' || dato == '*' || dato == '/';
  }

  bool esOperando(char dato) {
    return dato >= '0' && dato <= '9';
  }

  int precedencia(char op) {
    if (op == '/') {
      return 3;
    } else if (op == '*') {
      return 2;
    } else if (op == '+' || op == '-') {
      return 1;
    } else {
      return 0;
    }
  }

  int operar(char simbolo, int dato1, int dato2) {
    switch (simbolo) {
      case '+':
        return dato1 + dato2;
      case '-':
        //return dato1 - dato2;
        if (pila1.size() == 1) {
          return -dato1;  // Negación
        } else {
          return dato1 - dato2;  // Resta con cambio de orden
        }

      case '*':
        return dato1 * dato2;
      case '/':
        if (dato2 == 0) {
          error = "DIVISIÓN POR 0";
          return 0;
        }
        return dato1 / dato2;
      default:
        return 0;
    }
  }

  void infijaPostFija() {
    for (int i = 0; i < infija.length(); i++) {
      if (esOperando(infija[i]) || (infija[i] == '-' && (i == 0 || esOperador(infija[i - 1])))) {
        int num = 0;
        int sign = 1;
        if (infija[i] == '-') {
          sign = -1;
          i++;
        }
        while (i < infija.length() && !esOperador(infija[i])) {
          num = num * 10 + (infija[i] - '0');
          i++;
        }

        i--;
        num *= sign;

        Valor valor_operando = { (int)num, ' ' };
        postfija += String(num) + " ";
      } else if (esOperador(infija[i])) {
        Valor aux = pila1.peek();
        if (aux.valor == 404.0 && aux.simbolo == "$") {
          error = "Falta de operandos";
        } else {
          while (!pila1.estaVacia() && precedencia(aux.simbolo) >= precedencia(infija[i])) {
            Valor valor_operador = pila1.pop();
            postfija += String(valor_operador.simbolo) + " ";
            aux = pila1.peek();  // Actualizar el valor auxiliar después de sacar un operador de la pila
          }
        }
        Valor valor_operador = { 0.0f, infija[i] };
        pila1.push(valor_operador);
      }
    }
    while (!pila1.estaVacia()) {
      Valor valor_operador = pila1.pop();
      postfija += String(valor_operador.simbolo) + " ";
    }
  }



  int evaluarExp() {
    int resultado = 0;

    char postfijaArray[postfija.length() + 1];
    postfija.toCharArray(postfijaArray, postfija.length() + 1);

    char* token = strtok(postfijaArray, " ");
    bool esError = false;
    while (token != NULL) {
      if (esOperando(token[0]) || (token[0] == '-' && strlen(token) > 1)) {  // Verificar si es un operando o un número negativo
        int valor = atoi(token);
        Valor valor_operando = { valor, ' ' };
        pila1.push(valor_operando);
      } else if (esOperador(token[0])) {  // Verificar si es un operador
        if (pila1.size() < 2) {           // Verificar si hay suficientes operandos en la pila
          error = "Falta de operandos";
          esError = true;
          break;
        }
        Valor dato2 = pila1.pop();
        Valor dato1 = pila1.pop();
        int resultado_operacion = operar(token[0], dato1.valor, dato2.valor);
        if (error.length() != 0) {
          esError = true;
          break;
        }
        Valor valor_resultado = { resultado_operacion, ' ' };
        pila1.push(valor_resultado);
      }

      token = strtok(NULL, " ");
    }

    if (!pila1.estaVacia()) {
      resultado = pila1.pop().valor;
    }

    if (esError) {
      return 0;
    }
    return resultado;
  }
};

const byte filas = 4;
const byte columnas = 4;

const char tecla[filas][columnas] = {
  { '7', '8', '9', '+' },
  { '4', '5', '6', '-' },
  { '1', '2', '3', '*' },
  { '$', '0', '#', '/' }
};

byte pinFilas[filas] = { 18, 19, 20, 21 };        //conectarse a las patillas de salida de fila del
byte pinColumnas[columnas] = { 14, 15, 16, 17 };  //conectarse a las patillas de las
Keypad keypad = Keypad(makeKeymap(tecla), pinFilas, pinColumnas, filas, columnas);

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEV 3
#define CLK 8
#define DATA 6
#define CS 5
#define BUF_SIZE 90
#define BUFFER_SIZE 5
// Variable que controla la matriz
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA, CLK, CS, MAX_DEV);
char curMessage[BUF_SIZE] = { "" };
char buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;
// temporizador
unsigned long displayStartTime = 0;
const unsigned long displayDuration = 15000;
// direccion
const byte switchPin = 13;
bool scrollRight = true;
// animacion
const byte switchPin2 = 12;
// velocidad
int potentiometerPin = A0;
int potValue = 0;

String printBuffer() {
  String aux = "";
  for (int i = head; i != tail; i = (i + 1) % BUFFER_SIZE) {
    // P.print(buffer[i]);
    aux += buffer[i];
  }
  return aux;
}

void addCharToBuffer(char c) {
  buffer[tail] = c;
  tail = (tail + 1) % BUFFER_SIZE;
  if (tail == head) {
    head = (head + 1) % BUFFER_SIZE;  // Descartar el caracter más antiguo si el buffer está lleno
  }
}

void clearBuffer() {
  head = 0;
  tail = 0;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = '\0';  // Llena el buffer con caracteres vacíos
  }
}

void displayScrollingText(const char* text, bool scrollRight, bool animation_, int speed_) {
  displayStartTime = millis();
  int scrollDirection = (scrollRight) ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT;
  P.setSpeed(speed_);

  while (millis() - displayStartTime < displayDuration) {
    P.displayClear();
    if (animation_) {
      animation(text, animation_, scrollDirection, speed_);
    } else {
      P.displayText(text, PA_LEFT, speed_, 0, scrollDirection);
    }
    while (!P.displayAnimate()) {
      if (millis() - displayStartTime >= displayDuration) {
        break;
      }
    }
  }

  memset(curMessage, 0, sizeof(curMessage));
  clearBuffer();
  P.displayClear();
  P.displayReset();
}

void animation(const char* text, bool animation_, int scrollDirection, int speed_) {
  if (animation_) {
    int indice = random(1, 22);

    P.setSpeed(speed_);
    switch (indice) {
      case 1:
        P.displayText(text, PA_LEFT, 50, 0, PA_PRINT & scrollDirection);
        break;

      case 2:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_UP & scrollDirection);
        break;

      case 3:
        P.displayText(text, PA_LEFT, 50, 0, PA_SLICE & scrollDirection);
        break;

      case 4:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCAN_HORIZ & scrollDirection);
        break;

      case 5:
        P.displayText(text, PA_LEFT, 50, 0, PA_OPENING_CURSOR & scrollDirection);
        break;

      case 6:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_DOWN_RIGHT & scrollDirection);
        break;

      case 7:
        P.displayText(text, PA_LEFT, 50, 0, PA_WIPE & scrollDirection);
        break;

      case 8:
        P.displayText(text, PA_LEFT, 50, 0, PA_GROW_UP & scrollDirection);
        break;

      case 9:
        P.displayText(text, PA_LEFT, 50, 0, PA_CLOSING_CURSOR & scrollDirection);
        break;

      case 10:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_UP_LEFT & scrollDirection);
        break;

      case 11:
        P.displayText(text, PA_LEFT, 50, 0, PA_MESH & scrollDirection);
        break;

      case 12:
        P.displayText(text, PA_LEFT, 50, 0, PA_OPENING & scrollDirection);
        break;

      case 13:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_UP_RIGHT & scrollDirection);
        break;

      case 14:
        P.displayText(text, PA_LEFT, 50, 0, PA_BLINDS & scrollDirection);
        break;

      case 15:
        P.displayText(text, PA_LEFT, 50, 0, PA_DISSOLVE & scrollDirection);
        break;

      case 16:
        P.displayText(text, PA_LEFT, 50, 0, PA_CLOSING & scrollDirection);
        break;

      case 17:
        P.displayText(text, PA_LEFT, 50, 0, PA_RANDOM & scrollDirection);
        break;

      case 18:
        P.displayText(text, PA_LEFT, 50, 0, PA_WIPE_CURSOR & scrollDirection);
        break;

      case 19:
        P.displayText(text, PA_LEFT, 50, 0, PA_GROW_DOWN & scrollDirection);
        break;

      case 20:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCAN_VERT & scrollDirection);
        break;

      case 21:
        P.displayText(text, PA_LEFT, 50, 0, PA_SCROLL_DOWN_LEFT & scrollDirection);
        break;

      default:
        break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  P.begin();

  pinMode(switchPin, INPUT);
  pinMode(switchPin2, INPUT);

  pinMode(potentiometerPin, INPUT);
}

void loop() {
  P.displayAnimate();

  // Leer el estado del switch
  bool switchState = digitalRead(switchPin);
  bool switch2State = digitalRead(switchPin2);

  potValue = analogRead(potentiometerPin);

  // Ajustar la velocidad de la animación en función del valor del potenciómetro
  int animationSpeed = 100 - map(potValue, 0, 1023, 10, 100);

  static String input = "";
  char key = keypad.getKey();
  if (key) {
    if (isdigit(key)) {
      input += key;
      addCharToBuffer(key);
      String a = printBuffer();
      // P.setCharSpacing(P.getCharSpacing() - 1);
      P.print(a.c_str());
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
      input += key;
      addCharToBuffer(key);
      String a = printBuffer();
      P.print(a.c_str());
    } else if (key == '#') {
      Serial.println("-> Calculando");
      Calculadora calc;
      String resultado = calc.calcular(input);
      strcpy(curMessage, resultado.c_str());
      P.displayClear();
      P.displayReset();
      // P.print(curMessage);
      Serial.println("-> Mostrando Resultado");
      displayScrollingText(curMessage, switchState, switch2State, animationSpeed);
      input = "";
      resultado = 0.0;
       Serial.println("-> Esperando Operacion");
    } else if (key == '$') {
      input = "";
      clearBuffer();
      P.displayClear();
      P.displayReset();
    }
  }

  static bool lastSwitchState = switchState;
  if (switchState != lastSwitchState) {
    P.displayReset();  // Reiniciar la animación para un cambio inmediato
    lastSwitchState = switchState;
  }
}
