//initilize board with ramping functions
#define fun(x) ((x)-sin(x))
#include<EEPROM.h>
#include<math.h>
const int noofsamples =20;
const int nooffunction=1;   //number of ramping function
                            // 0 means no function
                            // change this every time u run
const int lowerbound =0;
const int upperbound =2*3.14159;
const int startPos=(nooffunction-1)*noofsamples;
void setup() {
  delay(1000);
  Serial.begin(9600);
 Serial.print("Burning Started at ");
  Serial.print(nooffunction);
  Serial.println(" position"); 
   Serial.print("Sample  rate ");
   Serial.println( noofsamples );
    
   
  
  EEPROM.update(0,nooffunction);
  double width=((upperbound)-(lowerbound))/(noofsamples-1);
  double temp,temp1,temp2;
 
  temp1=(fun(lowerbound));      //used for mapping
  
  
   temp2=(fun(upperbound));
   
  for (unsigned int i=0;i<noofsamples;i++)
  {//function finding value of nth sample
    temp=fun(lowerbound+i*width);
    byte value = map(temp,temp1,temp2,0,255);
    EEPROM.update(startPos+i+1,value);
  }
Serial.println("Burning Finished :D \n Reading values...");
for (unsigned int i=0;i<noofsamples;i++)
Serial.println((EEPROM[startPos+i+1]));
while(1);

}

void loop() {
}

  
 

