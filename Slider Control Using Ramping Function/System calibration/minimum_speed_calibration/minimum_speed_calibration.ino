
#include <USBSabertooth.h>
USBSabertoothSerial C(Serial1);
USBSabertooth ST(C, 128);

////Define Shared Variables we'll be connecting to
//RampPositioning(double*currentpos, int*speedatinstant, double*final, int pup = 2047, int plwr = -2047, int minimumspeed = 200)
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  }

int milli;       // for data intake millimeter
int sign = 1;
int i=0;
void loop()
{ 
  
  if (Serial.available())
  { milli = Serial.parseInt();
    if (Serial.read() == '-') sign *= -1;
    i = milli * sign;
  }
  Serial.println(i);
ST.motor(1, i);
}


