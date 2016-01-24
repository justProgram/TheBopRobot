// Arduino UNO R3
// https://arduino.cc/ <- Arduino website
  
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>
#include <Servo.h>
  
// Code Log: 
// 1/16/16 -> Blink program
// 1/19/16 -> BT program, Distance Sensor wiring, Tweaks
// 1/20/16 -> Distance Sensor programming, Buzzer programming w/ SV
// 1/21/16 -> More BT, Github??
  
// Pin Usage:
int rLED = 2;
int lLED = 4;
int btRX = A0;
int btTX = A1;
int buzz = 8;
int dECHO = 3;
int dTRIG = 11;
int pan = 9;
int tilt = 10;
int cal = 15;
  
SoftwareSerial bt(btRX,btTX);
  
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *mR = AFMS.getMotor(3);
Adafruit_DCMotor *mL = AFMS.getMotor(1);

Servo pS;
Servo tS;

int dist(){
 digitalWrite(dTRIG, HIGH);
 delayMicroseconds(1000);
 digitalWrite(dTRIG, LOW);
 int duration = pulseIn(dECHO, HIGH);
 int distance = (duration/2) / 29.1;
 return distance;  
}
  
void stopMotors() {
  mR->setSpeed(0);
  mL->setSpeed(0);
  mR->run(RELEASE);
  mL->run(RELEASE);
  delay(1000);
} 
  
void forward(int mLSpeed, int mRSpeed) {
  stopMotors();
  mR->setSpeed(mRSpeed-cal);
  mL->setSpeed(mLSpeed);
  mR->run(FORWARD);
  mL->run(FORWARD);
  // turn on motor
} 
  
void reverse(){
  stopMotors();
  mR->setSpeed(150);
  mL->setSpeed(150);
  mR->run(BACKWARD);
  mL->run(BACKWARD);
} 

void adjCal(int val){
  cal = cal+val;
}
  
void buzzer(){
  for(int i=0;i<3;i++){
     tone(buzz,440,1000);
     delay(500);
     noTone(buzz);
     delay(500);
  }
  noTone(buzz);
} 
  
int pAngle = 80;
boolean pDir = true;
  
void sweep(int deg){
  int maxAng = pAngle + deg;
  int minAng = pAngle - deg;
  int i;
  int d;
  for(i = pAngle;i<maxAng && i < 180 ;i++){
    pS.write(i);
    delay(15);
    d =  dist();
    if(d < 20 ){
      pAngle = i+10;
      pS.write(pAngle);
      return;
    }
    Serial.println(d);
  }
  for(;i>minAng && i >= 0 ;i--){
    pS.write(i);
    delay(15);    
    d =  dist();
    if(d < 20 ){
      pAngle = i-10;
      pS.write(pAngle);
      return;
    }
    Serial.println(d);
  }
  /*
  for(;i!=pAngle;i++){
    pS.write(i);
    delay(15);    
  }
  */
} 
  
void setup() {
  // put your setup code here, to run once:
  pinMode(rLED,OUTPUT);
  pinMode(lLED,OUTPUT);
  Serial.begin(9600);
  bt.begin(9600);
  AFMS.begin();
  pinMode(dTRIG,OUTPUT);
  pinMode(dECHO,INPUT);
  pS.attach(pan);
  tS.attach(tilt);
  pS.write(pAngle);
  tS.write(90);
} 
  
void LEDhigh(){
  digitalWrite(rLED,HIGH);
  digitalWrite(lLED,HIGH);
} 
  
void LEDlow(){
  digitalWrite(rLED,LOW);
  digitalWrite(lLED,LOW);
} 
  
void LEDright(){
  for(int i=0;i<3;i++){
    digitalWrite(rLED,HIGH);
    delay(1000);
    digitalWrite(rLED,LOW);
    delay(1000);
 } 
}  
   
void LEDleft(){
  for(int i=0;i<3;i++){
    digitalWrite(lLED,HIGH);
    delay(1000);
    digitalWrite(lLED,LOW);
    delay(1000);
 } 
}  


   
void loop() {
  // put your main code here, to run repeatedly:
  if(bt.available()){
    char c = bt.read(); 
    Serial.print("Read character :");
    Serial.println(c);
    bt.println(c);
    switch(c){
      case 'h':
        LEDhigh();
        break;
      case 'l' :
        LEDlow();
        break;
      case 'f' :
        forward(255,255);
        break;
      case 'r' :
        reverse();
        buzzer();
        break;
      case 's' :
        stopMotors();
        break; 
      case 'b' :
        buzzer();
        break;
      case ']' :
        LEDright();
        break;
      case '[' :
        LEDleft();
        break;
      case 'q' :
        forward(150,255);
        break;
      case 'w' :
        forward(255,150);
        break;
      case '+' :
        adjCal(5);
        break;
      case '-' :
        adjCal(-5);
        break;
      case 'a' :
        sweep(60);
        break;
    }
  }
  delay(100);
  Serial.println(dist());
  Serial.print(" cm");
}  
   
   
