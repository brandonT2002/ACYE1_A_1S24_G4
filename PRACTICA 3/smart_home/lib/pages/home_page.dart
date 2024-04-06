import 'dart:convert';

import 'package:carousel_slider/carousel_slider.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:smart_home/util/CardH.dart';
import 'package:smart_home/util/smart_devices_box.dart';

import '../util/card.dart';
import '../util/segmentButton.dart';

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  // bluetooth
  final _bluetooth = FlutterBluetoothSerial.instance;
  bool _bluetoothState = false;
  bool _isConnecting = false;
  BluetoothConnection? _connection;
  List<BluetoothDevice> _devices = [];
  BluetoothDevice? _deviceConnected;
  int times = 0;
  int _currentIndex = 0;

  // Datos de sensores
  String _temperature = "N/A";
  String _humedity = "N/A";
  String _propane = "N/A";
  String _door = "N/A";
  
  // padding constants
  final double horizontalPadding = 40;
  final double verticalPadding = 25;
  List<bool> selectedWeatherValues = [true, false]; // Define la lista de seleccionados
 // ToggleButtonsSample toggleButtonsSample = ToggleButtonsSample(selectedWeather: []);
  // list of smart devices
  List mySmartDevices = [
    // [ name, path, status ]
    ["Todo",     "assets/light-bulb.png", false],
    ["Cuarto 1", "assets/light-bulb.png", false],
    ["Cuarto 2", "assets/light-bulb.png", false],
    ["Baño",     "assets/light-bulb.png", false],
    ["Sala",     "assets/light-bulb.png", false],
    ["Comedor",  "assets/light-bulb.png", false],
    ["Jardín",   "assets/light-bulb.png", false],
  ];

  List extras = [
    // [ name, path, status ]
    ["Humedad", "assets/humidity.png"],
    ["Propano", "assets/Propane.png" ],
  ];
  String weather = 'No se ha seleccionado el clima';
  final ValueNotifier<List<bool>> selectedWeather = ValueNotifier([true, false]);

  void _updateWeather() {
    // Aquí puedes actualizar la UI de acuerdo al estado de selectedWeather
    setState(() {
      int selectedIndex = selectedWeather.value.indexOf(true);
      weather = selectedIndex == 0 ? 'S' : 'M';
     // String valor = selectedIndex == 0 ? 'S' : 'N' ;
      _sendData(weather);
    });
  }

  // Notificación
  void _showAlertDialog(BuildContext context, String message) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text('Sistema de Seguridad'),
          content: Text(message),
          actions: <Widget>[
            TextButton(
              onPressed: () {
                Navigator.of(context).pop();
              },
              child: Text('OK'),
            ),
          ],
        );
      },
    );
  }

  // power button switched
  void powerSwitchChanged(bool value, int index) {
    setState(() {
      if (index == 0) {
        // Si el switch "Todo" cambia, cambiar todos los switches
        for (int i = 0; i < mySmartDevices.length; i++) {
          mySmartDevices[i][2] = value;
        }
      } else {
        // Si se cambia otro switch, actualizar solo ese switch
        mySmartDevices[index][2] = value;
        // Si el switch no es el "Todo" y se desactiva, desactivar también el switch "Todo"
        if (!value) {
          mySmartDevices[0][2] = false;
        }
      }
    });
    
    if (index == 0) {
      print(mySmartDevices[index][2]);
      for (int i = 1; i < mySmartDevices.length; i++) {
        // Calcular el valor a enviar basado en el índice y el patrón especificado
        if(!mySmartDevices[index][2]){
          int valueToSend = i+ 6 ;
          String dataToSend = "";
          if (valueToSend >= 10){
            valueToSend +=55;
            dataToSend = String.fromCharCode(valueToSend);
          }else{
            dataToSend = valueToSend.toString();
          }
          _sendData(dataToSend);
        }else{
          String dataToSend = i.toString();
          _sendData(dataToSend);
        }

      }
    } else {
      // Calcular el valor a enviar basado en el índice y el patrón especificado
      //print(mySmartDevices[index][2]);
      //print(index);
      if(!mySmartDevices[index][2]){
        int valueToSend = index + 6; // Si el interruptor está activado, se suma 1, de lo contrario se suma
        String dataToSend = "";
        if (valueToSend >= 10){
          valueToSend +=55;
          dataToSend = String.fromCharCode(valueToSend);
        }else{
          dataToSend = valueToSend.toString();
        }
        print(dataToSend);
        _sendData(dataToSend);
      }else{
        int valueToSend = index; // Si el interruptor está activado, se suma 1, de lo contrario se suma 2
        String dataToSend = valueToSend.toString();
        print(dataToSend);
        _sendData(dataToSend);
      }

    }

    // Actualizar el estado del widget
    setState(() {});
  }

  // Function to show Bluetooth dialog
  void _showBluetoothDialog(BuildContext context) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        if (_bluetoothState) {
          // Si el Bluetooth está activado, mostrar la lista de dispositivos
          return Dialog(
            child: Scaffold(
              appBar: AppBar(
                title: Text("Bluetooth"),
                actions: <Widget>[
                  TextButton(
                    onPressed: () {
                      Navigator.of(context).pop();
                    },
                    child: Text("Cancelar"),
                  ),
                ],
              ),
              body: Column(
                children: [
                  _infoDevice(),
                  Expanded(child: _listDevices()),
                ],
              ),
            ),
          );
        } else {
          // Si el Bluetooth está desactivado, mostrar un mensaje indicando que está apagado
          return AlertDialog(
            title: Text("Bluetooth"),
            content: Text("Activa el Bluetooth para ver los dispositivos."),
            actions: <Widget>[
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text("OK"),
              ),
            ],
          );
        }
      },
    );
  }

  // ==============================
  // ======== Temperatura =========

  Widget _buildTemperatureWidget() {
    // Aquí puedes personalizar cómo mostrar la temperatura ambiente
    // Esto puede ser un Text, un Icon, una combinación de ambos, etc.
    // Por ejemplo, aquí hay un Text simple:
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 20),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(Icons.thermostat),
          SizedBox(width: 10),
          Text(
            //'$weather', // Puedes cambiar esto con la temperatura real
            "Temperatura: $_temperature°C",
            style: TextStyle(fontSize: 20),
          ),
        ],
      ),
    );
  }

  // ==============================
  // ========= Bluetooth ==========

  void _getDevices() async {
    var res = await _bluetooth.getBondedDevices();
    setState(() => _devices = res);
  }

  void _receiveData() {
  String buffer = ''; // Variable para acumular los datos recibidos
  List<String> _mq2;
  
  _connection?.input?.listen((List<int> event) {
    String dataReceived = utf8.decode(event); // Convierte los datos a una cadena UTF-8
    
    // Agrega los datos recibidos al búfer
    buffer += dataReceived;
    
    // Verifica si el búfer contiene un mensaje completo terminado con un carácter de nueva línea
    if (buffer.contains('\n')) {
      // Separa el mensaje completo del búfer
      List<String> messages = buffer.split('\n');
      
      // El último elemento de 'messages' puede ser un mensaje incompleto, guárdalo para el próximo bucle
      buffer = messages.removeLast();
      
      // Procesa cada mensaje completo
      messages.forEach((message) {
        setState(() {
          // Utiliza split para dividir el mensaje por '\n' y toma la primera parte
          _mq2 = message.split('\n')[0].split(";");
          if (_mq2[0].codeUnitAt(0) != 13){
            if (_mq2[0] == "sensores") {
              _humedity = _mq2[1];
              _temperature = _mq2[2];
              _propane = _mq2[3];
            } else if (_mq2[0] == "puerta"){
              _door = _mq2[1];
            } else if (_mq2[0] == "entrada") {
              _showAlertDialog(context, 'Se detectó un objeto en la entrada.');
            }
          }
        });
      });
    }
  });
}


  void _sendData(String data) {
    if (_connection?.isConnected ?? false) {
      _connection?.output.add(ascii.encode(data));
    }
  }

  void _requestPermission() async {
    await Permission.location.request();
    await Permission.bluetooth.request();
    await Permission.bluetoothScan.request();
    await Permission.bluetoothConnect.request();
  }

  @override
  void initState() {
    super.initState();
    selectedWeather.addListener(_updateWeather);
    _requestPermission();

    _bluetooth.state.then((state) {
      setState(() => _bluetoothState = state.isEnabled);
    });

    _bluetooth.onStateChanged().listen((state) {
      setState(() => _bluetoothState = state.isEnabled);
    });
    _updateWeather();
  }

  @override
  void dispose() {
    selectedWeather.removeListener(_updateWeather);
    super.dispose();
  }

  Widget _infoDevice() {
    return ListTile(
      tileColor: Colors.black12,
      title: Text("Conectado a: ${_deviceConnected?.name ?? "ninguno"}"),
      trailing: _connection?.isConnected ?? false
          ? TextButton(
              onPressed: () async {
                await _connection?.finish();
                setState(() => _deviceConnected = null);
              },
              child: const Text("Desconectar"),
            )
          : TextButton(
              onPressed: _getDevices,
              child: const Text("Ver dispositivos"),
            ),
    );
  }

  Widget _listDevices() {
    return _isConnecting
        ? const Center(child: CircularProgressIndicator())
        : SingleChildScrollView(
            child: Container(
              color: Colors.grey.shade100,
              child: Column(
                children: [
                  ...[
                    for (final device in _devices)
                      ListTile(
                        title: Text(device.name ?? device.address),
                        trailing: TextButton(
                          child: const Text('conectar'),
                          onPressed: () async {
                            setState(() => _isConnecting = true);

                            try {
                              _connection = await BluetoothConnection.toAddress(device.address);
                            } catch (e) {
                              // print('===>>>>  Error de conexión: $e');
                            }
                            _deviceConnected = device;
                            _devices = [];
                            _isConnecting = false;

                            _receiveData();

                            setState(() {});
                          },
                        ),
                      )
                  ]
                ],
              ),
            ),
          );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.grey[300],
      appBar: AppBar(
        backgroundColor: Colors.grey[300],
        elevation: 0,
        title: Padding(
          padding: EdgeInsets.only(right: 20),
          child: Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              GestureDetector(
                onTap: () {
                  _showBluetoothDialog(context);
                },
                child: Icon(
                  _bluetoothState ? Icons.bluetooth_connected : Icons.bluetooth_disabled,
                  size: 40,
                  color: _bluetoothState ? Colors.blue[600] : Colors.grey[800],
                ),
              ),
              Container(
                decoration: BoxDecoration(
                  color: Colors.grey[800],
                  borderRadius: BorderRadius.circular(20),
                ),
                height: 50,
                width: 100,
                child:  ToggleButtonsSample(selectedWeather: selectedWeather),
              ),
            ],
          ),
        ),
      ),
      body: SafeArea(
        child: SingleChildScrollView(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              // welcome home
              Padding(
                padding: EdgeInsets.symmetric(
                  horizontal: horizontalPadding,
                  vertical: 25,
                ),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text("Smart Home"),
                    Text(
                      "Grupo4 - Arqui1",
                      style: TextStyle(
                        fontSize: 35,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    Text("Dispositivos Inteligentes"),
                  ],
                ),
              ),

              // Widget para mostrar la temperatura ambiente
              _buildTemperatureWidget(),

              const SizedBox(height: 30),

              // smart devices + carousel
              Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Padding(
                    padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
                  ),
                  CarouselSlider(
                    options: CarouselOptions(
                      height: 255.0,
                      initialPage: _currentIndex,
                      enlargeCenterPage: true,
                      onPageChanged: (index, reason) {
                        setState(() {
                          _currentIndex = index;
                        });
                      },
                      // Establecer límites para que el carrusel no pueda ir más allá del primer o último elemento
                      aspectRatio: 16 / 9,
                      viewportFraction: 0.75,
                      enableInfiniteScroll: true,
                    ),
                    items: mySmartDevices.asMap().entries.map((entry) {
                      int index = entry.key;
                      List smartDevice = entry.value;
                      return Builder(
                        builder: (BuildContext context) {
                          return Padding(
                            padding: EdgeInsets.symmetric(horizontal: 5.0), // Ajusta el margen horizontal aquí
                            child: SmartDeviceBox(
                              smartDeviceName: smartDevice[0],
                              iconPath: smartDevice[1],
                              powerOn: smartDevice[2],
                              onChanged: (value) => powerSwitchChanged(value, index),
                              disabled: (index == 0 || index == 4 || index == 5 || index == 6) && weather == "S",
                            ),
                          );
                        },
                      );
                    }).toList(),
                  ),
                  SizedBox(height: 5.0),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: mySmartDevices.asMap().entries.map((entry) {
                      int index = entry.key;
                      return Container(
                        width: 8.0,
                        height: 8.0,
                        margin: EdgeInsets.symmetric(horizontal: 2.0),
                        decoration: BoxDecoration(
                          shape: BoxShape.circle,
                          color: _currentIndex == index ? Colors.blue : Colors.grey,
                        ),
                      );
                    }).toList(),
                  ),
                  const SizedBox(height: 10),
                  GridView.builder(
                    shrinkWrap: true,
                    physics: NeverScrollableScrollPhysics(),
                    itemCount: 2,
                    padding: const EdgeInsets.symmetric(horizontal: 25),
                    gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                      crossAxisCount: 2,
                      childAspectRatio: 1 / 1,
                    ),
                    itemBuilder: (context, index) {
                      return CardExample(
                        titleCard: extras[index][0],
                        iconPath:  extras[index][1],
                        value: extras[index][0] == "Humedad" ? _humedity : _propane,
                      );
                    },
                  ),

                  GridView.builder(
                    shrinkWrap: true,
                    physics: NeverScrollableScrollPhysics(),
                    itemCount: 1,
                    padding: const EdgeInsets.symmetric(horizontal: 25),
                    gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                      crossAxisCount: 1,
                      childAspectRatio: 1 / 0.35,
                    ),
                    itemBuilder: (context, index) {
                      return CardHorizontal(
                        titleCard: "ttest",
                        iconPath:  "assets/door.png",
                        value:  _door,
                      );
                    },
                  ),
                ],
              ),
            ],
          ),
        ),
      ),

    );
  }
}