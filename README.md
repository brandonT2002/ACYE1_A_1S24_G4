<h1 align="center">Practica 1</h1>


<div align="center">
Grupo #04
</div>
<div align="center">
📕 Arquitectura De Computadoras y Ensambladores 1
</div>
<div align="center"> 🏛 Universidad San Carlos de Guatemala</div>
<div align="center"> 📆 Primer Semestre 2024</div>


# 📍 Manual Técnico

## ⚙ Tecnologías Utilizadas

<div align="center" style="display:flex;justify-content:center;gap:20px">
 <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=arduino,git" />
  </a>
</div>
<ul>
  <li>Arduno</li>
  <li>Proteus</li>
  <li>Librerías</li>
  <ul>
	<li>Keypad</li>
	<li>MD_Parola</li>
	<li>MD_MAX72xx</li>
  </ul>
</ul>

## 🧮 Como funciona

### Calculadora

**Clase Pila**

Se hace uso de una clase pila para poder guardar datos y precedencia, se definen los siguientes métodos:

* **push**: Agrega un dato de la pila.
* **pop**: Extrae el primer elemento dato de la pila, desapilandolo en el proceso.
* **peek**: Extrae el valor un dato de la pila, sin desapilar el valor de la pila.
* **clear**: Limpia la pila.
* **estaVacia()**: Verifica si la pila esta vacia.
* **size**: retorna la cantidad de elementos en la pila.

**Clase Calculadora**

Dentro de esta clase se hace el proceso para pasar una expresión de infija a postfija, con el método infijaPostFija(), el cual sirve de la siguiente manera

```c++

void infijaPostFija() {
    //Itera a lo largo de la expresión original
    for (int i = 0; i < infija.length(); i++) {
        // verifica si es un operando, a su vez verifica si el número en cuestión es trata de un numero negativo o solo se trata de la operación resta
      if (esOperando(infija[i]) || (infija[i] == '-' && (i == 0 || esOperador(infija[i - 1])))) {
        //Se concatena los valores de más de 2 números y se verifica su simbolo (+ -) para poder guardarlo en el string postfija
        postfija += String(num) + " ";
    
      //se verifica si es operador
      } else if (esOperador(infija[i])) {
        //Se extrae el primero de la pila, luego se valida si se trata de un error o no, si todo esta correcto se procede a guardar los datos en la pila siguiendo las siguientes reglas:
           /*
                * Si la pila esta vacía o el operador en la cima tiene menor prioridad que el operador acutal, se agrega a la pila
                *Si el caso es el contrario se extraee y agrega a la salida el operador en la cima de la pila hasta que se encuentre un operador con menor o igual pruioridad a la actual

           */
        pila1.push(valor_operador);
      }
    }
    while (!pila1.estaVacia()) {
      //Si quedan datos se agregan a la salida 
    }

  }
```

Para evaluar las expresiones y obtener el resultado se hace uso de la función evaluarExp:
```c++
int evaluarExp() {
    int resultado = 0;

    //se convierte la salida de la funcion que transforma de infija a postfija, a un arreglo separandolos por los espacios en blanco

    //se itera todos los elementos del arreglo
    while (token != NULL) {

    // se verifica si el elemento es un operando y si es un número negativo
      if (esOperando(token[0]) || (token[0] == '-' && strlen(token) > 1)) {  
        // Verificar si es un operando o un número negativo
        
      } else if (esOperador(token[0])) {  // 
        if (pila1.size() < 2) {           
            // Verificar si hay suficientes operandos en la pila
        }
        // extrae los valores a usar para las operaciones
        Valor dato2 = pila1.pop();
        Valor dato1 = pila1.pop();
        // se llama a la función operar mandandole la información de la operación a realizar
        int resultado_operacion = operar(token[0], dato1.valor, dato2.valor);
        if (error.length() != 0) {
          //Si existe un error mostrarlo en pantalla
        }
       //agregar el valor resultante a la pila
      }

      token = strtok(NULL, " ");
    }

    // retornar el valor del resultado
  }
```

anteriormente se hicieron uso de funciónes auxiliares, a continuación se muestran dichos métodos/funciones:

```c++
 bool esOperador(char dato) {
    // verifica que el caracter recibido sea un signo de un aperación
  }

  bool esOperando(char dato) {
    // verifica que el caracter sea un número
  }

  int precedencia(char op) {
    // valida la precedencia de operadores devolviendo un valor dependiendo del caracter recibido
  }

  int operar(char simbolo, int dato1, int dato2) {
    // Dentro de esta función se operan los valores recibidos, teniendo varios casos, y validando errores como la división por 0
  }
```

Finalmente se hace uso de la función calcular para poder realizar las operaciones enviadas:

```c++
 String calcular(String exp) {
    // se agregan los valores a sus respecitvas variables
    agregarExp(exp);
    // se pasa a postfija la exprsión
    infijaPostFija();
    // se valida si hay errores
    if (error.length() != 0) {
      return error;
    } else {
        // si no hay errores se porcede a obtener el resultado de la operacion y darle el formato para la impresión en la matriz
        // de igual manera se imprime de forma serial la exprsión junto con su resultado
    }
 }
```