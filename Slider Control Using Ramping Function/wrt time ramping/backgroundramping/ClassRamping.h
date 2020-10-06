
#define fun1(x) ((x)-sin(x))
#define fun2(x) (x)
#define calibrate 5
#include<math.h>
#include <USBSabertooth.h>
const int MaxTime=5000;                     //MAX TIME (time taken for motor to change from -2047 to 2047 at instant pulse)
 
/*Eg: Ramp Saber(2,0,5); or Ramp Saber;//for linear ramp
      Saber.initialize(ST);//insideloop implements ramp need sabertooth variable
      Saber.SetSpeed(pwm,motorno); 
*/   


class Ramp 
{ int funno;
int pwm1,pwm;unsigned long time1;
int pwm2;unsigned long time2;
  float temp1,temp2;
 boolean flag;
 float lowerbound ;
  float upperbound ;
  int motorno;
 public:
 Ramp(int no=1,double lb=0,double ub=2*3.14159)                       //Arguments at initilisation:(function number,lower bound,upperbound)
 { pwm1=pwm2=pwm=0;
   funno=no;
   lowerbound=lb ;
   upperbound=ub ;
   flag=false;
   
  if(no==1){
             temp1=(fun1(lowerbound)); 
             temp2=(fun1(upperbound));  //used for mapping 
           
          }
  if(no==2){
             temp1=(fun2(lowerbound)); 
             temp2=(fun2(upperbound));  //used for mapping 
            
          }
          
 }
 void rmp(USBSabertooth ST)
 {
   if(funno==1){
  if(flag) 
  { if ((millis()-time1)<(time2-time1))
   {
    float temp= fun1((upperbound-lowerbound)*(millis()-time1)/(time2-time1));
  float c=(pwm2-pwm1)/(temp2-temp1);
      pwm = ((c*(temp-temp1))+pwm1); 
    
 ST.motor(motorno,pwm);}}
 else {ST.motor(motorno,pwm2);
      flag=false;
     pwm=pwm1=pwm2; 
     }
 }
 else if (funno==2){
   if(flag) 
  { if ((millis()-time1)<(time2-time1))
   {
    float temp= fun2((upperbound-lowerbound)*(millis()-time1)/(time2-time1));
  float c=(pwm2-pwm1)/(temp2-temp1);
      pwm = ((c*(temp-temp1))+pwm1); 

 ST.motor(motorno,pwm);}}
 else {ST.motor(motorno,pwm2);
      flag=false;
     pwm=pwm1=pwm2; 
     }
 
 }
 }
 void SetSpeed(int k,int motorN)    //Arguments(ST variable,motorno:,pwm1,pwm2)
 {if(k<pwm2-calibrate||k>pwm2+calibrate)
 {flag=true;

 motorno =motorN;
 pwm1=pwm;time1=millis();
 pwm2=k;
 time2=int((abs(pwm2-pwm1))*(MaxTime/(2047*2)))+time1;
 }
  
  
 }
  
};

