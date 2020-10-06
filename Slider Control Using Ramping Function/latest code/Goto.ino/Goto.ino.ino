#include "Goto.h"
#include <digitalWriteFast.h>
#include <USBSabertooth.h>
#define Apin 2
#define Bpin 3// interrupt pin
volatile long counter = 0;
//volatile long array[1800];
USBSabertoothSerial C(Serial1);
USBSabertooth ST(C, 128);

////Define Shared Variables we'll be connecting to
double Setpoint = 0;
double Input = 0;
int pwm = 0;
//RampPositioning(double*currentpos, int*speedatinstant, double*final, int pup = 2047, int plwr = -2047, int minimumspeed = 200)
RampPositioning XX(&Input, &pwm,  2047, -2047, 150);
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  pinModeFast(Apin, INPUT);
  pinModeFast(Bpin, INPUT);
  attachInterrupt(0, Arise, RISING);
}

bool flag = true;
int milli;       // for data intake millimeter
int sign = 1;

void loop()
{ Input = counter / 252.84158;
  XX.Startpositioning(flag, Setpoint, 0);
  if (Serial.available())
  { milli = Serial.parseInt();
    if (Serial.read() == '-') sign *= -1;
    Setpoint = milli * sign;
  }
//  if (counter>=1790){ST.motor(1,0);
//  for(int i=0; i<1790;i++) Serial.println(array[i]);
//  while(1);}
  Serial.print(Input);  Serial.print(','); Serial.println(Setpoint);
  ST.motor(1, pwm);
}

void Arise()
{// if (counter<1790) array[counter]=micros();
  if (  digitalReadFast(Bpin) == digitalReadFast(Apin) )
    counter ++;
  else
    counter --;

}


