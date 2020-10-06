#include "Arduino.h"
#define fun1(x) (0.2312+1.1706*x-0.080185*x*x+0.0018727*x*x*x)
#define fun2(x) (x-sin(x))
#include <math.h>


/*Eg: associate the variables that has current position and pwm
Ramp Name(current position,current pwm); // inside setup

rmp(true);             //implements ramp .. should be inside loop
SetSpeed(final poition,pwm2); //if direction is to be reversed
                            //first stop before changing direction
*/


class Ramp {
  public:
    int funno;
    int pwm1, * pwm; double X1;
    int pwm2, minspeed; double X2;
    double * pos;
    float temp1, temp2;
    bool flag;
    float lowerbound ;
    float upperbound ;
    int sign = 1, pwmsign = 1; //direction
    Ramp(int no = 1, double lb = 0, double ub = 19.84 ) {              //Arguments at initilisation:(function number,lower bound,upperbound)
      pwm1 = pwm2 = 0;
      funno = no;
      lowerbound = lb ;
      upperbound = ub ;
      flag = false;
      if (no == 1) {
        temp1 = (fun1(lowerbound));
        temp2 = (fun1(upperbound)); //used for mapping
      }
      else if (no == 2) {
        temp1 = (fun2(lowerbound));
        temp2 = (fun2(upperbound)); //used for mapping
      }
    }
    void pass (double * input, int*speedvariable, int minimumspeed = 150)
    { pos = input; minspeed = minimumspeed;
      pwm = speedvariable;
    }

    void rmp(bool jk) {
      if (jk && flag) {
        if (funno == 1) {
          //Serial.println(X2);
          if (((*pos)*sign) < (X2 * sign)) {
            int tmp = pwm2;
            if (abs(pwm2) < minspeed) tmp = ((pwm1 < 0) ? (-1 * minspeed) : minspeed);
            float temp = fun1((upperbound - lowerbound) * ((*pos) - X1) / (X2 - X1));
            float c = (tmp - pwm1) / (temp2 - temp1);
            *pwm = ((c * (temp - temp1)) + pwm1);
          }
          else {
            flag = false;
            *pwm = pwm1 = pwm2;
          }
        }
        else if (funno == 2) {
          if (((*pos)*sign) < (X2 * sign)) {
            int tmp = pwm2;
            if (abs(pwm2) < minspeed) tmp = ((pwm1 < 0) ? (-1 * minspeed) : minspeed);
            float temp = fun2((upperbound - lowerbound) * ((*pos) - X1) / (X2 - X1));
            float c = (tmp - pwm1) / (temp2 - temp1);
            *pwm = ((c * (temp - temp1)) + pwm1);
          }
          else {
            flag = false;
            *pwm = pwm1 = pwm2;
          }
        }
      }
    }

    void SetSpeed(double pos2, int pwmfnl)   //Arguments(ST variable,motorno:,pwm1,pwm2)
    { if (X2 != pos2 || pwmfnl != pwm2)
      { flag = true;
        pwm1 = *pwm; X1 = *pos;
        pwm2 = pwmfnl;
        X2 = pos2;
        if (X2 > X1)sign = 1;
        else sign = -1;
      }
    }
};

