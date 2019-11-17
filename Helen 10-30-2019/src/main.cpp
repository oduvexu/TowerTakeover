/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Jake                                             */
/*    Created:      Wed Oct 02 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
// A global instance of vex::competition
vex::competition Competition;

// define your global instances of motors and other devices here
vex::motor DriveFR(PORT1,true);
vex::motor DriveBR(PORT2,true);
vex::motor DriveBL(PORT3,false);
vex::motor DriveFL(PORT4,false);
vex::motor RampRaiserR(PORT5,true);
vex::motor RampRaiserL(PORT6,false);
vex::motor ArmSpinnerR(PORT9,true);
vex::motor ArmSpinnerL(PORT10,false);
vex::motor ArmTilterR(PORT8,false);
vex::motor ArmTilterL(PORT7,true);
vex::motor RampConveyer(PORT11,true);
vex::controller Controller;

void drivecontrol();
void armcontrol();
void rampcontrol();

int intakeMode;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */ 
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton( void ) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  intakeMode=3;
  
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous( void ) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol( void ) 
{
  intakeMode=3;
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo 
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to 
    // update your motors, etc.
    // ........................................................................

    drivecontrol();
    armcontrol();
    rampcontrol();
 
    vex::task::sleep(20); //Sleep the task for a short amount of time to prevent wasted resources. 
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
    //Set up callbacks for autonomous and driver control periods.
    //Competition.autonomous( autonomous );
    //Competition.drivercontrol( usercontrol );
    
    //Run the pre-autonomous function. 
    pre_auton();
       
    //Prevent main from exiting with an infinite loop.                        
    while(1) {
      drivecontrol();
      armcontrol();
      rampcontrol();
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }    
       
}

void drivecontrol()
{
  DriveFL.spin(fwd, Controller.Axis3.value()+Controller.Axis4.value(), pct);
  DriveFR.spin(fwd, Controller.Axis3.value()-Controller.Axis4.value(), pct);
  DriveBL.spin(fwd, Controller.Axis3.value()+Controller.Axis4.value(), pct);
  DriveBR.spin(fwd, Controller.Axis3.value()-Controller.Axis4.value(), pct);
}

void armcontrol()
{
  //arm tilt controls
  if(Controller.ButtonUp.pressing()==true)
  {
    ArmTilterL.spin(fwd,10,pct);
    ArmTilterR.spin(fwd,10,pct);
  }
  else if (Controller.ButtonDown.pressing()==true)
  {
    ArmTilterL.spin(fwd,-10,pct);
    ArmTilterR.spin(fwd,-10,pct);
  }
  else
  {
    ArmTilterL.stop(hold);
    ArmTilterR.stop(hold);
  }

  //intakeMode controls
  //the belt on ends of the arms and up and down the ramp will act as the intakeMode
  switch(intakeMode)
  {
  case 1:
    ArmSpinnerL.spin(fwd,70,pct);
    ArmSpinnerR.spin(fwd,70,pct);
    RampConveyer.spin(fwd,70,pct);
    if(Controller.ButtonB.pressing()==true)
      intakeMode=3;
    break;
  case 2:
    ArmSpinnerL.spin(fwd,-70,pct);
    ArmSpinnerR.spin(fwd,-70,pct);
    RampConveyer.spin(fwd,-70,pct);
    if(Controller.ButtonB.pressing()==true)
      intakeMode=3;
    break;
  case 3:
    if(Controller.ButtonR1.pressing()==true)
  {
    ArmSpinnerL.spin(fwd,70,pct);
    ArmSpinnerR.spin(fwd,70,pct);
    RampConveyer.spin(fwd,70,pct);
  }
  else
    if(Controller.ButtonR2.pressing()==true)
    {
      ArmSpinnerL.spin(fwd,-70,pct);
      ArmSpinnerR.spin(fwd,-70,pct);
      RampConveyer.spin(fwd,-70,pct);
    }
    else
    {
      ArmSpinnerL.stop(hold);
      ArmSpinnerR.stop(hold);
      RampConveyer.stop(hold);
    }
    break;
  }
  if(Controller.ButtonA.pressing()==true)
    intakeMode=2;
  if(Controller.ButtonY.pressing()==true)
    intakeMode=1;
  if(Controller.ButtonR1.pressing()==true||Controller.ButtonR2.pressing()==true)
    intakeMode=3;
}

void rampcontrol()
{
  if(Controller.ButtonL1.pressing()==true)
  {
    RampRaiserL.spin(fwd,20,pct);
    RampRaiserR.spin(fwd,20,pct);
  }
  else
    if(Controller.ButtonL2.pressing()==true)
    {
      RampRaiserL.spin(fwd,-20,pct);
      RampRaiserR.spin(fwd,-20,pct);
    }
    else
    {
      RampRaiserL.stop(hold);
      RampRaiserR.stop(hold);
    }
}

