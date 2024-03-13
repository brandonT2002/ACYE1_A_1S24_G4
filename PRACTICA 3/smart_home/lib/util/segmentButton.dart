import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

const List<Widget> icons = <Widget>[
  Icon(Icons.sunny),
  Icon(Icons.settings),
];

class ToggleButtonsSample extends StatefulWidget {
  final ValueNotifier<List<bool>> selectedWeather;

  const ToggleButtonsSample({Key? key, required this.selectedWeather}) : super(key: key);

  @override
  State<ToggleButtonsSample> createState() => _ToggleButtonsSampleState();
}

class _ToggleButtonsSampleState extends State<ToggleButtonsSample> {
  bool vertical = false;

  @override
  Widget build(BuildContext context) {
    final ThemeData theme = Theme.of(context);

    return Scaffold(
      backgroundColor: Colors.grey[300],
      body: Center(
        child: SingleChildScrollView(
          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: <Widget>[
              // ToggleButtons with a single selection
              ToggleButtons(
                direction: vertical ? Axis.vertical : Axis.horizontal,
                onPressed: (int index) {
                  setState(() {
                    // The button that is tapped is set to true, and the others to false.
                    for (int i = 0; i < widget.selectedWeather.value.length; i++) {
                      widget.selectedWeather.value[i] = i == index;
                    }
                    // Notificar a los listeners que el valor ha cambiado
                    widget.selectedWeather.notifyListeners();
                  });
                },
                borderRadius: const BorderRadius.all(Radius.circular(8)),
                selectedBorderColor: Colors.grey[900],
                selectedColor: Colors.white,
                fillColor: Colors.grey[900],
                color: Colors.grey[900],
                isSelected: widget.selectedWeather.value,
                children: icons,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
