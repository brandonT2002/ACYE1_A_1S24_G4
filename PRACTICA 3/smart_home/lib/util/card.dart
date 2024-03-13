import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class CardExample extends StatelessWidget {
  final String titleCard;
  final String iconPath;
  final String value;

  CardExample({
    Key? key,
    required this.titleCard,
    required this.iconPath,
    required this.value,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(16.0),
      child: Container(
        padding: EdgeInsets.all(2.0),
        decoration: BoxDecoration(
          //border: Border.all(),
          color: Colors.grey[200],
          borderRadius: BorderRadius.circular(10.0),
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          mainAxisSize: MainAxisSize.min,
          children: [
            Image.asset(
              iconPath,
              height: 40,
              color: Colors.grey[900],
            ),
            SizedBox(height: 10.0),
            Text(
              titleCard,
              style: TextStyle(
                fontSize: 16.0,
              ),
              textAlign: TextAlign.center,
            ),
            SizedBox(height: 10.0),
            Text(
              value,
              style: TextStyle(
                  fontWeight: FontWeight.bold,
                  fontSize: 18.0),
              textAlign: TextAlign.center,
            ),
          ],
        ),
      ),
    );
  }
}