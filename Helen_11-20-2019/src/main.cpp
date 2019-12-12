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
//defining the color signatures of the cubes
vex::vision::signature PurpleCube = vex::vision::signature (1,  -5851, -3465, -4658, -3667, -1731, -2698, 2.5, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 9007, 9985, 9496, -2837, -2441, -2640, 3, 0);
vex::vision::signature GreenCube  = vex::vision::signature (3, -6639, -4373, -5506, -3223, -1851, -2536, 2.5, 0);
// define your global instances of motors and other devices here
vex::sonar Right(Brain.ThreeWirePort.F);
vex::sonar Rear(Brain.ThreeWirePort.H);
vex::vision Camera1(PORT19);
vex::vision Camera2(PORT20);
vex::bumper ArmTilt(Brain.ThreeWirePort.A);
vex::line Feed(Brain.ThreeWirePort.B);
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
void findTarget();
void countobjects();      // function counting the number of cubes within the frame
void measureobjects();    //function measuring the position of on cube of each color and printing it to the screen
void printMultiple();     //function reading several different cubes of every color and printing it the the screen
void turntocube();        //turns the robot to the closest green cube when the 'X' button is pushed
void drive();             //drives the robot, switches modes using the 'A' button           
int G_num;                //the number of green cubes in the countobjects function
int O_num;                //the number of orange cubes in the countobjects function
int P_num;                //the number of purple cubes in the countobjects function
int cube_num;             //number of cubes in the countobjects function
int j, i, d;                    //used in for loops
float b;           
float s;
int lastpressed;          //used in changing driving modes
bool cubeFound = false;   //says whether the cube is within the desired frame in the turntocube function
int targetColor = 0;
int targetid = 0;
int targetDist;
bool targetFound = false;
bool turnDone = false;
int t = 0;
int  gPriority = 0; 
int oPriority = 0;
int pPriority = 0;
int currentPriority = 0;
bool Home = true;
int BL = 0;
int BR = 0;
int FL = 0;
int FR = 0;
float rad = 0.0174532925;
bool tiltDone = false;
bool bumpDone = false;

class location{
  private:
   float tBias = -1.26;
    float LB1 = 0;
    float RF1 = 0;
    float LF1 = 0;
    float RB1 = 0;
    float LB2 = 0;
    float RF2 = 0;
    float LF2 = 0;
    float RB2 = 0; 

  public:
  float Rotation;
    float Rdistance;
    float Ldistance;
    float Distance;
    float s;
    float Lspin;
    float Rspin;
    float xoffset = 0;
    float yoffset = 0;
    float x = 0;
    float y = 0;
    float rotate = 0;
    int baseX;
    int baseY;
  void setBase(){
    
  }
  void setHome(){
    x = 0;
    y = 0;
    Rotation = 0;
    Distance = 0;
    s = 0;
    s = 0;
    rotate = 0;
    xoffset = 0;//144 - (right.distance(distanceUnits::in)+5.5);
    yoffset =  0;//rear.distance(distanceUnits::in)+7.5;

    
  }
  float getX(){
    return x;
  }
  float getY(){
    return y;
  }
  void updatePosition(){
    
    float w = 13;  //w = width of the robot
    LB2 = LB1;
    LF2 = LF1;
    RB2 = RB1;
    RF2 = RF1;
    LB1 = DriveBL.rotation(vex::rotationUnits::deg)*0.0174533; //angles in radians
    LF1 = DriveFL.rotation(vex::rotationUnits::deg)*0.0174533;
    RB1 = DriveBR.rotation(vex::rotationUnits::deg)*0.0174533;
    RF1 = DriveFR.rotation(vex::rotationUnits::deg)*0.0174533;
    Lspin =(((LB1-LB2)+(LF1-LF2))); //calculates circumfrence travelled
    Rspin =(((RB1-RB2)+(RF1-RF2))); //calculates circumfrence travelled
    s = ((Lspin+Rspin)/2);  // change in distance travelled since last loop
    Distance = Distance +s; //true distance travelled
    rotate = (((Lspin-Rspin))/((w+tBias)*2)); //change in angle in degrees since last loop
    Rotation = Rotation + rotate;
    Rdistance = Rspin;
    Ldistance = Lspin;
    x = x +(s*cos(Rotation))+xoffset;
    y = y + (s*sin(Rotation))+yoffset;

  }
  float getAngle(){
    return Rotation/0.0174533;
  }
};
class cube{
 private:        //variables declared that will not be modified within the program
    int colorMultiplier = 0;
    int distMultiplier = 500;
    int proxMultiplier = 1;
    int shapeMultiplier = 1;
    int cubePriority = 0;
    int greenPriority = 10;   //biases for the priority function
    int orangePriority = 5;  //from 1 to 10, 10 is highest priority
    int purplePriority = 5;
    double w;
    float a;
    float b;
    float c;
    float A;
    float B;
    float C;

