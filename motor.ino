
#define BLYNK_PRINT Serial
// replace with blynk device settings from device screen
// see https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/quickstart-device-code-overview
#define BLYNK_TEMPLATE_ID "TMPLWxxxxxxxxxx"
#define BLYNK_DEVICE_NAME "devicex"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxx"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>



//Motor Left
const int motorLin1  = D4;  
const int motorLin2  = D3;  
const int motorLPWM  = D5;  

//Motor Right
const int motorRin1  = D2; 
const int motorRin2  = D1; 
const int motorRPWM  = D6; 

const int BlynkSpeedometer = V0;
const int BlynkReverseLed = V1;


// chenag with you wifi settings
const char* ssid = "wifi-ssid"; 
const char* password = "wihi-passwrd"; 
const char auth[] = BLYNK_AUTH_TOKEN;

//This will run only one time.
void setup(){
 
    //Set pins as outputs
    pinMode(motorLin1, OUTPUT);
    pinMode(motorLin2, OUTPUT);
    pinMode(motorLPWM, OUTPUT);

    pinMode(motorRin1, OUTPUT);
    pinMode(motorRin2, OUTPUT);
    pinMode(motorRPWM, OUTPUT);

    //And this code will stop motors
    motorLStop();
    motorRStop();
    
    //Test motors
    motorLClockWise(255);
    motorRClockWise(255);
    delay(50); 
    motorLCounterClockWise(255);
    motorRCounterClockWise(255);
    delay(50);
    motorLStop();
    motorRStop();

    // Debug console 
    Serial.begin(115200);
    Blynk.begin(auth, ssid, password); 
  
}

void motorRStop() {
    digitalWrite(motorRin1, LOW);
    digitalWrite(motorRin2, LOW);
}

void motorRClockWise(int speed) {
    analogWrite(motorRPWM, speed);
    digitalWrite(motorRin1, HIGH);
    digitalWrite(motorRin2, LOW);
}

void motorRCounterClockWise(int speed) {
    analogWrite(motorRPWM, speed);
    digitalWrite(motorRin1, LOW);
    digitalWrite(motorRin2, HIGH);
}

void motorLStop() {
    digitalWrite(motorLin1, LOW);
    digitalWrite(motorLin2, LOW);
}

void motorLClockWise(int speed) {
    analogWrite(motorLPWM, speed);
    digitalWrite(motorLin1, HIGH);
    digitalWrite(motorLin2, LOW);
}

void motorLCounterClockWise(int speed) {
    analogWrite(motorLPWM, speed);
    digitalWrite(motorLin1, LOW);
    digitalWrite(motorLin2, HIGH);
}

void stopEngine() {
  motorLStop();
  motorRStop();  
}

void motorLMove(int rot, int speed){
  if (rot > 0) {
    motorLClockWise(speed);
  } else {
    motorLCounterClockWise(speed);
  }
}

void motorRMove(int rot, int speed){
  if (rot > 0) {
    motorRClockWise(speed);
  } else {
    motorRCounterClockWise(speed);
  }
}

void loop(){ 
  Blynk.run();  
} 

// change virtual pin to your joystick controller pin number
BLYNK_WRITE(V4)
{
  // Set incoming value from pin V4 to a variable
  int x = param[0].asInt(); 
  int y = param[1].asInt();  

  int rotRL =  x > 128 ? 1 : -1;
  int rotFB =  y > 128 ? 1 : -1;


  int speeda = abs(y - 128) * 2;
  int angel  = abs(x - 128) * 2;
  int speedb = abs(speeda - angel);
  
  Serial.println("x :"+String(x)+ " y: "+String(y));
  Serial.println("rotRL :"+String(rotRL)+ " rotFB: "+String(rotFB)+ " speeda: "+String(speeda)+" speedb: "+String(speedb)+" angel: "+String(angel));
  if (speeda < 25 && speedb < 25) {
    stopEngine();
  } else {
    int speedL;
    int speedR;
    if (rotRL > 0) {
      speedL = speeda;
      speedR = speedb;
    } else {
      speedL = speedb;
      speedR = speeda;
    }
    motorLMove(rotFB, speedL);
    motorRMove(rotFB, speedR);
  }

  Blynk.virtualWrite(BlynkSpeedometer, std::max(speeda, speedb));
  Blynk.virtualWrite(BlynkReverseLed, rotFB > 0 ? 0 : 1);
}
