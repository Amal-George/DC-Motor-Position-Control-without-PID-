
#define fun1(x) ((x)-sin(x))
#define fun2(x) (x)
#include<math.h>
#include <USBSabertooth.h>

/*Eg: 
*/


class Ramp
{ public:
  int funno;
int pwm1,* pwm; double X1;
int pwm2; double X2;
double * pos;
  float temp1,temp2;
 boolean flag;
 float lowerbound ;
  float upperbound ;
  int motorno;
 int sign=1;//direction
 Ramp(double & input,int & speedvariable,int no=1,double lb=0,double ub=2*3.14159 )                       //Arguments at initilisation:(function number,lower bound,upperbound)
 { pwm1=pwm2=0;
   *pos=input;
   *pwm=speedvariable;
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


 void rmp(boolean jk)
 {if (jk){
   
   if(funno==1){
  if(flag)
  { if (((*pos)*sign)<(X2*sign))
   {
    float temp= fun1((upperbound-lowerbound)*abs((*pos)-X1)/abs(X2-X1));
  float c=(pwm2-pwm1)/(temp2-temp1);
      *pwm = ((c*(temp-temp1))+pwm1);

 }
 else {
      flag=false;
     *pwm=pwm1=pwm2;
     }
 }}
 
 
 
 else if (funno==2){
   if(flag)
  { if (((*pos)*sign)<(X2*sign))
   {
    float temp= fun2((upperbound-lowerbound)*abs((*pos)-X1)/abs(X2-X1));
  float c=(pwm2-pwm1)/(temp2-temp1);
      *pwm = ((c*(temp-temp1))+pwm1);

 }
 else {
      flag=false;
     *pwm=pwm1=pwm2;
     }
 }}
 
 }}
 
 void Rampdown(double pos2,int pwmfnl,int motorN)    //Arguments(ST variable,motorno:,pwm1,pwm2)
 {if(X2!=pos2||pwmfnl!=pwm2)
 {flag=true;
  motorno =motorN;
 pwm1=*pwm;X1=*pos;
 pwm2=pwmfnl;
 X2=pos2;
 if(X2>X1)sign=1;
 else sign=-1;
 }


 }

};

