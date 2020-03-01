#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// VEXcode device constructors

motor DriveFR = motor(PORT2, ratio18_1, true);
motor DriveBR = motor(PORT10, ratio18_1, true);
motor DriveBL = motor(PORT3, ratio18_1, false);
motor DriveFL = motor(PORT4, ratio18_1, false);

inertial AccnGyro = inertial(PORT8);
motor RampRaiserR = motor(PORT5, ratio18_1, false);
motor RampRaiserL = motor(PORT1, ratio18_1, true);
motor ArmSpinnerL = motor(PORT6, ratio18_1, true);
motor ArmSpinnerR = motor(PORT7, ratio18_1, false);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain gyro
  wait(200, msec);
  AccnGyro.calibrate();
  Brain.Screen.print("Calibrating Gyro for Drivetrain");
  // wait for the gyro calibration process to finish
  while (AccnGyro.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  wait(50, msec);
  Brain.Screen.clearScreen();
}
