#include "Pila.h"

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
