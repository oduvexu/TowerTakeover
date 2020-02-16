/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Jake                                             */
/*    Created:      Tue Feb 04 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----

// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

bool rvb=false;//true being red, false being blue
bool bumpRamp=false;

controller Controller = controller(primary);
motor DriveFR = motor(PORT2, ratio18_1, true);
motor DriveBR = motor(PORT10, ratio18_1, true);
motor DriveBL = motor(PORT3, ratio18_1, false);
motor DriveFL = motor(PORT4, ratio18_1, false);
motor ArmSpinnerR = motor(PORT7, ratio18_1, true);
motor ArmSpinnerL = motor(PORT6, ratio18_1, false);
motor RampRaiserR = motor(PORT1, ratio18_1, true);
motor RampRaiserL = motor(PORT5, ratio18_1, false);

void armcontrol();
void armcontrol(int a);
void rampcontrol();
void rampcontrol(int a);
void rampcontrol(double a);
void drivecontrol();
void drivecontrol(int f, bool t);
void autoSetUp();
void autoStack();
void bump(int a, int b);

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  RampRaiserL.setPosition(0,degrees);
  RampRaiserR.setPosition(0,degrees);
}

void autonomous(void)
{
  autoSetUp();
}

void usercontrol(void) {
  while(1)
  {
    armcontrol();
    rampcontrol();
    drivecontrol();
    wait(.01,seconds);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  
}

void drivecontrol()
{
  int f=-Controller.Axis3.value();
  if(f>80)
    f=80;
  int s=.5*Controller.Axis4.value();
  DriveFR.spin(fwd,f-s,pct);
  DriveBR.spin(fwd,-f-s,pct);
  DriveFL.spin(fwd,f+s,pct);
  DriveBL.spin(fwd,-f+s,pct);
}

void drivecontrol(int f, int s)
{
  DriveFR.spin(fwd,-f-s,pct);
  DriveBR.spin(fwd,f-s,pct);
  DriveFL.spin(fwd,-f+s,pct);
  DriveBL.spin(fwd,f+s,pct);
}

void armcontrol()
{
  if(Controller.ButtonR1.pressing()==true)
  {
    ArmSpinnerL.spin(fwd,100,pct);
    ArmSpinnerR.spin(fwd,100,pct);
   }else
    if(Controller.ButtonR2.pressing()==true)
    {
      ArmSpinnerL.spin(fwd,-100,pct);
      ArmSpinnerR.spin(fwd,-100,pct);
    }
    else
    {
    ArmSpinnerL.spin(fwd,-Controller.Axis2.value(),pct);
    ArmSpinnerR.spin(fwd,-Controller.Axis2.value(),pct);
    }
}

void armcontrol(int a)
{
  ArmSpinnerL.spin(fwd,-a,pct);
  ArmSpinnerR.spin(fwd,-a,pct);
}

void rampcontrol()
{
 double a = 50-RampRaiserL.position(deg)*40/1000;
 double b = 50-RampRaiserL.position(deg)*20/1000;
 //"a" represents the speed of the ramp. We found that the max value that position() outputs is around 1000.
  //The closer the ramp is to 1000, the slower it is meant to go. At 1000, a is about 10. At 0, a is about 50. 
 if(Controller.ButtonL1.pressing()==true){
    RampRaiserL.spin(fwd,a,pct);
    RampRaiserR.spin(fwd,a,pct);
  }else
  if(Controller.ButtonL2.pressing()==true){
      RampRaiserL.spin(fwd,-50,pct);
      RampRaiserR.spin(fwd,-50,pct);
  }else{
    RampRaiserL.stop(hold);
    RampRaiserR.stop(hold);
  }
}

void rampcontrol(int a)
{
 if(a!=0){
    RampRaiserL.spin(fwd,a,pct);
    RampRaiserR.spin(fwd,a,pct);
  }else{
    RampRaiserL.stop(hold);
    RampRaiserR.stop(hold);
  }
}

void rampcontrol(int a,bool t)
{
  double b = abs(a)+10-RampRaiserL.position(deg)*abs(a)/1000;
  if(t)
  {
    if(a>0)
    {
      RampRaiserL.spin(fwd,b,pct);
      RampRaiserR.spin(fwd,b,pct);
    }
    else
      if(a<0)
      {
        RampRaiserL.spin(fwd,-b,pct);
        RampRaiserR.spin(fwd,-b,pct);
      }
    else
    {
      RampRaiserL.stop(hold);
      RampRaiserR.stop(hold);
    }
  }
  else
  {
    if(a!=0)
    {
      RampRaiserL.spin(fwd,a,pct);
      RampRaiserR.spin(fwd,a,pct);
    }
    else
    {
      RampRaiserL.stop(hold);
      RampRaiserR.stop(hold);
    }
  }
}

void autoSetUp()
{
  int a;
  if (rvb)
    a =1;
  else
    a = -1;
//back up and release arms
  repeat(50)
  {
    drivecontrol(-90,0);
    rampcontrol(100,false);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }
  //pause to allow for any ramp shifting
  drivecontrol(0,0);
  wait(2,seconds);
  //reset ramp
  repeat(100)
  {
    wait(.01,seconds);
    RampRaiserL.spinToPosition(0,degrees,false);
    RampRaiserR.spinToPosition(0,degrees,false);
    if(Controller.ButtonA.pressing())
      break;
  }
  //drive forward to avoid rotating into cubes
  repeat(100)
  {
    drivecontrol(10,0);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }
  //rotate to cubes
  repeat(605)
  {
    drivecontrol(0,a*10);
    if(Controller.ButtonA.pressing())
      break;
    wait(.01,seconds);
  }
  // intake cubes
  repeat(450)
  {
    armcontrol(-100);
    drivecontrol(25,0);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }
  armcontrol(0);

  //rotate to corner
  repeat(103)
  {
    drivecontrol(0,a*50);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }
  //move to corner
  repeat(445)
  {
    drivecontrol(25,0);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }
  drivecontrol(0,0);
  //stacking process
  autoStack();
}

void autoStack(){
  //adjust cubes within the intake
  repeat(50)
  {
    armcontrol(-15);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }

  //raise the ramp safely. If the motor goes past 1000, it has issues going down
  //To avoid this, the program simply stops at 1000 degrees. 
  repeat(500)
  {
    if(RampRaiserR.position(degrees)>1000)
      rampcontrol(0);
    else
      rampcontrol(40,true);
    armcontrol(5);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }
  bump(10,30);

  //ramp returns to starting position
  repeat(200)
  {
    RampRaiserR.spinToPosition(0,degrees,false);
    RampRaiserL.spinToPosition(0,degrees,false);
    if(Controller.ButtonA.pressing())
      break;
    wait(.01,seconds);
  }
  //the robot slowly backs away and slowly spits out cubes to prevent taking
  //the whole stack with it. 
  repeat(250)
  {
    armcontrol(15);
    drivecontrol(-10,0);
    wait(.01,seconds);
    if(Controller.ButtonA.pressing())
      break;
  }
  //apply breaks to prevent excessive motor usage
  armcontrol(0);
  rampcontrol(0);
  drivecontrol(0,0);
}

void bump(int a, int b){
  repeat(a)
  {
    rampcontrol(-b);
    wait(.01,seconds);
  }
  repeat(a)
  {
    rampcontrol(b);
    wait(.01,seconds);
  }
  rampcontrol(0);
}