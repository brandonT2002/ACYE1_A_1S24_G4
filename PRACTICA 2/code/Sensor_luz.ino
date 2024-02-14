#define outPin 8
#define s0 9
#define s1 10
#define s2 11
#define s3 12

// Variables
int red, green, blue;
String color = "";

void setup(){
  Serial.begin(9600);
  
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);
  
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);
}
void loop(){
  getColor();
  printData();
  delay(1000);
}


void printData(void){ 
  Serial.print("red= ");
  Serial.print(red);
  Serial.print("   green= ");
  Serial.print(green);
  Serial.print("   bluee= ");
  Serial.print(blue);
  Serial.print (" - ");
  Serial.print (color);
  Serial.println (" detected!");
}

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

void getColor(){  
  readRGB();
  if(red>7  && red<11  && green>17 && green<25 && blue>13 && blue<19) Serial.println("RED");
  else if(red>22 && red<28  && green>11 && green<16 && blue>6  && blue<12) Serial.println("BLUE");
  else if(red>3  && red<8   && green>4  && green<10 && blue>9  && blue<13) Serial.println("YELLOW");
  else  Serial.println("COLOR NOT FOUND");
}

