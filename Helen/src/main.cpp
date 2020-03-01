/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    1, 2, 3, 4, F   
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "VisionSensor1.h"
#include "VisionSensor2.h"
#include "VisionSensor3.h"
using namespace vex;

// A global instance of competition
competition Competition;


// define your global instances of motors and other devices here
  void tempurpedicControl(); //this controls the back wheels. Feel free to rename this Andrew, -Jake
  void drivecontrol();
  void armcontrol();
  void rampcontrol();
  void findTarget();
  void findTargetsimple();
  void countobjects();      // function counting the number of cubes within the frame
  void measureobjects();    //function measuring the position of on cube of each color and printing it to the screen
  void printMultiple();     //function reading several different cubes of every color and printing it the the screen
  void approachcube();        //turns the robot to the closest green cube when the 'X' button is pushed
  void reset();              //resets all functions of robot
  void drive();             //drives the robot, switches modes using the 'A' button 
  void smartAutonomous();  
  void dumbAutonomous();
  void pickup();    
  void stack(); 
  int getBaseLocation();   
  int G_num;                //the number of green cubes in the countobjects function
  int O_num;                //the number of orange cubes in the countobjects function
  int P_num;                //the number of purple cubes in the countobjects function
  int cube_num;             //number of cubes in the countobjects function
  int j, i, d;                    //used in for loops
  int k;           
  float s;
  int lastpressed;          //used in changing driving modes
  bool cubeFound = false;   //says whether the cube is within the desired frame in the turntocube function
  int targetColor = 0;
  int targetid = 0;
  int targetDist;
  bool targetAcquired = false;
  bool turnDone = false;
  int width;
  int t = 0;
  int gPriority = 0; 
  int oPriority = 0;
  int pPriority = 0;
  int currentPriority = 0;
  bool Home = true;
  int BL = 0;
  int BR = 0;
  int FL = 0;
  int FR = 0;
  float rad = 0.0174532925;
  bool targetFound = false;
  bool tiltDone = false;
  bool bumpDone = false;
  bool grabDone = false;
  bool Reset = false;
  int count = 0;
  bool intakeFull = false;
  float Angle[5];
  bool baseFound = false;
  bool approachBase = true;
  bool stacktower = false;
  bool stackDone = false;
  int x;
  int y;


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
 
    RampRaiserL.setRotation(0, vex::rotationUnits::deg);
    RampRaiserR.setRotation(0, vex::rotationUnits::deg);
    ArmTilterR.setRotation(0, deg);
    ArmTilterL.setRotation(0, deg);
    DriveBL.setStopping(coast);
    DriveFL.setStopping(coast);
    DriveFR.setStopping(coast);
    DriveBR.setStopping(coast);
    TempurpedicBackSupportL.setStopping(hold);
    TempurpedicBackSupportR.setStopping(hold);
    
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
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

void autonomous(void) {
  
  
  dumbAutonomous();


}
void usercontrol(void) {
    while(1){
   
        Controller.Screen.setCursor(10, 10);
        Controller.Screen.print("%d        ", int(Drivetrain.heading()));
        
      
      Drivetrain.arcade(Controller.Axis3.value(), Controller.Axis4.value());
      //drivecontrol();
      rampcontrol();
      armcontrol();
      if(Controller.ButtonA.pressing()){
        smartAutonomous();
      }
      tempurpedicControl();
    }
}

//
// Main will set up the competition functions and callbacks.
//

int main() {

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  Controller.Screen.clearScreen();
  pre_auton();
  

 

 
  } 
void smartAutonomous(){
  Drivetrain.setTurnVelocity(10, percent);
  Drivetrain.setDriveVelocity(10, percent);
  Drivetrain.driveFor(fwd, 20, inches);
  Drivetrain.turnFor(right, 90, degrees);
  Drivetrain.driveFor(fwd, 60, inches);
  Drivetrain.turnFor(right, 180, degrees);
  Drivetrain.driveFor(fwd, 60, inches);
  Drivetrain.turnFor(right, 90, degrees);
  Drivetrain.driveFor(reverse, 20, inches);
}
 void dumbAutonomous(){
 
  
  
  
   ArmSpinnerL.spin(fwd, 100, pct);
   ArmSpinnerR.spin(fwd, 100,pct);
   task::sleep(300);
   ArmSpinnerL.spin(fwd, 0, pct);
   ArmSpinnerR.spin(fwd,0,pct);
   while(ArmTilterR.rotation(deg) < 330 && ArmTilterR.rotation(deg) < 330 ){
    ArmTilterR.spin(fwd, 50, pct);
    ArmTilterL.spin(fwd, 50, pct);
   }
    ArmSpinnerL.spin(fwd, 100, pct);
    ArmSpinnerR.spin(fwd, 100,pct);
    task::sleep(300);
    ArmSpinnerL.spin(fwd,0, pct);
    ArmSpinnerR.spin(fwd,0,pct);


   while(ArmTilterR.rotation(deg) > 100 && ArmTilterR.rotation(deg) > 100){
    ArmTilterR.spin(fwd, -50, pct);
    ArmTilterL.spin(fwd, -50, pct);
   }
    RampRaiserL.spin(fwd, -20, pct);
    RampRaiserR.spin(fwd, -20, pct);
    wait(3, seconds);
    RampRaiserL.spin(fwd, 0, pct);
    RampRaiserR.spin(fwd, 0, pct);

   Drivetrain.drive(forward);
   wait(3, seconds);
   Drivetrain.drive(reverse);
   wait(3, seconds);
   Drivetrain.stop(coast);


   }

