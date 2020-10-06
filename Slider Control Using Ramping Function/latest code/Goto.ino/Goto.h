#include "ClassRamping.h"


float AA,BB;

int signof(double x)
 {if (x>0)return (1);
  else if (x==0)return (0);
       else return (-1);
}
double X0up(int x) 

{AA=        0.0103226*1;       // xtra distance for ramping
  
  return(AA*x);
  
}
  
double X0dwn(int x) 

{BB=     0.0103226*1;          // xtra distance for ramping
  
  return(BB*x);
  
}
 
class RampPositioning
{
    //double * Setpoint;  //variables that are linked with
    double * Input;        //other functions in loop
    int * pwm;
    int pwmfinal;
    double S, gap, initialgap;
    Ramp Name;
    int minspeed, pwmlowerlimit, pwmupperlimit;
    bool over, initilize;
    int path;
    bool k; //controls ramping true/false
    int signpath3;
  public:
    RampPositioning(double*currentpos, int*speedatinstant, int pup = 2047, int plwr = -2047, int minimumspeed = 200)
    { Name.pass(currentpos, speedatinstant, minimumspeed);
      Input = currentpos;
      pwm = speedatinstant;
      pwmlowerlimit = plwr;
      pwmupperlimit = pup;
      minspeed = minimumspeed;
      over = false; initilize = false;
      path = 0;
      signpath3 = 0;
      k = true;
    }
    bool Startpositioning(bool flag, double final, int pwm3 = 0)                     ///implemented in loop
    { if (S != final) initilize = true;
      S = final;
      Name.rmp(k);
      pwmfinal = pwm3;
      gap = (S - *Input);
      if (flag) {
        if (initilize) { // find path
          //Serial.println("initilizing");
          over = false;
          initilize = false;
          if (signof(*pwm) == signof(S - *Input) || *pwm == 0)       // destination ahead in direction of motion
          {
            int sign = signof(*pwm);
            if (sign * (*Input + X0dwn(*pwm)) > (S)*sign) {
              path = 1;                //Overshoot and trace back path
              Name.SetSpeed(*Input + X0dwn(*pwm), 0); //Serial.print("*Input+X0dwn(*pwm)"); Serial.println(*Input + X0dwn(*pwm));
            }
            else {
              int pwm2 = (gap + X0up(*pwm) + X0dwn(pwmfinal)) / (AA + BB);
              if (pwm2 > pwmupperlimit) {
                pwm2 = pwmupperlimit;
              }
              else if (pwm2 < pwmlowerlimit) pwm2 = pwmlowerlimit;
              path = 2;                                // rampup - run -rampdwn path
              if(abs(*pwm)<minspeed) *pwm=minspeed*signof(S-*Input);                     
              Name.SetSpeed(*Input + X0up(pwm2)-X0up(*pwm), pwm2);
              
              if (abs(pwm2) < minspeed) {
                pwm2 = minspeed * signof(gap);      //min speed motion path
                path = 3;
                signpath3 = signof(gap);
                Name.SetSpeed(*Input, pwm2);
              }
             // Serial.print("dist"); Serial.println(*Input + X0up(pwm2)); Serial.print("pwm2"); Serial.println(pwm2);
            }

          }
          else                                                             //destination behind in opposite direction of motion
          {
            Name.SetSpeed(*Input + X0dwn(*pwm), 0); path = 1;
          }
         // Serial.print("path: "); Serial.println(path);
          //while(1);
        }
        else // if not initilize
        { 
          switch (path)
          { case 3: if (*Input * signpath3 > S) {
                *pwm = pwmfinal ;
                over = true;// Serial.println("path3");
                
              }
              break;
            case 1: if (*pwm == 0) {
                initilize = true;// Serial.println("path1");
              }
              break;
            case 2: if (signof(*pwm) * (*Input) >= (S - X0dwn(*pwm))*signof(*pwm)) { //Serial.println("path2");
                                                                                      Name.SetSpeed(S, pwmfinal);
                                                                                      if (signof(*pwm) * (*Input) >= (S)*signof(*pwm)){
                                                                                      *pwm = pwmfinal;
                                                                                       over = true;
                                                                                       }
                                                                                    }
                      break;
          }
        }

      }                                     //flag close
      return (over);
    }                                  //end of Startpositioning()
};

