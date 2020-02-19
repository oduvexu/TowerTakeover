using namespace vex;

extern brain Brain;

using signature = vision::signature;
extern smartdrive Drivetrain;
// VEXcode devices
extern signature PurpleCube;
extern signature OrangeCube;
extern signature GreenCube;
extern signature RedGoal;
extern signature SIG_5;
extern signature SIG_6;
extern signature SIG_7;
extern signature PurpleCube2;
extern signature OrangeCube2;
extern signature GreenCube2;
extern signature PurpleCube3;
extern signature OrangeCube3;
extern signature GreenCube3;
extern signature RedGoal2;
extern signature SIG_52;
extern signature SIG_62;
extern signature SIG_72;
extern signature SIG_43;
extern signature SIG_53;
extern signature SIG_63;
extern signature SIG_73;
extern vision Camera1;
extern vision Camera2;
extern vision Camera3;
extern controller Controller;


extern sonar Intake;
extern vision Camera2;
extern limit ArmTilt;
extern line Feed;
extern line FeedFull;
extern line RightLine;
extern line LeftLine;
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
extern motor ArmCompress;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );