#include <digitalWriteFast.h>

#define Apin 2   
#define Bpin 3// interrupt pin
#include <USBSabertooth.h>
USBSabertoothSerial C(Serial1); 
USBSabertooth ST(C, 128); 

 #include <PID_v1.h>



//Define Variables we'll be connecting to
 double Setpoint=0,  Output;
double Input=0;
//Specify the links and initial tuning parameters
int i=100;
//double Kp=i, Ki=0, Kd=0;
double Kp=i*0.5, Ki=i*2.5, Kd=i/20;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

volatile long counter =0;





void setup() {
   Serial.begin(9600);
   Serial1.begin(9600);
    pinModeFast(Apin, INPUT);
    pinModeFast(Bpin, INPUT);
   attachInterrupt(0, Arise, RISING);
   ST.freewheel(false);
   myPID.SetMode(AUTOMATIC);
   myPID.SetOutputLimits(-2047,2047);
   myPID.SetSampleTime(100); 
  
 
  
}
int milli;
int sign=1;
void loop() {
  
  if(Serial.available())
 {milli=Serial.parseInt();
   if (Serial.read()=='-') sign*=-1;
   
  Setpoint=milli*sign;
  
 } 

 Input=counter/252.84158; 
 Serial.println(Input);
 myPID.Compute();
 //Serial.print(Output);Serial.print(',');Serial.println(Input);
ST.motor(1,Output);
 
}

  
 


void Arise()
{
  if (  digitalReadFast(Bpin)==digitalReadFast(Apin) )
    counter ++;
  else
    counter --;
    
}