void drivecontrol(){
 
  if(Controller.Axis3.value() != 0 || Controller.Axis4.value() != 0){
    FL = Controller.Axis3.value()+Controller.Axis4.value();
    FR = Controller.Axis3.value()-Controller.Axis4.value();
    BR = Controller.Axis3.value()-Controller.Axis4.value();
    BL = Controller.Axis3.value()+Controller.Axis4.value();
    DriveFL.spin(fwd, FL, pct);
    DriveFR.spin(fwd, FR, pct);
    DriveBL.spin(fwd, BL, pct);
    DriveBR.spin(fwd, BR, pct);
  }else{
    DriveFL.spin(fwd, 0, pct);
    DriveFR.spin(fwd, 0, pct);
    DriveBL.spin(fwd, 0, pct);
    DriveBR.spin(fwd, 0, pct);
  }
  
}
void armcontrol(){
  //arm tilt controls

  if(Controller.ButtonLeft.pressing()){
    ArmCompress.spin(fwd, 60, pct);
  }else if (Controller.ButtonRight.pressing()){
    ArmCompress.spin(fwd, -60, pct);
  }else{
    ArmCompress.stop(hold);
  }
  if(Controller.ButtonX.pressing()==true)
  {
    ArmTilterL.spin(fwd,30,pct);
    ArmTilterR.spin(fwd,30,pct);
  }else if (Controller.ButtonB.pressing()==true){
    ArmTilterL.spin(fwd,-30,pct);
    ArmTilterR.spin(fwd,-30,pct);
  }else{
    ArmTilterL.stop(hold);
    ArmTilterR.stop(hold);
  }
 

  if(Controller.ButtonR1.pressing()==true){
    ArmSpinnerL.spin(fwd,100,pct);
    ArmSpinnerR.spin(fwd,100,pct);
    RampConveyer.spin(fwd,100,pct);
   }else
    if(Controller.ButtonR2.pressing()==true)
    {
      ArmSpinnerL.spin(fwd,-100,pct);
      ArmSpinnerR.spin(fwd,-100,pct);
      RampConveyer.spin(fwd,-100,pct);
    }
    else if (Controller.ButtonLeft.pressing()){
      ArmSpinnerL.spin(fwd,100,pct);
      ArmSpinnerR.spin(fwd,-100,pct);
    }else{
    ArmSpinnerL.spin(fwd,-Controller.Axis2.value(),pct);
    ArmSpinnerR.spin(fwd,-Controller.Axis2.value(),pct);
    
  }
}
void rampcontrol(){

 if(Controller.ButtonL1.pressing()==true){
    RampRaiserL.spin(fwd,30,pct);
    RampRaiserR.spin(fwd,30,pct);
  }else
  if(Controller.ButtonL2.pressing()==true){
      RampRaiserL.spin(fwd,-30,pct);
      RampRaiserR.spin(fwd,-30,pct);
  }else{
    RampRaiserL.stop(hold);
    RampRaiserR.stop(hold);
  }

 

  
}

void reset(){
  ArmTilterL.stop(coast);
  ArmTilterR.stop(coast);
  RampRaiserL.stop(coast);
  RampRaiserR.stop(coast);
  vex::task::sleep(100);
  pre_auton();
  turnDone = false;
  grabDone = false;
  targetFound = false;
  targetAcquired = false;
  Reset = false;
  ArmTilterL.setRotation(0, deg);
  ArmTilterR.setRotation(0, deg);

}
void tempurpedicControl()
{
  if(Controller.ButtonDown.pressing())
    {
      TempurpedicBackSupportR.spin(fwd,-50,pct);
      TempurpedicBackSupportL.spin(fwd,-50,pct);
    }
  else if(Controller.ButtonUp.pressing())
  {
      TempurpedicBackSupportR.rotateTo(0, degrees,false);
      TempurpedicBackSupportL.rotateTo(0, degrees,false);
  }
  else
  {
    TempurpedicBackSupportL.stop();
    TempurpedicBackSupportR.stop();
  }

  Brain.Screen.clearScreen();
  Brain.Screen.print(TempurpedicBackSupportL.position(degrees));
  Brain.Screen.newLine();
  Brain.Screen.print(TempurpedicBackSupportR.position(degrees));
  Brain.Screen.setCursor(1,1);
}