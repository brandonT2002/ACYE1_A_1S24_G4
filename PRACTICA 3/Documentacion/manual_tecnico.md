<h1 align="center">Proyecto 01</h1>


<div align="center">
Grupo #04
</div>
<div align="center">
📕 Arquitectura De Computadoras y Ensambladores 1
</div>
<div align="center"> 🏛 Universidad San Carlos de Guatemala</div>
<div align="center"> 📆 Primer Semestre 2024</div>


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

---

