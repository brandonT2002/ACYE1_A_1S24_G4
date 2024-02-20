<h1 align="center">Practica 02</h1>


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
  <li>Arduino</li>
  <li>Proteus</li>
  <li>Librerías</li>
  <ul>
	<li>Stepper</li>
	<li>LiquidCristal</li>
	<li>EEPROM</li>
  <li>Servo</li>
  </ul>
</ul>

## 🧮 Como funciona

### Banda Transportadora

**Menú**

Permite seleccionar varias opciones mediante un display LCD como lo es el historial, modo filtro y reset.

```c++
void loop() {
  if (!inSubMenu) {
    UPandDOWN();
  } else if (!inColorMenu) {
    filterMenu();
  } else {
    historyMenu();
  }
}
```

**Boton de emergencia**

Este se encarga de detener todos los procesos en cualquier momento regresando asi a un punto inicial en el que es necesario reiniciar el sistema.
```c++
```

**Filtro**

Permite seleccionar el modo en el cual trabaja la banda transportora el cual tiene dos opciones:

* **Filtro color**: 
 Se encarga de analizar el color de las cajas y separarlas segun el color que se identifico para este existen 3 alternativas de ruta para cada uno de los colores.
```c++
void readRGB(){
  red = 0, green=0, blue=0;
  
  int n = 10;
  for (int i = 0; i < n; ++i){
    //read red component
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    red = red + pulseIn(outPin, LOW);
  
   //read green component
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    green = green + pulseIn(outPin, LOW);
    
   //let's read bluee component
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    blue = blue + pulseIn(outPin, LOW);
  }
  red = red/n;
  green = green/n;
  blue = blue/n;
}
```

* **Filtro tamaño**:
 Se encarga de separar cada una de las cajas por tamaño habiendo 3 alternativas y ruta para cada una de estas.
```c++
void loop() {
  int grande = 0;
  int mediano = 0;
  int peque = 0;
  grande = digitalRead(sensorPin);
  mediano = digitalRead(sensorPin2);
  peque = digitalRead(sensorPin3);

  if (grande == LOW) {
    cajaGrande();
  } else if (mediano == LOW) {
    cajaMediana();
  } else if (peque == LOW) {
    cajaPequenia();
  }

  delay(1500);
}
```







