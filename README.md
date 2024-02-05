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

### Login

**Funcion sistema**

Solicita la entrada de una contraseña y verifica si coincide con la contraseña predeterminada. También gestiona el bloqueo del sistema después de un número máximo de intentos fallidos.

```c++
void sistema() {
  static bool claveCorrecta = false;
  
  Serial.println(" ");
  Serial.println("Ingrese contrasenia");

  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA == '#') {
      CLAVE[INDICE] = '\0';
      Serial.println(" ");

      if (strcmp(CLAVE, CLAVE_DEFECTO) == 0) {
        // Contraseña correcta
        claveCorrecta = true;
        Serial.println("Validando contrasenia... "); 
        Serial.println("La contrasenia se ha ingresado correctamente"); 
        Serial.println(" ");
        Serial.println("Desbloqueado..."); 
        intentosIncorrectos = 0;  
        encenderLEDs(intentosIncorrectos);
        modoSeguridad();
      } else {// Contraseña incorrecta
        Serial.println("Validando contrasenia... "); 
        Serial.println("La contrasenia no se ha ingresado correctamente"); 
        intentosIncorrectos++;
        encenderLEDs(intentosIncorrectos);

        if (intentosIncorrectos >= maxIntentos) {
          Serial.println(" ");
          Serial.println("Maximo de intentos fallidos"); 
          Serial.println("Bloqueando...");
          tiempoBloqueo = millis(); 
          while (millis() - tiempoBloqueo < 15000) { 
            delay(1000); 
          }

          intentosIncorrectos = 0;
          apagarLEDs();
          tiempoBloqueo = 0; 
        }
      }
      
      INDICE = 0;  
      break;       
    } else if (TECLA) {
      CLAVE[INDICE] = TECLA;
      INDICE++;
      Serial.print("*"); 
    }
  }
}
```

**Funcion seguridad**

Cambia al modo de seguridad, donde se espera la entrada de una contraseña o cambio a funcionalidad de calculdora.

```c++
void modoSeguridad() {

  Serial.println(" ");
  Serial.println("Cambiando a modo Seguridad"); 
  Serial.println("=== ARMADO ==="); 

  while (digitalRead(push_boton1) == HIGH || digitalRead(push_boton2) == HIGH) {
    delay(50);
  }
  
  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA) {
      if (TECLA == '*') {
        cambiarClave();
        return; 
      } else {
        Serial.print(TECLA);
      }
    }

    if (digitalRead(push_boton2) == HIGH) {
      calculadora();
      return;
    }
     if (digitalRead(push_boton1) == HIGH) {
      sistema();
      return;
    }
  }
}
```

**Funcion cambiar clave**

Permite cambiar la contraseña actual solicitando una nueva contraseña y verificando su formato antes de actualizar la contraseña predeterminada.

```c++
void cambiarClave() {
  Serial.println(" ");
  Serial.println("Esperando contrasenia, ingrese la nueva clave "); 

  INDICE = 0;         
  int longitud = 0;   

  char nuevaClave[9];

  while (true) {
    TECLA = teclado1.getKey();

    if (TECLA) {
      if (TECLA == '#') {
        if (longitud != 6 && longitud != 8) {
          Serial.println(" La clave debe tener 6 u 8 caracteres. Intente de nuevo.");
          modoSeguridad();
          return;
        }

        // Verificar patron para clave de 6
        if (longitud == 6 &&
            !((isdigit(nuevaClave[0]) && isalpha(nuevaClave[1]) &&
               isdigit(nuevaClave[2]) && isalpha(nuevaClave[3]) &&
               isdigit(nuevaClave[4]) && isdigit(nuevaClave[5])) ||
              (isalpha(nuevaClave[0]) && isdigit(nuevaClave[1]) &&
               isalpha(nuevaClave[2]) && isdigit(nuevaClave[3]) &&
               isalpha(nuevaClave[4]) && isdigit(nuevaClave[5])))) {
          Serial.println(" Patron incorrecto para clave de 6 caracteres.");
          modoSeguridad();
          return;
        }

        // Verificar patron para clave de 8
        if (longitud == 8 &&
            !((isalpha(nuevaClave[0]) && isdigit(nuevaClave[1]) &&
               isalpha(nuevaClave[2]) && isalpha(nuevaClave[3]) &&
               isdigit(nuevaClave[4]) && isdigit(nuevaClave[5]) &&
               isalpha(nuevaClave[6]) && isdigit(nuevaClave[7])) ||
              (isdigit(nuevaClave[0]) && isalpha(nuevaClave[1]) &&
               isdigit(nuevaClave[2]) && isdigit(nuevaClave[3]) &&
               isalpha(nuevaClave[4]) && isalpha(nuevaClave[5]) &&
               isdigit(nuevaClave[6]) && isalpha(nuevaClave[7])))) {
          Serial.println(" Patron incorrecto para clave de 8 caracteres.");
          modoSeguridad();
          return;
        }

        // Actualizar CLAVE_DEFECTO con la nueva clave
        strncpy(CLAVE_DEFECTO, nuevaClave, sizeof(CLAVE_DEFECTO) - 1);
        CLAVE_DEFECTO[sizeof(CLAVE_DEFECTO) - 1] = '\0';

        Serial.println(" Nueva clave ingresada correctamente.");


        while (teclado1.getKey() != '#') {
          delay(50);
        }

        Serial.println(" Nueva clave almacenada correctamente."); 
        delay(300);

        return;
      } else {
        nuevaClave[INDICE] = TECLA;
        INDICE++;
        Serial.print(TECLA);
        longitud++;
      }
    }
  }
}
```

**Funcion encenderLEDs**

Enciende los LEDs correspondientes según la cantidad proporcionada.
 
```c++
void encenderLEDs(int cantidad) {
  digitalWrite(ledPin1, cantidad >= 1 ? HIGH : LOW);
  digitalWrite(ledPin2, cantidad >= 2 ? HIGH : LOW);
  digitalWrite(ledPin3, cantidad == 3 ? HIGH : LOW);
}
```