 public:         //variables that wil be modified and references within the program
    bool cubestate;
    int cubeid;              //used in the description of the cube object 
    int cubeColor;           //used in the desfription of the cube object
    int Ccolor = 0;          //used in finding the color priority
    int cubeDist;
    int cubeProx;
    int shape;
    int dif;
    double Dist;
    double Dist2;
    
 
 void setData(int ID, int CUBECOLOR){   //used to set the color of each object and its id
    cubeid = ID;
    cubeColor = CUBECOLOR;
  }
 int getID(){
    return cubeid;                      //call this to get any cubes id
  }
 int getDistance(){                     //calculates the distance from the camera to the cube
    switch(cubeColor){
      case 3:
       Camera1.takeSnapshot(GreenCube);   
        Camera2.takeSnapshot(GreenCube);              //green
      break;
      case 2:
        Camera1.takeSnapshot(OrangeCube);   
        Camera2.takeSnapshot(OrangeCube);             //orange
      break;
      case 1:
       Camera1.takeSnapshot(PurpleCube);  
        Camera2.takeSnapshot(PurpleCube);             //purple
        break;
    }
    if(Camera1.objects[cubeid].exists && Camera2.objects[cubeid].exists){
      w = ((Camera2.objects[cubeid].width+Camera1.objects[cubeid].width)/2);
    }else
    if(Camera1.objects[cubeid].exists && !Camera2.objects[cubeid].exists){
      w = Camera1.objects[cubeid].width;
    }else
    if(!Camera1.objects[cubeid].exists && Camera2.objects[cubeid].exists){
      w = Camera2.objects[cubeid].width;
    }
    //Dist = 1632/w;
    //Dist = 1026*pow(w,-.876);
    Dist = (0.00275132275132*pow(w, 2))-(0.744973544974*w)+(62.9360846561);

    a = 90-abs(((Camera1.objects[cubeid].centerX*0.284810126582)-45));
    b = 90-abs(((Camera2.objects[cubeid].centerX*0.284810126582)-45));
    c = 180 - (a+b);
    A = a*rad;
    B = b*rad;
    C = c*rad;
    Dist2 = (17.25*sin(A)*sin(B)/sin(C));
    return Dist;
  }
 int getAngle(){
   {
   switch(cubeColor){
      case 3:
       Camera1.takeSnapshot(GreenCube);   
        Camera2.takeSnapshot(GreenCube);              //green
      break;
      case 2:
        Camera1.takeSnapshot(OrangeCube);   
        Camera2.takeSnapshot(OrangeCube);             //orange
      break;
      case 1:
       Camera1.takeSnapshot(PurpleCube);  
       Camera2.takeSnapshot(PurpleCube);             //purple
       break;
    }
    double Angle;
    if(Camera1.objects[cubeid].exists && Camera2.objects[cubeid].exists){
      Angle = (((Camera1.objects[cubeid].centerX*0.284810126582)-45)+((Camera2.objects[cubeid].centerX*0.284810126582)-45))/2;
    }else
    if(Camera1.objects[cubeid].exists && !Camera2.objects[cubeid].exists){
      Angle = ((Camera1.objects[cubeid].centerX*0.284810126582)-45);
    }else
    if(!Camera1.objects[cubeid].exists && Camera2.objects[cubeid].exists){
      Angle = ((Camera2.objects[cubeid].centerX*0.284810126582)-45);
    }else{
      Angle = 0;
    }

   return Angle;
  
  }
  }
 int getProximity(){      // not done yet
   cubeProx = 0;           
   return cubeProx*proxMultiplier;
  }
 int getShape(){
   int s;         
   switch(cubeColor){      //will be used in the priority function to determine the confidence of the shape or the uncertainty
    case 3: 
      Camera1.takeSnapshot(GreenCube);
      Camera2.takeSnapshot(GreenCube);
    break;
    case 2: 
      Camera1.takeSnapshot(OrangeCube);
      Camera2.takeSnapshot(OrangeCube);
    break;
    case 1:
      Camera1.takeSnapshot(PurpleCube);
      Camera2.takeSnapshot(PurpleCube);
    break;
  }
    s = abs(Camera1.objects[cubeid].width-Camera1.objects[cubeid].height);
    if(s<50 && s != 0){
      shape = (-s/4)+10;
    }else
    if(s == 0){
    shape = 10;
    }else{
      shape = 0;
    }
  
    return shape*shapeMultiplier;
  }
 int getPriority(){
   switch(cubeColor){                               //will set Ccolor to either 1, 2, or 3 based on what the user has set the priorities of each color to be
      case 3: 
        Ccolor = greenPriority;              //green
      break;
      case 2: 
        Ccolor = orangePriority;             //orange
      break;
      case 1:
        Ccolor = purplePriority;            //purple
      break;
    }
    cubePriority = (distMultiplier/getDistance()+(Ccolor*colorMultiplier)+getShape());   
    // uses the distance and the color of the cubes to calculate the priority of each
    //colorMuliplier is set to 0 so we are pretty much only using the distance right now
     
    return cubePriority;
  }
 void draw(){        //draws rectangles on the Brain Screen relative to where they are fro the perspective of the camera
   int rectx;
   int recty;
   int rectw;
   int recth;
   int ink;
   
   switch(cubeColor){
      case 3: 
        Camera1.takeSnapshot(GreenCube);
        ink = 100;
      break;
      case 2: 
        Camera1.takeSnapshot(OrangeCube);
         ink = 30;
      break;
      case 1:
        Camera1.takeSnapshot(PurpleCube);
         ink = 255;
        break;
   
    }
    
   rectx = Camera1.objects[cubeid].originX;
   recty = Camera1.objects[cubeid].originY;
   rectw = Camera1.objects[cubeid].width;
   recth = Camera1.objects[cubeid].height;
   Brain.Screen.setPenColor(kLedColorBlack);
   Brain.Screen.setPenColor(ink);
   Brain.Screen.drawRectangle(rectx, recty, rectw, recth);
 
  }
 bool exists(){
   switch(cubeColor){
      case 3: 
        Camera1.takeSnapshot(GreenCube);
        break;
      case 2: 
        Camera1.takeSnapshot(OrangeCube);
        break;
      case 1:
        Camera1.takeSnapshot(PurpleCube);
        break;
   }
   if(Camera1.objects[cubeid].exists){
     cubestate = true;
   }else{
     cubestate = false;
   }
    return cubestate;
  }
 void snap(){
   switch(cubeColor){
      case 0: 
        Camera1.takeSnapshot(GreenCube);
      break;
      case 1: 
        Camera1.takeSnapshot(OrangeCube);
      break;
      case 2:
        Camera1.takeSnapshot(PurpleCube);
        break;
    }
  }
};
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

  while(!ArmTilt.pressing()){
    RampRaiserR.spin(fwd, -60, pct);
     RampRaiserL.spin(fwd, -60, pct);
 }
    RampRaiserL.setRotation(0, vex::rotationUnits::deg);
    RampRaiserR.setRotation(0, vex::rotationUnits::deg);
  
  
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

