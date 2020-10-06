#define Apin 2          // interrupt pin
#include <USBSabertooth.h>
USBSabertoothSerial C(Serial1); 
USBSabertooth ST(C, 128); 
const int nofsamples = 20;
const int funNo=1;
const int MaxTime=3000;
byte array[noofsamples];


const int num =1500;             // no: of Samples (pulses) to be taken
volatile unsigned long time[num];
boolean flag=false;


volatile unsigned int counter =0;




void Arise()
{ 
  
  time[counter++] = micros(); //code takes almost 8 micro seconds
                               //ie;(60*10^6)/(CPR*RPM) > 10 
}


void ramp(int v1,int v2)
{
  for(int i=0;i<noofsamples;i++)
  {
   int power=map(array[i],0,255,v1,v2);
   ST.motor(1,power);
   delay(MaxTime/noofsamples); 
  }
}


void setup() {
 // initilizing ramping array
 for(int i=0;i<noofsamples;i++)
 {
   array[i]=EEPROM[(funNo-1)*noofsamples+1+i];
 }
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(Apin, INPUT);
  digitalWrite(Apin,HIGH);
  //pinMode(Bpin, INPUT);
   attachInterrupt(0, Arise, RISING);
  ST.motor(1,1000);
  //attachInterrupt(0, Brise, CHANGE);
}






void loop() {
 
  //graph making part
   if (counter>=(num-10))
   { detachInterrupt(0);
     ST.motor(1,0);
       for(counter=0;counter<num;counter++)
       {
         Serial.println(time[counter]);
             
       }
       while(1);                //Stop
   }
  
 

 
}

  
 

/*
void Brise()
{
  if ( digitalRead(Apin) != digitalRead(Bpin) )
    angPos += increment;
  else
    angPos -= increment;
}
*/
