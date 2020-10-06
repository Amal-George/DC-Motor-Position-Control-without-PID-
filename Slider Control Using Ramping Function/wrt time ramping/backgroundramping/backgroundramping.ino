#include "ClassRamping.h"

#define Apin 2          // interrupt pin
#include <USBSabertooth.h>
USBSabertoothSerial C(Serial1); 
USBSabertooth ST(C, 128); 
/* for taking reading*/
const int num =1500;             // no: of Samples (pulses) to be taken
volatile unsigned long time[num];
volatile unsigned int counter =0;

void Arise()
{ 
  
  time[counter++] = micros(); //code takes almost 8 micro seconds
                               //ie;(60*10^6)/(CPR*RPM) > 10 
 if (counter>=(num-10))
    detachInterrupt(0);
}
Ramp Saber;
void setup() 
{ 
  Serial.begin(9600);
 Serial1.begin(9600);
  pinMode(Apin, INPUT);
//  
  digitalWrite(Apin,HIGH);
 
  attachInterrupt(0, Arise, RISING);
  
  
}





int k=0;
void loop()
{Saber.rmp(ST);
if(Serial.available()>0)

{ char c=Serial.read();
 if (c=='a') k=2047;
 if (c=='b') k=-2047;
 if (c=='c') k=0;
 if (c=='1') k=1000;
 if (c=='2') k=-1000;
}
 Saber.SetSpeed(k,1);
//Serial.println(k);
  if (counter>=(num-10))
   { detachInterrupt(0);
     ST.motor(1,0);
       for(int i=0;i<counter;i++)                                                                                                                                                                                                                                                                                                                                                                                                                 
       {
         Serial.println(time[i]);
             
       }
       while(1);                //Stop
   }


 
}

  
 
