
#define fun1(x) (0.2312+1.1706*x-0.080185*x*x+0.0018727*x*x*x)
#define fun2(x) (x-sin(x))
#include <math.h>

#include <digitalWriteFast.h>
#define Apin 2   
#define Bpin 3// interrupt pin
volatile long counter =0;
void Arise()
{
  if (  digitalReadFast(Bpin)==digitalReadFast(Apin) )
    counter ++;
  else
    counter --;
    
}

#include <USBSabertooth.h>
USBSabertoothSerial C(Serial1); 
USBSabertooth ST(C, 128); 

////Define Shared Variables we'll be connecting to
double Setpoint=0;
double Input=0;
int pwm=0;


/*Eg: associate the variables that has current position and pwm
Ramp Name(current position,current pwm); // inside setup

rmp(true);             //implements ramp .. should be inside loop
SetSpeed(final poition,pwm2); //if direction is to be reversed 
                            //first stop before changing direction
*/


class Ramp
{ public:
  int funno;
int pwm1,* pwm; double X1;
int pwm2,minspeed; double X2;
double * pos;
  float temp1,temp2;
 bool flag;
 float lowerbound ;
  float upperbound ;
  int sign=1,pwmsign=1;//direction
 Ramp(int no=1,double lb=0,double ub=19.84 )                       //Arguments at initilisation:(function number,lower bound,upperbound)
 { pwm1=pwm2=0;
   
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
 void pass (double * input,int*speedvariable,int minimumspeed=150)
{pos=input;minspeed=minimumspeed;
 pwm=speedvariable;}

 void rmp(bool jk)
 {if (jk&&flag){
   
   if(funno==1){
  Serial.print("pwm");Serial.println(*pwm);delay(2000);
  { if (((*pos)*sign)<(X2*sign))
   {int tmp=pwm2;
     if(abs(pwm2)<minspeed) tmp=((pwm1<0)?(-1*minspeed):minspeed);
    float temp= fun1((upperbound-lowerbound)*((*pos)-X1)/(X2-X1));
  float c=(tmp-pwm1)/(temp2-temp1);
      *pwm = ((c*(temp-temp1))+pwm1);

 }
 else {
      flag=false;
     *pwm=pwm1=pwm2;
     }
 }}
 
 
 
 else if (funno==2){
   
  { if (((*pos)*sign)<(X2*sign))
   {int tmp=pwm2;
     if(abs(pwm2)<minspeed) tmp=((pwm1<0)?(-1*minspeed):minspeed);
    
    float temp= fun2((upperbound-lowerbound)*((*pos)-X1)/(X2-X1));
  float c=(tmp-pwm1)/(temp2-temp1);
      *pwm = ((c*(temp-temp1))+pwm1);

 }
 else {
      flag=false;
     *pwm=pwm1=pwm2;
     }
 }}
 
 }}
 
 void SetSpeed(double pos2,int pwmfnl)    //Arguments(ST variable,motorno:,pwm1,pwm2)
 {if(X2!=pos2||pwmfnl!=pwm2)
 {flag=true;
  pwm1=*pwm;X1=*pos;
 pwm2=pwmfnl;
 X2=pos2;
 if(X2>X1)sign=1;
 else sign=-1;
 Serial.print("pwmfnl"); Serial.println(pwmfnl);
 }


 }

};
/*****************************************************************/

float AA,BB;

int signof(double x)
 {if (x>0)return (1);
  else if (x==0)return (0);
       else return (-1);
}
double X0up(int x) 

{AA=        0.0103226*2;       // xtra distance for ramping
  
  return(AA*x);
  
}
  
double X0dwn(int x) 

{BB=     0.0103226*4;          // xtra distance for ramping
  
  return(BB*x);
  
}
 
/*******************************************************************/
class RampPositioning
{
double * Setpoint;  //variables that are linked with 
double * Input;        //other functions in loop   
int * pwm;
int pwmfinal;
double S,gap,initialgap;
Ramp Name;
int minspeed,pwmlowerlimit,pwmupperlimit;
bool over,initilize;
int path;
bool k; //controls ramping true/false
int signpath3;
public:
RampPositioning(double*currentpos,int*speedatinstant,double*final,int pup=2047,int plwr=-2047,int minimumspeed=200)
{ Name.pass(currentpos,speedatinstant,minimumspeed);
  Input=currentpos;
  pwm=speedatinstant;
  Setpoint=final;
  pwmlowerlimit=plwr;
  pwmupperlimit=pup;
  minspeed=minimumspeed;
  over=false;initilize=true;
  path=0;
  signpath3=0;
  k=true; 
}
bool Startpositioning(bool flag,double final,int pwm3=0)                         ///implemented in loop
{ if (S!=final) initilize=true;
  S=final;
  Name.rmp(k);
  pwmfinal=pwm3;
  gap=(S-*Input);
  if(flag){Serial.print("path");Serial.println(path);
  if(initilize){// find path
  
                    over=false;
                    initilize=false;  
                 if(signof(*pwm)==signof(S-*Input)||*pwm==0)                // destination ahead in direction of motion
                  {double x0pwm1=X0dwn(*pwm);
                  int sign=signof(*pwm);
                  if(sign*(*Input+x0pwm1)>(S)*sign){ path=1;                  //Overshoot and trace back path
                                                              Name.SetSpeed(*Input+x0pwm1,0);
                                                            }
                  else {int pwm2=(gap+X0up(*pwm)+X0dwn(pwmfinal))/(AA+BB);
                              if(pwm2>pwmupperlimit){pwm2=pwmupperlimit;}
                              else if (pwm2<pwmlowerlimit) pwm2=pwmlowerlimit;
                              path=2;                                  // rampup - run -rampdwn path
                              Name.SetSpeed(*Input+X0up(pwm2),pwm2);
                              if(abs(pwm2)<minspeed){pwm2=minspeed*signof(gap);          //min speed motion path
                                                    path=3;
                                                    signpath3=signof(gap);
                                                    Name.SetSpeed(*Input,pwm2);}
                                                
                         }
                    
                  }
                 else                                                             //destination behind in opposite direction of motion
                   {
                     Name.SetSpeed(*Input+X0dwn(*pwm),0);path=1;
                   } 
                Serial.println("initilizing");
                delay(2000);  
                }
 else // if not initilize
     {
           switch(path)
          { case 3:if(*Input*signpath3>S){*pwm= pwmfinal ;over=true;}
                   break;
            case 1:if(*pwm==0){initilize=true;}
                    break;
            case 2:if(signof(*pwm)*(*Input)>=(S-X0dwn(*pwm))*signof(*pwm)) { Name.SetSpeed(S,pwmfinal);
                                                                                  if(signof(*pwm)*(*Input)>=(S)*signof(*pwm))
                                                                                  {*pwm=pwmfinal;over=true;}
                                                                                 }
                   break;
            
          }
     }
 
}                                     //flag close
return (over);
}                                  //end of Startpositioning()
};

/*****************************************************/



RampPositioning XX(&Input,&pwm,&Setpoint,2047,2047,200);
void setup()
{  Serial.begin(9600);
   Serial1.begin(9600);
   pinModeFast(Apin, INPUT);
    pinModeFast(Bpin, INPUT);
   attachInterrupt(0, Arise, RISING);
 
}

bool flag=true,s;
int milli;       // for data intake millimeter
int sign=1;       

void loop()
{ Input = counter/252.84158;
   s=XX.Startpositioning(flag,Setpoint,0);
   
   if(Serial.available())
 {milli=Serial.parseInt();
   if (Serial.read()=='-') sign*=-1;
   Setpoint=milli*sign;}
   
  Serial.print(Input);Serial.print(',');Serial.print(s);Serial.print(',');
Serial.println(Setpoint);
ST.motor(1,pwm);
}
