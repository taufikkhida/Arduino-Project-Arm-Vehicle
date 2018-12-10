#include <Servo.h>

const int in1 = 5;
const int in2 = 6;
const int in3 = 3;
const int in4 = 4;
const int enA = 7;
const int enB = 2;
int mSpeedL, mSpeedR;
String dataIn;
unsigned long int jarak = 0;
bool state = true;

Servo A,B,C,D;

void setup() {
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  
  Serial.begin(9600);
  //Serial3.begin(9600);
  
  analogWrite(enA, mSpeedL);
  analogWrite(enB, mSpeedR);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // ambil data
  while(Serial.available()>0) {
    delay(2);
    char s = Serial.read();
    if (s == 'X') break;
    if (s != "") dataIn += s;
  }
  if(dataIn.length() > 0) {
    //Serial.println(dataIn);
    parseString(dataIn);
    dataIn = "";
    jarak = millis();
  }
  // akhir dari ambil data
  if((millis() - jarak) > 2000 && state == true) {
    A.detach();
    B.detach();
    C.detach();
    D.detach();
    digitalWrite(LED_BUILTIN, HIGH);
    state = false;
  }
}

void parseString(String data) {
  int index = 0;
  String value[11];
  for(char s : data) {
    if(s == ',') {
      index++;
    } else {
      value[index] += s;
    }
  }
  proses(value);
}

int tempA, tempB, tempC, tempD;
void proses(String data[]) {
  // kondisi roda
 digitalWrite(in1, (data[1] == "true" || data[4] == "true"));
 digitalWrite(in2, (data[2] == "true" || data[3] == "true"));
 digitalWrite(in3, (data[1] == "true" || data[2] == "true"));
 digitalWrite(in4, (data[3] == "true" || data[4] == "true"));
 // ambil nilai servo
 int valA, valB, valC, valD;
 valA = map(data[5].toInt(),0,100,0,180);
 valB = map(data[6].toInt(),0,100,0,180);
 valC = map(data[7].toInt(),0,100,0,180);
 valD = map(data[8].toInt(),0,100,0,180);
 
 if(tempA != valA || tempB != valB || tempC != valC || tempD != valD) {
  A.attach(A0);
  B.attach(A1);
  C.attach(A2);
  D.attach(A3);
  
  A.write(valA);
  B.write(valB);
  C.write(valC);
  D.write(valD);

  tempA = valA;
  tempB = valB;
  tempC = valC;
  tempD = valD;

  jarak = millis();
  state = true;
 digitalWrite(LED_BUILTIN, LOW);
 }
 
 // ambil nilai kecepatan motor
 mSpeedL = (map(data[9].toInt(),0,100,0,255));
 mSpeedR = (map(data[10].toInt(),0,100,0,255));
 analogWrite(enA, mSpeedL);
 analogWrite(enB, mSpeedR);
}

