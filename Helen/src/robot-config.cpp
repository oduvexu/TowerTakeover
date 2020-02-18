#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller = controller(primary);
sonar Right = sonar(Brain.ThreeWirePort.E);
sonar Rear = sonar(Brain.ThreeWirePort.G);

limit ArmTilt = limit(Brain.ThreeWirePort.A);
line Feed = line(Brain.ThreeWirePort.B);
line Feed2 = line(Brain.ThreeWirePort.C);
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






/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}