void usercontrol( void ) {

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
 if(Controller.ButtonB.pressing()){
   while(!ArmTilt.pressing()){
    RampRaiserR.spin(fwd, -30, pct);
    RampRaiserL.spin(fwd, -30, pct);
 }
    RampRaiserL.setRotation(0, vex::rotationUnits::deg);
    RampRaiserR.setRotation(0, vex::rotationUnits::deg);
    
 }
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
   // RampRaiserL.setRotation(1000, vex::rotationUnits::deg);
    //RampRaiserR.setRotation(1000, vex::rotationUnits::deg);
    //Run the pre-autonomous function. 
    pre_auton();
    cube gcubes[10];      //creates objects for the cubes
    cube ocubes[10];                           
    cube pcubes[10];
    location robot;
  
     
    
    Controller.Screen.clearScreen();

    //Prevent main from exiting with an infinite loop.                        
    while(1) {
        for(i = 0; i<=9; i++){               //cycles through 10 objects for each color and sets them to be an object
         Camera1.takeSnapshot(GreenCube);
        
         if(Camera1.objects[i].exists){
           gcubes[i].setData(i,3);
          }
          Camera1.takeSnapshot(OrangeCube);
          
         if(Camera1.objects[i].exists){
           ocubes[i].setData(i,2);
          }
          Camera1.takeSnapshot(PurpleCube);
       
         if(Camera1.objects[i].exists){
           pcubes[i].setData(i,1);
          }
      }
      drivecontrol();
      armcontrol();
      rampcontrol();
      vex::task::sleep(10);//Sleep the task for a short amount of time to prevent wasted resources.
      Brain.Screen.setCursor(1, 0);
      Brain.Screen.print(" %d", Feed.value(pct));
      Camera1.takeSnapshot(GreenCube);
      Camera2.takeSnapshot(GreenCube);
      if(Camera1.largestObject.exists){
      Brain.Screen.setCursor(1, 10);
      Brain.Screen.print("Cam1  %d ", Camera1.largestObject.exists);
      Brain.Screen.setCursor(2, 10);
      Brain.Screen.print("Cam2  %d  ", Camera2.largestObject.exists);
    }    }
       
}

void drivecontrol(){
 if(!Controller.ButtonX.pressing()){
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
}

void armcontrol(){
  //arm tilt controls
  if(Controller.ButtonUp.pressing()==true)
  {
    ArmTilterL.spin(fwd,30,pct);
    ArmTilterR.spin(fwd,30,pct);
  }
  else if (Controller.ButtonDown.pressing()==true){
    ArmTilterL.spin(fwd,-30,pct);
    ArmTilterR.spin(fwd,-30,pct);
  }else{
    ArmTilterL.stop(hold);
    ArmTilterR.stop(hold);
  }
  


  if(Controller.ButtonR1.pressing()==true){
    ArmSpinnerL.spin(fwd,70,pct);
    ArmSpinnerR.spin(fwd,70,pct);
    RampConveyer.spin(fwd,70,pct);
   }else
    if(Controller.ButtonR2.pressing()==true)
    {
      ArmSpinnerL.spin(fwd,-70,pct);
      ArmSpinnerR.spin(fwd,-70,pct);
      RampConveyer.spin(fwd,-70,pct);
    }
    else
    {
      ArmSpinnerL.spin(fwd,-Controller.Axis2.value(),pct);
    ArmSpinnerR.spin(fwd,-Controller.Axis2.value(),pct);
    RampConveyer.spin(fwd,-Controller.Axis2.value(),pct);
  }
}

void rampcontrol(){

 if(Controller.ButtonL1.pressing()==true && RampRaiserL.rotation(deg)<=780 && RampRaiserR.rotation(deg)<=780){
    RampRaiserL.spin(fwd,30,pct);
    RampRaiserR.spin(fwd,30,pct);
  }else
  if(Controller.ButtonL2.pressing()==true && RampRaiserL.rotation(deg)>15 && RampRaiserR.rotation(deg)>15){
      RampRaiserL.spin(fwd,-30,pct);
      RampRaiserR.spin(fwd,-30,pct);
  }else{
    RampRaiserL.stop(hold);
    RampRaiserR.stop(hold);
  }

  if(Controller.ButtonX.pressing()){
    while(RampRaiserL.rotation(deg)<=770 && RampRaiserR.rotation(deg)<=770 && tiltDone == false){
    RampRaiserL.spin(fwd, 20,pct);
    RampRaiserR.spin(fwd, 20,pct);
    
    }
    tiltDone = true;
    if(tiltDone == true && bumpDone == false){
       bumpDone = true;
    }

    vex::task::sleep(250);
    DriveFL.stop(brake);
    DriveFR.stop(brake);
    DriveBL.stop(brake);
    DriveBR.stop(brake);
    vex::task::sleep(1000);

    DriveFL.spin(fwd, -20, pct);
    DriveFR.spin(fwd, -20, pct);
    DriveBL.spin(fwd, -20, pct);
    DriveBR.spin(fwd, -20, pct);
    ArmSpinnerL.spin(fwd,100,pct);
    ArmSpinnerR.spin(fwd,100,pct);
    RampConveyer.spin(fwd,100,pct);

    vex::task::sleep(1000);
    DriveFL.spin(fwd, 0, pct);
    DriveFR.spin(fwd, 0, pct);
    DriveBL.spin(fwd, 0, pct);
    DriveBR.spin(fwd, 0, pct);
    ArmSpinnerL.spin(fwd,0,pct);
    ArmSpinnerR.spin(fwd,0,pct);
    RampConveyer.spin(fwd,0,pct);
   
     
    
  }else{
    bumpDone = false;
    tiltDone = false;
  }

  if(ArmTilt.pressing()){
    RampRaiserL.setRotation(0, vex::rotationUnits::deg);
    RampRaiserR.setRotation(0, vex::rotationUnits::deg);
    RampRaiserL.rotateFor(fwd,15,deg);
    RampRaiserR.rotateFor(fwd,15,deg);
  }

  
}

