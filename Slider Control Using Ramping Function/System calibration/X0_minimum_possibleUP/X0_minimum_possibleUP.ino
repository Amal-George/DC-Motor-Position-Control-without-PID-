
#include <digitalWriteFast.h>

#define Apin 2   
#define Bpin 3// interrupt pin
#include <USBSabertooth.h>
USBSabertoothSerial C(Serial1); 
USBSabertooth ST(C, 128); 
#include <math.h>

volatile long counter =0;


boolean k=false;


void setup() {
   Serial.begin(9600);
   Serial1.begin(9600);
    pinModeFast(Apin, INPUT);
    pinModeFast(Bpin, INPUT);
   attachInterrupt(0, Arise, RISING);
}

int milli,Setpoint,sign=1;
long i=0,time1;
const int num =1800;
float input=0,Speed,LastInput;unsigned long array[num];
void loop() {

  
    if(Serial.available())
 {milli=Serial.parseInt();
   if (Serial.read()=='-') sign*=-1;
   Setpoint=milli*sign;
  
   
   ST.motor(1,Setpoint);//k=true;
  // time1=millis();
 }
// input=counter/252.84158;
// Speed=LastInput-input;
// LastInput=input;
// if(k&&input>0){
//array[i++]=Speed;
//if (i>=num||millis()>time1+1000)
//{ST.motor(1,0);for (i=0;i<num;i++) 
//Serial.println(array[i]);while(1);}
// }
 if(i>=num-10){detachInterrupt(0);
ST.motor(1,0);for (int i=0;i<num;i++) 
Serial.println(array[i]);while(1);}
}
void Arise()
{ if(i>=num-10){detachInterrupt(0);
ST.motor(1,0);for (int i=0;i<num;i++) 
Serial.println(array[i]);while(1);}
  array[i++]=micros();
    
}

