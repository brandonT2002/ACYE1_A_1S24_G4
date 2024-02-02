#include <Keypad.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define MAX_SIZE 100  // Define el tamaño máximo de la pila
struct Valor {
  float valor;
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
      float resultado = evaluarExp();
      if (error.length() != 0) {
        return error;
      }
      String format = "Resultado = " + String(resultado) + "-";
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

  float operar(char simbolo, float dato1, float dato2) {
    switch (simbolo) {
      case '+':
        return dato1 + dato2;
      case '-':
        return dato1 - dato2;
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
      if (esOperando(infija[i])) {
        // Manejar números de dos dígitos o más
        int num = 0;
        while (i < infija.length() && !esOperador(infija[i])) {
          num = num * 10 + (infija[i] - '0');
          i++;
        }
        i--;  // Retroceder para compensar el incremento final en el bucle
        Valor valor_operando = { (float)num, ' ' };
        postfija += String(num) + " ";
      } else if (esOperador(infija[i])) {
        // Si el operador en la pila tiene mayor precedencia, sacarlo y agregarlo a la postfija
        Valor aux = pila1.peek();
        if (aux.valor == 404.0 && aux.simbolo == "$") {
          error = "Falta de operandos";
        } else {
          while (!pila1.estaVacia() && precedencia(aux.simbolo) >= precedencia(infija[i])) {
            Valor valor_operador = pila1.pop();
            postfija += String(valor_operador.simbolo) + " ";
          }
        }
        // Agregar el operador actual a la pila
        Valor valor_operador = { 0.0f, infija[i] };
        pila1.push(valor_operador);
      }
    }
    // Sacar todos los operadores restantes de la pila y agregarlos a la postfija
    while (!pila1.estaVacia()) {
      Valor valor_operador = pila1.pop();
      postfija += String(valor_operador.simbolo) + " ";
    }
  }


  float evaluarExp() {
    float resultado = 0;

    char postfijaArray[postfija.length() + 1];
    postfija.toCharArray(postfijaArray, postfija.length() + 1);

    char* token = strtok(postfijaArray, " ");
    bool esError = false;
    while (token != NULL) {
      if (esOperando(token[0])) {  // Verificar si es un operando
        float valor = atof(token);
        Valor valor_operando = { valor, ' ' };
        pila1.push(valor_operando);
      } else if (esOperador(token[0])) {  // Verificar si es un operador
        Valor dato2 = pila1.pop();
        Valor dato1 = pila1.pop();
        if ((dato2.valor == 404.0 && dato2.simbolo == '$') || (dato1.valor == 404.0 && dato1.simbolo == '$')) {
          error = "Falta de operandos";
          esError = true;
          break;
        }
        float resultado_operacion = operar(token[0], dato1.valor, dato2.valor);
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
void setup() {
  Serial.begin(9600);
  P.begin();
}

void loop() {
  P.displayAnimate();

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
      //Serial.println(input);
      Calculadora calc;
      String resultado = calc.calcular(input);
      strcpy(curMessage, resultado.c_str());
      P.displayClear();
      P.displayReset();
      P.print(curMessage);
      input = "";
      resultado = 0.0;
      delay(3000);
      clearBuffer();
      P.displayClear();
      P.displayReset();
    } else if (key == '$') {
      input = "";
      clearBuffer();
      P.displayClear();
      P.displayReset();
    }
  }
}
