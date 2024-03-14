import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class CardHorizontal extends StatelessWidget {
  final String titleCard;
  final String iconPath;
  final String value;

  CardHorizontal({
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
        padding: EdgeInsets.all(30.0),
        decoration: BoxDecoration(
          color: Colors.grey[200],
          borderRadius: BorderRadius.circular(10.0),
        ),
        child: Row( // Cambio a Row en lugar de Column
          mainAxisAlignment: MainAxisAlignment.spaceBetween, // Alinea los elementos al centro
          children: [
            Image.asset(
              iconPath,
              height: 40,
              color: Colors.grey[900],
            ),
            SizedBox(width: 80.0), // Espacio entre los elementos
            Row( // Columna para el texto
              crossAxisAlignment: CrossAxisAlignment.center, // Alinea el texto a la izquierda
              children: [
                Text(
                  "Estado: ",
                  style: TextStyle(
                    fontSize: 16.0,
                  ),
                ),
              ],
            ),

        Container(
          // Contenedor para el texto de la etiqueta
          padding: EdgeInsets.symmetric(horizontal: 12.0, vertical: 6.0),
          decoration: BoxDecoration(
            color: Colors.white, // Color de fondo del texto
            borderRadius: BorderRadius.circular(10.0), // Borde redondeado
          ),
          child: Text(
            value,
            style: TextStyle(
              fontWeight: FontWeight.bold,
              fontSize: 18.0,
              color: Colors.blue, // Color del texto
            ),
          ),
        )
          ],
        ),
      ),
    );
  }
}
