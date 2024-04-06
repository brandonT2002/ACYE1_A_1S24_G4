<h1 align="center">Proyecto 01</h1>


<div align="center">
Grupo #04
</div>
<div align="center">
📕 Arquitectura De Computadoras y Ensambladores 1
</div>
<div align="center"> 🏛 Universidad San Carlos de Guatemala</div>
<div align="center"> 📆 Primer Semestre 2024</div>

### RECURSOS NECESARIOS
- Sistema operativo Android, iOS.
- Dispositivo compatible con Bluetooth.

## ⚙ Tecnologías Utilizadas

<div align="center" style="display:flex;justify-content:center;gap:20px">
 <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=arduino,git,flutter" />
  </a>
</div>
<ul>
  <li>Arduino</li>
  <li>Proteus</li>
  <li>Librerías</li>
  <ul>
	<li>LCD</li>
  <li>Servo</li>
  <li>SoftwareSerial</li>
  </ul>
</ul>

<br>
<br>
<br>

# 📍 Manual Tecnico

*Este manual técnico proporciona detalles sobre la implementación del sistema de monitoreo y control. El sistema integra sensores, actuadores y conectividad Bluetooth para permitir a los usuarios controlar y monitorear diversos aspectos de su hogar de manera remota.*

## Componentes del Sistema
### Sensores:

-  Sensor de temperatura y humedad DHT11.
-  Sensor de humedad de suelo conectado al pin A0.
-  Sensor de efecto Hall para la detección de campos magnéticos.
-  Actuadores:
- Luces LED controladas por pines específicos.
- Bomba de agua controlada por un relé (pin bombAguaPin).
- Servomotor para ajustar posición basado en el sensor de efecto Hall.
- Comunicación Bluetooth: Módulo Bluetooth configurado con SoftwareSerial para RX (pin 10) y TX (pin 11).
  

### Funcionamiento del Código
#### Funciones Principales:

-  sensorDHT11(): Lee la humedad y temperatura de la casa utilizando  el sensor DHT11 y envía datos a través de Bluetooth.
-  sensorHSuelo(): Mide la humedad del suelo y controla la bomba de agua según el nivel de humedad.
-  hallSensor(): Controla la posición del servomotor basándose en la detección de campos magnéticos.
-  Bluetooth(): Gestiona la comunicación Bluetooth y ejecuta acciones según los comandos recibidos.
-  Control de Luces: Se controlan seis luces LED (pines ledPin1 a ledPin6) mediante comandos Bluetooth desde '1' hasta '6'. 'E' enciende todas las luces, 'A' apaga   todas las luces.

#### Configuración y Conexiones:

- Configuración del sensor de efecto Hall, el servomotor, las luces LED, la bomba de agua y la comunicación Bluetooth.
Bucle Principal:

- El bucle principal ( loop() ): realiza lecturas de sensores, controla actuadores, maneja la comunicación Bluetooth y espera 1 segundo entre mediciones.

- La imagen presentada muestra el diagrama de conexiones detallado de un proyecto electrónico utilizando un Arduino Mega. El diagrama incluye una pantalla LCD JHD-2X16-I2C, que se conecta al Arduino a través de los pines SCL y SDA para la comunicación I2C, así como a los pines de 5V y GND para su alimentación. Adicionalmente, se muestra I2C que comparte las líneas SCL y SDA con la pantalla LCD para la comunicación de datos, y también se conecta a sus propias fuentes de alimentación y tierra. Este esquema proporciona una guía visual clara para la correcta conexión de los componentes y facilita el montaje físico del proyecto.

<p align="center">
    <img src="/PRACTICA 3/Documentacion/imagenes/diagrama-arqui.jpg" width="700">
</p>

#### Implementación del Código:
##### Sensor MQ2

