#include "DHT.h"
#define DHTPIN 9 
#define DHTTYPE DHT11
#include<Servo.h>
Servo myservo;
int servoPin=8 ;

DHT dht(DHTPIN,DHTTYPE);
//dht için millis
unsigned long dht_previousMillis=0;
int interval_dht = 5000;

//JOYSTICK DATA
int yAxis;
int xAxis;
int mappedxAxis;
int mappedy1;
int mappedy2;

//JOYSTICK millis()
unsigned long joy_previousMillis=0;
int interval_joy = 1000;

//L298 PINS
int dir11=2;
int dir12=3;
int dir21=4;
int dir22=5;
int speed1=6;
int speed2=7;

//BATTERY PERCENTAGE
int Battery = A0;
float Max_volt = 8.4;
float Min_volt = 5.2;
int yuzde = 0;
float gerilim = 0;
float gerilim_mapped;
char chr='+';

//battery millis()
unsigned long batt_previousMillis=0;
int interval_batt = 7000;

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin);
  myservo.write(60);
  
  dht.begin();
  pinMode(dir11,OUTPUT);
  pinMode(dir12,OUTPUT);
  pinMode(dir21,OUTPUT);
  pinMode(dir22,OUTPUT);
  pinMode(speed1,OUTPUT);
  pinMode(speed2,OUTPUT);

  analogWrite(speed1, 0);
  analogWrite(speed2, 0);
}

void loop() {
  //TEMP and HUMIDITY
  unsigned long currentMillis = millis(); //joystick print için 600ms delay

   if((unsigned long)(currentMillis - dht_previousMillis)>= interval_dht)
  {
    temp_and_hum();
    dht_previousMillis = millis();
  }
  char check;
  //JOYSTICK READ
  if(Serial.available()>=2)
  {
    check = Serial.read();
    if(check=='x')
    {
     xAxis = Serial.read();
    }
    if(check=='y')
    {
     yAxis = Serial.read();
    }

   }
 //JOYSTICK MAPPED
 mappedxAxis = map(xAxis,0,237,0,180);
 mappedy1 = map(yAxis,0,97,255,0);
 mappedy2 = map(yAxis,97,236,0,255);

    //SERVO WRITE
    if(mappedxAxis<=8)
    {
      mappedxAxis=8;
      myservo.write(8);
    }
    else if(mappedxAxis>8 && mappedxAxis<111)
    {
      myservo.write(mappedxAxis);
    }
    else if(mappedxAxis>=111)
    {
      mappedxAxis=111;
      myservo.write(111);
    }
   
  
  
  //DC MOTOR DIRECTIONS and SPEED
  if(!(yAxis==0 && (check != 'y' || check != 'x')))
  {
    if(yAxis<97)
    {
      ileri();
    }
    else if(yAxis>97){
      geri();
    }
    else if(yAxis=97){
      dur();
    }
  }
    
    
  
//BATTERY CODE
if((unsigned long)(currentMillis - batt_previousMillis)>= interval_batt)
  {
    gerilim = analogRead(Battery);
    gerilim_mapped = mapfloat(gerilim,0,1024,0,Max_volt);
    gerilim_mapped += 0.45;

    battery_display(gerilim_mapped, Min_volt, chr);
    batt_previousMillis = millis();
  }
}//END of loop()

void battery_display(long mappedd, long minn, char chr){
   
  
  if(gerilim_mapped > 7.72 && gerilim_mapped <= 7.95)
  {
    Serial.print(chr);
    Serial.println(5);
    }
  if(gerilim_mapped > 7.95 && gerilim_mapped <= 8.18)
  {
    Serial.print(chr);
    Serial.println(10);
    }
   if(gerilim_mapped > 8.18 && gerilim_mapped <= 8.41)
  {
    Serial.print(chr);
    Serial.println(15);
    }
    if(gerilim_mapped > 8.41 && gerilim_mapped <= 8.64)
  {
    Serial.print(chr);
    Serial.println(20);
    }
    if(gerilim_mapped > 8.64 && gerilim_mapped <= 8.87)
  {
    Serial.print(chr);
    Serial.println(25);
    }
    if(gerilim_mapped > 8.87 && gerilim_mapped <= 9.1)
  {
    Serial.print(chr);
    Serial.println(30);
    }
    if(gerilim_mapped > 9.1 && gerilim_mapped <= 9.33)
  {
    Serial.print(chr);
    Serial.println(35);
    }
    if(gerilim_mapped > 9.33 && gerilim_mapped <= 9.56)
  {
    Serial.print(chr);
    Serial.println(40);
    }
    if(gerilim_mapped > 9.56 && gerilim_mapped <= 9.79)
  {
    Serial.print(chr);
    Serial.println(45);
    }
   if(gerilim_mapped > 9.79 && gerilim_mapped <= 10.02)
  {
    Serial.print(chr);
    Serial.println(50);
    }
    if(gerilim_mapped > 10.02 && gerilim_mapped <= 10.25)
  {
    Serial.print(chr);
    Serial.println(55);
    }
    if(gerilim_mapped > 10.25 && gerilim_mapped <= 10.48)
  {
    Serial.print(chr);
    Serial.println(60);
    }
    if(gerilim_mapped > 10.48 && gerilim_mapped <= 10.71)
  {
    Serial.print(chr);
    Serial.println(65);
    }
    if(gerilim_mapped > 10.71 && gerilim_mapped <= 10.94)
  {
    Serial.print(chr);
    Serial.println(70);
    }
    if(gerilim_mapped > 10.94 && gerilim_mapped <= 11.17)
  {
    Serial.print(chr);
    Serial.println(75);
    }
    if(gerilim_mapped > 11.17 && gerilim_mapped <= 11.4)
  {
    Serial.print(chr);
    Serial.println(80);
    }
  if(gerilim_mapped > 11.4 && gerilim_mapped <= 11.63)
  {
    Serial.print(chr);
    Serial.println(85);
    }
   if(gerilim_mapped > 11.63 && gerilim_mapped <= 11.86)
  {
    Serial.print(chr);
    Serial.println(90);
    }
    if(gerilim_mapped > 11.86 && gerilim_mapped <= 12.09)
  {
    Serial.print(chr);
    Serial.println(95);
    }
    if(gerilim_mapped > 12.09 && gerilim_mapped <= 12.32)
  {
    Serial.print(chr);
    Serial.println(100);
    }
  }
void temp_and_hum(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print(t);
  Serial.print("C");
  Serial.print(";"); //Splitter
  Serial.print(h);
  Serial.println("%");
 }
void ileri(){
  
  digitalWrite(dir11,LOW);
  digitalWrite(dir12,HIGH);
  digitalWrite(dir21,LOW);
  digitalWrite(dir22,HIGH);
  analogWrite(speed1, mappedy1);
  analogWrite(speed2, mappedy1);
}
void geri(){
  digitalWrite(dir11,HIGH);
  digitalWrite(dir12,LOW);
  digitalWrite(dir21,HIGH);
  digitalWrite(dir22,LOW);
  analogWrite(speed1, mappedy2);
  analogWrite(speed2, mappedy2);
}
void dur(){
  digitalWrite(dir11,LOW);
  digitalWrite(dir12,LOW);
  digitalWrite(dir21,LOW);
  digitalWrite(dir22,LOW);
  analogWrite(speed1, 0);
  analogWrite(speed2, 0);
}
long mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
