int offset =20;// set the correction offset value

#include <Servo.h>
Servo servo01;      // variable to store the servo position
int servo1Pos;
int servo1PPos;

unsigned long lastMillis = 0;
#define Relay1 8
void setup()
{
  Serial.begin(9600);
  servo01.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, 1);
}

void loop() 
{  
  int volt = analogRead(A0);// read the input
  double voltage = map(volt,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
  
  voltage /=100;// divide by 100 to get the decimal values
  Serial.print("Voltage: ");
  Serial.write("Voltage: ");
  Serial.print(voltage);//print the voltge
  Serial.println("V");
  delay(10);

  if (millis() - lastMillis > 1000UL)
  {
    Serial.print("Voltage: ");
    Serial.print(voltage);//print the voltge
    Serial.println("V");
    lastMillis = millis();
  }


  if(voltage<12)
  {
    digitalWrite(Relay1, 0);
  } 
  else
  {
    digitalWrite(Relay1, 1);
    //Serial.print("Relay on");
    //servo1Pos=179;
    //servo01.write(180);
    //Serial.print("Servo position= 180");
    //servoFunction();
  }
}