Esta función lee los datos del sensor de gas MQ2 y toma decisiones en función del valor leído.
```c++
void sensorMQ2() {
  int mq2Value = analogRead(mq2);
  Serial.println(mq2Value);
  if (mq2Value >= 300) {
    analogWrite(pinPWM, 200);
  } else {
    digitalWrite(pinPWM, 0);
  }
}

```

##### Sensor HT11

Esta función lee los datos del sensor de temperatura y humedad DHT11 y realiza acciones en función de los valores leídos.
```c++
void sensorDHT11() {
  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println("Error al leer del sensor DHT");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("°C");

  int tempInt = min(abs(round(t)), 99);
  int decenas = tempInt / 10;
  int unidades = tempInt % 10;

  // Control de displays de 7 segmentos basado en la temperatura
  // (no está implementado completamente en este fragmento de código)
}

```

##### Iluminación

Esta función controla la iluminación en función de la selección de modo (automático o manual) y los comandos recibidos a través de Bluetooth.
```c++
void iluminacion() {
  if (selecOp == 0) {
    encendidoManual();
  } else {
    encendidoAutomatico();
  }
  delay(10);
}

```

##### Bluetooth

Esta función maneja la comunicación Bluetooth y realiza acciones en función de los comandos recibidos.
```c++
void Bluetooth() {
  if (bluetooth.available()) {
    char receivedChar = bluetooth.read();
    switch (receivedChar) {
      case 'S':
        selecOp = 1;
        break;
      case 'M':
        selecOp = 0;
        break;
      default:
        modeLight = receivedChar;
        break;
    }
  }
}

```

##### Costos de implementacion

| Componente | Descripción | Precio (Q)|
|------------|-------------|--------|
| Sensor de temperatura y humedad DHT11 | Sensor para medir la temperatura y humedad del ambiente | 22 |
| Sensor de humedad de suelo | Sensor para medir la humedad del suelo | 5 |
| Sensor de efecto Hall | Sensor para la detección de campos magnéticos | 15 |
| Luces LED | Luces controladas por pines específicos | 6 |
| Bomba de agua | Bomba controlada por un relé | 30  |
| Servomotor | Motor para ajustar posición basado en el sensor de efecto Hall | 31 |
| Módulo Bluetooth | Módulo para la comunicación Bluetooth | 50 |
| Maqueta          |                                       | 350  |
| H-Bridge Motor Driver | Controlador para los ventiladores | 37 |
| Ventiladores 12v 4x4cm | Ventiladores para la circulación de aire | 23 |
| MD-DQ9 | sensor gas | 34 |
| Módulo de resistencia |  luz | 18 |
| Total |  | 621 |

<br>
<br>
<br>
<br>

# 📍 Manual Usuario

## Funcionamiento del sistema
Sistema Iot para controlar y monitorear diferentes servicios de una vivienda moderna de manera remota. 

1. Se necesitar tener instalada la app en el dispositivo movil.
   
<p align="center">
    <img src="/PRACTICA 3/Documentacion/imagenes/1.jpeg" width="250">
</p>

1. Emparejar el dispositivo al sensor , este proceso asegura que ambos dispositivos esten autorizados para cambiar datos.
   
<p align="center">
    <img src="/PRACTICA 3/Documentacion/imagenes/2.jpeg" width="250">
</p>

1. Una vez que la conexión Bluetooth está establecida, el celular y el sensor pueden comenzar a intercambiar datos para el funcionamiento.
   
<p align="center">
    <img src="/PRACTICA 3/Documentacion/imagenes/3.jpeg" width="250">
</p>

1. La vivienda incorpora una sofisticada instalacion de sensores Estos dispositivos inteligentes permiten la captura de datos en tiempo real, proporcionando a los habitantes un control sin precedentes sobre el entorno doméstico.

<p align="center">
    <img src="/PRACTICA 3/Documentacion/imagenes/5.jpeg" width="400">
    <img src="/PRACTICA 3/Documentacion/imagenes/6.jpeg" width="400">
</p>