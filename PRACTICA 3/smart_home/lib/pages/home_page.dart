import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:smart_home/util/smart_devices_box.dart';

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

  // padding constants
  final double horizontalPadding = 40;
  final double verticalPadding = 25;

  // list of smart devices
  List mySmartDevices = [
    // [ name, path, status ]
    ["Cuarto 1", "assets/light-bulb.png", false],
    ["Cuarto 2", "assets/light-bulb.png", false],
    ["Baño", "assets/light-bulb.png", false],
    ["Sala", "assets/light-bulb.png", false],
    ["Cocina", "assets/light-bulb.png", false],
    ["Jardín", "assets/light-bulb.png", false],
  ];

  // power button switched
  void powerSwitchChanged(bool value, int index) {
    setState(() {
      mySmartDevices[index][2] = value;
    });
    
    // Calcular el valor a enviar basado en el índice y el patrón especificado
    int valueToSend = index * 2 + (value ? 1 : 2); // Si el interruptor está activado, se suma 1, de lo contrario se suma 2
    String dataToSend = valueToSend.toString();
    _sendData(dataToSend);
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
  // ========= Bluetooth ==========

  void _getDevices() async {
    var res = await _bluetooth.getBondedDevices();
    setState(() => _devices = res);
  }

  void _receiveData() {
    _connection?.input?.listen((event) {
      if (String.fromCharCodes(event) == "p") {
        setState(() => times = times + 1);
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

    _requestPermission();

    _bluetooth.state.then((state) {
      setState(() => _bluetoothState = state.isEnabled);
    });

    _bluetooth.onStateChanged().listen((state) {
      setState(() => _bluetoothState = state.isEnabled);
    });
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
          padding: EdgeInsets.only(right: 20), // Ajusta el espacio entre el texto y el borde derecho
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
                  color: _bluetoothState ? Colors.green : Colors.grey[800],
                ),
              ),
              Text(
                " - ",
                style: TextStyle(
                  fontSize: 25,
                  color: Colors.black,
                ),
              ),
            ],
          ),
        ),
      ),
      body: SafeArea(
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

            const SizedBox(height: 30),

            // smart devices + grid
            Expanded(
              child: GridView.builder(
                itemCount: mySmartDevices.length,
                padding: const EdgeInsets.all(5),
                gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: 2,
                  childAspectRatio: 1 / 1.3,
                ),
                itemBuilder: (context, index) {
                  return SmartDeviceBox(
                    smartDeviceName: mySmartDevices[index][0],
                    iconPath: mySmartDevices[index][1],
                    powerOn: mySmartDevices[index][2],
                    onChanged: (value) => powerSwitchChanged(value, index),
                  );
                },
              ),
            )
          ],
        ),
      ),
    );
  }
}
