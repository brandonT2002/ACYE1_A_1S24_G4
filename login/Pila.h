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
