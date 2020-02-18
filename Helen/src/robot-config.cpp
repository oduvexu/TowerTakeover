#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller = controller(primary);

sonar Intake = sonar(Brain.ThreeWirePort.G);

limit ArmTilt = limit(Brain.ThreeWirePort.A);
line Feed = line(Brain.ThreeWirePort.C);
line FeedFull = line(Brain.ThreeWirePort.B);
line LeftLine = line(Brain.ThreeWirePort.E);
line RightLine = line(Brain.ThreeWirePort.F);
motor DriveFR = motor(PORT1, ratio18_1, true);
motor DriveBR = motor(PORT2, ratio18_1, true);
motor DriveBL = motor(PORT3, ratio18_1, false);
motor DriveFL = motor(PORT4, ratio18_1, false);
motor RampRaiserR = motor(PORT5, ratio18_1, true);
motor RampRaiserL = motor(PORT6, ratio18_1, false);
motor ArmTilterL = motor(PORT7, ratio18_1, true);
motor ArmTilterR = motor(PORT8, ratio18_1, false);
motor ArmSpinnerR = motor(PORT9, ratio18_1, true);
motor ArmSpinnerL = motor(PORT10, ratio18_1, false);
motor RampConveyer = motor(PORT11, ratio18_1, true);
motor ArmCompress = motor(PORT20, ratio18_1, true);

motor leftMotorA = motor(PORT3, ratio18_1, false);
motor leftMotorB = motor(PORT4, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);

motor rightMotorA = motor(PORT1, ratio18_1, true); 
motor rightMotorB = motor(PORT2, ratio18_1, true); 
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);

inertial TurnGyroSmart = inertial(PORT13);
smartdrive Drivetrain= smartdrive(LeftDriveSmart, RightDriveSmart, TurnGyroSmart,319.19, 320, 165, mm, 1);






/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain gyro
  wait(200, msec);
  TurnGyroSmart.calibrate();
  Brain.Screen.print("Calibrating Gyro for Drivetrain");
  // wait for the gyro calibration process to finish
  while (TurnGyroSmart.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  wait(50, msec);
  Brain.Screen.clearScreen();
}