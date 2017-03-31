//Include Servo Motor Control Library
#include <Servo.h>


//Define pin functions on Arduino
  //Limit Switch used to determine X-axis position of motor.
#define limitSwitch 9
  //X-Axis Stepper Motor Pins.
  //SEE https://learn.sparkfun.com/tutorials/easy-driver-hook-up-guide; http://www.schmalzhaus.com/EasyDriver/
#define XStp 3
#define XDir 2
#define XMS 4
  //Y-Axis Stepper Motor Pins.
#define YStp 6
#define YDir 7
#define YMS 5

  
//Declare variables for functions
char user_input;
int XStepperPosition;
int YStepperPosition;
int servoPosition;
int servoUpState = 50;
int servoDownState = 38;
int servoLoadState = 90;

//Declare Servo Motor
Servo servoMotor;



//Setup Fucntions
void setup() {
  //Call Default State for Stepper Driver to Avoid Stepper Motor Acting Up
  XStepperPinReset();
  YStepperPinReset();
  
  //Open Serial Port for TESTING - May not be necessary later.
  Serial.begin(9600);
  Serial.println("Begin input:");
  
  //Limit Switch Setup
  pinMode(limitSwitch, INPUT);

  //X-Axis Stepper Motor Pins
  pinMode(XStp, OUTPUT);
  pinMode(XDir, OUTPUT);
  pinMode(XMS, OUTPUT);

  //Y-Axis Stepper Motor Pins
  pinMode(YStp, OUTPUT);
  pinMode(YDir, OUTPUT);
  pinMode(YMS, OUTPUT);

  //Servo Motor Setup and Initial Position
  servoMotor.attach(8);
  servoLoad();  

  //Pull X-Axis Stepper to Home Position
  digitalWrite(XStp, LOW);
  YStepForward(1000);
  Serial.println("Y Ready");
  XStepForward(1000);
  Serial.println("X Ready");
  XStepperReset();
  Serial.println("Reset Done");

  
}

//Main Loop that actually executes while MicroController is powered.
void loop() {
  //INITIAL CODE FOR TESTING, DRIVES FUNCTIONALITY THROUGH CONSOLE, RATHER THAN AUTOMATING
  while(Serial.available()){
    user_input = Serial.read(); //Read user input and trigger appropriate function  
    if (user_input =='1') {
      Serial.println("Stepper Reset");
      XStepperReset();
      Serial.println(XStepperPosition);
    }
    else if(user_input =='2') {
      XStepForward(10);
      XStepperPinReset();
      Serial.println(XStepperPosition);
    }
    else if(user_input =='3') {
      XStepForward(100);
      XStepperPinReset();
      Serial.println(XStepperPosition);
    }
    else if(user_input =='4') {
      XStepBackward(10);
      XStepperPinReset();
      Serial.println(XStepperPosition);
    }
    else if(user_input =='5') {
      XStepBackward(100);
      XStepperPinReset();
      Serial.println(XStepperPosition);
    }
    else if(user_input =='6') {
      servoMotor.write(servoLoadState);
      Serial.println("LOAD STATE");
    }
    else if(user_input =='7') {
      servoMotor.write(servoUpState);
      Serial.println("UP STATE");  
    }
    else if(user_input =='8') {
      servoMotor.write(servoDownState);
      Serial.println("DOWN STATE");
    }
    else {
      Serial.println("Invalid option entered.");
    }
  }
}

//Define Functions Used in Main Loop

void XStepperPinReset() {
  digitalWrite(XStp, LOW);
  digitalWrite(XMS, HIGH);
  }
  
void YStepperPinReset() {
  digitalWrite(YStp, LOW);
  digitalWrite(YMS, HIGH);
}

void XStepForward(int y) {
  digitalWrite(XDir,HIGH);
  for(int x = 0; x < y; x++) {
    digitalWrite(XStp,HIGH);
    delay(1);
    digitalWrite(XStp,LOW);
    delay(1);
    XStepperPosition = XStepperPosition + 1;
  }
}

void XStepBackward(int y) {
  digitalWrite(XDir,LOW);
  for(int x = 0; x < y; x++) {
    digitalWrite(XStp,HIGH);
    delay(1);
    digitalWrite(XStp,LOW);
    delay(1);
    XStepperPosition = XStepperPosition - 1;
  }
}

void XStepperReset() {
  XStepForward(10);
  while(digitalRead(limitSwitch)) {
    XStepBackward(1);
    delay(1);
  }
  XStepperPosition = 0;
}

void YStepForward(int y) {
  digitalWrite(YDir,HIGH);
  for(int x = 0; x < y; x++) {
    digitalWrite(YStp,HIGH);
    delay(1);
    digitalWrite(YStp,LOW);
    delay(1);
    YStepperPosition = YStepperPosition + 1;
  }
}

void YStepBackward(int y) {
  digitalWrite(YDir, LOW);
  for(int x = 0; x < y; x++) {
    digitalWrite(YStp,HIGH);
    delay(1);
    digitalWrite(YStp,LOW);
    delay(1);
    YStepperPosition = YStepperPosition - 1;
  }
}

void servoLoad() {
  while(digitalRead(limitSwitch)){
    servoMotor.write(servoLoadState);
  }
  servoMotor.write(servoUpState);
}

