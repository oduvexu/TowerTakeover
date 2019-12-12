using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature PurpleCube;
extern signature OrangeCube;
extern signature GreenCube;
extern signature SIG_4;
extern signature SIG_5;
extern signature SIG_6;
extern signature SIG_7;
extern signature PurpleCube2;
extern signature OrangeCube2;
extern signature GreenCube2;
extern signature SIG_42;
extern signature SIG_52;
extern signature SIG_62;
extern signature SIG_72;
extern vision Camera1;
extern vision Camera2;
extern controller Controller;
extern sonar Right;
extern sonar Rear;
extern vision Camera2;
extern limit ArmTilt;
extern line Feed;
extern line Feed2;
extern motor DriveFR;
extern motor DriveBR;
extern motor DriveBL;
extern motor DriveFL;
extern motor RampRaiserR;
extern motor RampRaiserL;
extern motor ArmTilterL;
extern motor ArmTilterR;
extern motor ArmSpinnerR;
extern motor ArmSpinnerL;
extern motor RampConveyer;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );