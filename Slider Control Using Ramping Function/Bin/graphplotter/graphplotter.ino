#define Apin 3
#define Bpin 2

#include <USBSabertooth.h>
 const int num =1500;
volatile unsigned long time[num];
boolean flag=false;
volatile unsigned int er=0;

volatile unsigned int counter =0;

USBSabertoothSerial C(Serial1); 
USBSabertooth ST(C, 128); 



void setup() {
 // SabertoothTXPinSerial.begin(9600);
  Serial.begin(9600);
 Serial1.begin(9600);
  pinMode(Apin, INPUT);
  //pinMode(Bpin, INPUT);
  
  attachInterrupt(1, Arise, RISING);
  //attachInterrupt(0, Brise, CHANGE);
}

void loop() {
 
 
 if(!flag){
   ST.motor(1,500);}
   
   else
   { detachInterrupt(1);
     ST.motor(1,0);
       for(counter=0;counter<num;counter++)
       {Serial.println(time[counter]);
       /* Serial.print(',');
        Serial.println(pos[counter]);*/
       
       }
       while(1);                //Stop
   }
  
 

 
}

void Arise()
{ /* if(millis()>pretime+10) 
   {pretime=time[counter]=millis();
   pos[counter]=angPos;
   counter++;
    }*/
   
    time[counter] = micros()+er;
    er+=er;
  counter++;
 if(counter>=num-20){flag=true;}
  
  
//  if (digitalRead(Apin) == digitalRead(Bpin) )
 
    //angPos += increment;
//  else
//    angPos -= increment;
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
