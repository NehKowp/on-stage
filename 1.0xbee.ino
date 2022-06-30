#include <FastLED.h>
#include <SoftwareSerial.h>

// En gros meme programme que le 0.5uss mais avec le xbee (et les coraux) 

// UltraSonicSensors
const byte trigD = 9; //9
const byte echoD = 11; //11
const byte trigH = 12; //12
const byte echoH = 10; //10

const byte stopArduino = 7; // obstacle
const byte totheOpenMV = 6; // coucou / change cible
const byte emergencyStop = 5; //Coupe tortue

byte obst = 0; 
byte coucou = 0;
byte stops = 0;

// Leds
const byte num_leds=10;
const byte data_pin=5;
int lumi=120;
CRGB leds[num_leds];

// Xbee
#include <SoftwareSerial.h>
const byte rxPin = 2; // A connecter a la broche DOUT
const byte txPin = 3; // A connecter a la broche DIN
String message = "";
SoftwareSerial xbee(rxPin, txPin);

void setup()
{
  Serial.begin(38400);
  delay(200);
  Serial.println("Meh");
  xbee.begin(9600);
  pinMode(stopArduino, OUTPUT);
  pinMode(totheOpenMV, OUTPUT);
  pinMode(emergencyStop, OUTPUT);
  // Leds
  FastLED.addLeds<NEOPIXEL, data_pin>(leds, num_leds);
  for(int i=0;i<num_leds;i++){
    leds[i]=CRGB::Black;
  }
  FastLED.show();
  digitalWrite(totheOpenMV, LOW);
}
void loop() 
{
  int distanceD = distance(trigD, echoD);
  int distanceH = distance(trigH, echoH);

  Serial.print(distanceD);
  Serial.print(" - ");
  Serial.print(distanceH);
  Serial.print(" ");
  //Serial.print(" - Obst:");
  
  if(distanceD <= 25){
    obst = 1;
    digitalWrite(stopArduino, HIGH);

    // Leds
    for(int i=0;i<num_leds;i++){
      leds[i]=CRGB::Red;
    }
    FastLED.show();

    // Xbee
    if(coucou == 0){
      xbee.println(9999);
      //Serial.println("999");
    } else {
      xbee.println(7777);
      //Serial.println("777");
    }
    
    Serial.print("Obstacle ");
    delay(50);
  } else {
    digitalWrite(stopArduino, LOW);
    obst = 0;
    for(int i=0;i<num_leds;i++){
      leds[i]=CRGB::Red;
    }
    FastLED.show();
    delay(100);
  }
  if(distanceH <= 20){
    digitalWrite(emergencyStop, HIGH);
    stops = 1;
    //Serial.print("Stop complet - ");
  }
  if(distanceH <= 40){
    coucou = 1;
    digitalWrite(totheOpenMV, HIGH);
    //Serial.print("Coucou - ");
  }else{
    
  }
  if(coucou == 1){
    Serial.print("coucou ");
  }
  if(stops == 1){
    Serial.print("stop ");
  }
  Serial.println();
}

float distance(int _trig, int _echo){
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
  int moyenne = 0;
  for(int i=0;i<4;i++){
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);
    long _tmps = pulseIn(_echo, HIGH, 10000);
    if(_tmps == 0){
      moyenne += 999;
    } else {
      moyenne += 0.25 * _tmps*0.034/2;
    }
  }
  return moyenne;
}
