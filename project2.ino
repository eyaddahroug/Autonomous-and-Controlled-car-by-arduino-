#include <SoftwareSerial.h>   //Bluetooth
#include "Servo.h"           // servo motor

//Motors
#define en1     11
#define en2     6
#define motorA1 10
#define motorA2 9
#define motorB1 7
#define motorB2 8

//bluetooth
#define bt_tx A1
#define bt_rx A0
SoftwareSerial BT(bt_tx, bt_rx);
char bl = 'S';
bool autoMode = false;

//SERVO
Servo myServo;
 int pos = 0;
 int dir = 1;


//flame
int read;
#define f   2
#define bu  4


//ultrasonic
#define Trig    A3
#define Echo    A4
float t; //variable of travel time
float distance; //variable of distance in cm

//flame Function
void flame_buzzer() {
  digitalWrite(bu, digitalRead(f));
}


void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  //Motors
  pinMode(en1, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorB1, OUTPUT);

  // start stop
  digitalWrite(motorA1, 0);
  digitalWrite(motorA2, 0);
  digitalWrite(motorB1, 0);
  digitalWrite(motorB2, 0);

  //Bluetooth
  pinMode(bt_tx, INPUT);
  pinMode(bt_rx, OUTPUT);

  //Flame
  pinMode(f , INPUT);
  pinMode(bu , OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), flame_buzzer, CHANGE);

  //ultrasonic
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  //Servo 
  myServo.attach(A2);

}

void loop() {
  
  //speed
  analogWrite(en1 ,900);
  analogWrite(en2 , 900);

  //Bluetooth 
if(BT.available()){
      bl = BT.read();
      Serial.write("chr: ");
      Serial.println(bl);
    }
if (bl == 'A'){
      autoMode = true;
      Serial.println("Auto mode enabled");
    }
if (bl == 'M'){
      autoMode = false;
      bl = 'S';
      Serial.println("Auto mode disabled");
    }

//MANUALMODE
if(autoMode == false)
    { 
    switch(bl)
     {
      case 'L': 
        {
        Serial.println("Going Left");
        digitalWrite(motorA1, 0);
        digitalWrite(motorA2, 1);
        digitalWrite(motorB1, 1);
        digitalWrite(motorB2, 0);
        break;
     }

      case 'R': {
        Serial.println("Going RIGHT");
        digitalWrite(motorA1, 1);
        digitalWrite(motorA2, 0);
        digitalWrite(motorB1, 0);
        digitalWrite(motorB2, 1);
        break;
      }

      case 'F':{
        Serial.println("Driving Forward");
        digitalWrite(motorA1, 0);
        digitalWrite(motorA2, 1);
        digitalWrite(motorB1, 0);
        digitalWrite(motorB2, 1);
        break;
      }

      case 'B' :{
        Serial.println("Driving Backwards");
        digitalWrite(motorA1, 1);
        digitalWrite(motorA2, 0);
        digitalWrite(motorB1, 1);
        digitalWrite(motorB2, 0);
        break;
      }

      case 'S': {
        //Serial.println("Stopping");
        digitalWrite(motorA1, 0);
        digitalWrite(motorA2, 0);
        digitalWrite(motorB1, 0);
        digitalWrite(motorB2, 0);
        break;
      }
    }

  }

//AUTOMODE
if(autoMode == true){

//Servo
    if (pos == 180)
      dir = -1;
    
    else if (pos == 0)
      dir = 1;
    
    pos += dir;
    myServo.write(pos);

//ultrasonic
    digitalWrite(Trig,0);
    delayMicroseconds(2);
    digitalWrite(Trig,1);
    delayMicroseconds(10);
    digitalWrite(Trig,0);
    delayMicroseconds(2);
    digitalRead(Echo);
    t = pulseIn(Echo , 1);
    distance = t * (0.017);
    Serial.println(distance);


//Forward
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, 1);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, 1);

    digitalWrite(bu ,0);
  
if(distance <= 20){

  digitalWrite(bu , 1);
    //Back
    digitalWrite(motorA1, 1);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, 1);
    digitalWrite(motorB2, 0);
    delay(100);

    //Stop
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, 0);
    delay(100);

    //Right
    digitalWrite(motorA1, 1);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, 1);
    delay(100);

  }
 }
}

