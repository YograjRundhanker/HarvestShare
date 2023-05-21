float offset =20;// set the correction offset value
float energy=0;
//#include <Servo.h>
//Servo servo01;      // variable to store the servo position
//int servo1Pos;
//int servo1PPos;

unsigned long lastMillis = 0;
#define Relay1 8
void setup() 
{
  
  Serial.begin(9600);
  //servo01.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, 1);
  while(Serial.available()==0){}
  //int t=Serial.parseInt();
}

void loop() 
{
  float x=random(10,100);
  delay(x);
  float  dec_volt = random(00,99)/100.0;
  float voltage = random(10.00,13.00)+dec_volt;
  energy+=(voltage*x*random(0.00,2.00))/3600000;
  
  Serial.print("Voltage: ");
  Serial.print(voltage);//print the voltge
  Serial.println("V");

  Serial.print("Energy consumed: ");
  Serial.print(energy);//print the voltge
  Serial.println("wh");  
  
  if(voltage<12)
  {
    digitalWrite(Relay1, 0);
    //servo01.write(0);
  } 
  else
  {
    digitalWrite(Relay1, 1);
    //servo01.write(180);
  }

}
