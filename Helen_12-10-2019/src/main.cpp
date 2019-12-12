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
// Vision13             vision        13              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "VisionSensor1.h"
#include "VisionSensor2.h"
using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
  void drivecontrol();
  void armcontrol();
  void rampcontrol();
  void findTarget();
  void countobjects();      // function counting the number of cubes within the frame
  void measureobjects();    //function measuring the position of on cube of each color and printing it to the screen
  void printMultiple();     //function reading several different cubes of every color and printing it the the screen
  void approachcube();        //turns the robot to the closest green cube when the 'X' button is pushed
  void reset();              //resets all functions of robot
  void drive();             //drives the robot, switches modes using the 'A' button           
  int G_num;                //the number of green cubes in the countobjects function
  int O_num;                //the number of orange cubes in the countobjects function
  int P_num;                //the number of purple cubes in the countobjects function
  int cube_num;             //number of cubes in the countobjects function
  int j, i, d;                    //used in for loops
  float b;           
  int s;
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
  bool grabDone = false;
  bool resetDone = false;

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
    Distance = Distance + s; //true distance travelled
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
    int orangePriority = 5;   //from 1 to 10, 10 is highest priority
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

    a = 90-abs(int((Camera1.objects[cubeid].centerX*0.284810126582)-45));
    b = 90-abs(int((Camera2.objects[cubeid].centerX*0.284810126582)-45));
    c = 180 - (a+b);
    A = a*rad;
    B = b*rad;
    C = c*rad;
    
    Dist2 = (17.25*sin(A)*sin(B)/sin(C));    

   if(Dist<32){
    return Dist2;
   }else{
     return Dist;
   }
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
   cubePriority = 0;
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
    if(Camera1.objects[cubeid].exists || Camera2.objects[cubeid].exists){
    cubePriority = (distMultiplier/getDistance());   
    }else{
      cubePriority = 0;
    }
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
cube gcubes[10];      //creates objects for the cubes
cube ocubes[10];                           
cube pcubes[10];
location robot;
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
  cube gcubes[10];      //creates objects for the cubes
  cube ocubes[10];                           
  cube pcubes[10];
  location robot;
  
  while(!ArmTilt.pressing()){
    RampRaiserR.spin(fwd, -100, pct);
     RampRaiserL.spin(fwd, -100, pct);
     
  }
    RampRaiserL.setRotation(0, vex::rotationUnits::deg);
    RampRaiserR.setRotation(0, vex::rotationUnits::deg);
    
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

void usercontrol(void) {
  // User control code here, inside the loop
     
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
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
    if(Controller.ButtonA.pressing()){
      repeat(3){
       findTarget();
      }
    
    Controller.Screen.setCursor(1, 0);
    Controller.Screen.print("%d  ", turnDone);
    Controller.Screen.setCursor(1, 3);
    Controller.Screen.print("%d  ", targetFound);
    Controller.Screen.setCursor(1, 6);
    Controller.Screen.print("%d  ", grabDone);
    Controller.Screen.setCursor(2,0);
    Controller.Screen.print("%d  ", targetColor);
    Controller.Screen.setCursor(2, 3);
    Controller.Screen.print(" %d  ", targetid);
     Controller.Screen.setCursor(3, 0);
    Controller.Screen.print("%d  ", int(gcubes[0].Dist));
    Controller.Screen.setCursor(3, 4);
    Controller.Screen.print("%d  ", int(gcubes[0].Dist2));
    Controller.Screen.setCursor(3, 8);
    //Controller.Screen.print("%d  ", int(gcubes[targetid].getDistance()));
    
    
      
      }
      approachcube();
  
   
  }


//
// Main will set up the competition functions and callbacks.
//

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  Controller.Screen.clearScreen();
  
  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (1) {
    
      
       usercontrol();
        
       if(Controller.ButtonB.pressing()){
         resetDone = false;
       }
         while(resetDone == false){
         reset();
         }
       
      
  }
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

void findTarget(){
  
  targetColor = 0;
  currentPriority = 0;
  b = 0;
  targetid = 0;
  for(j = 0; j<10; j++){                            //cycles through each color and sets the cube to the target if it has the highest priority
    gPriority = gcubes[j].getPriority();  //sets each of the colors priority before testing each
    oPriority = ocubes[j].getPriority();
    pPriority = pcubes[j].getPriority();
    
   if(gPriority > currentPriority){
     currentPriority = gPriority;
     targetColor = 3;
     targetid = j;
    }
   if(oPriority > currentPriority){
     currentPriority = oPriority;
     targetColor = 2;
     targetid = j;
    }
   if(pPriority > currentPriority){
     currentPriority = pPriority;
     targetColor = 1;
     targetid = j;
    }
  }
        
  
 targetFound = false;
  turnDone = false;
  grabDone = false;
}

void approachcube(){
 
 if(Controller.ButtonY.pressing() && targetFound == false && turnDone == false && grabDone == false){   //turns to the target cube, then sets turndone to true so that it will go to the cube
   switch(targetColor){ 
     case 3: 
        Camera1.takeSnapshot(GreenCube);
        s = int(gcubes[targetid].getAngle());
     break;
      case 2: 
        Camera1.takeSnapshot(OrangeCube);
        s = int(ocubes[targetid].getAngle());
      break;
     case 1:
        Camera1.takeSnapshot(PurpleCube);
       s = int(pcubes[targetid].getAngle());
     break;
    }
    
      if(s < -10){
        DriveFL.spin(fwd, s,pct);
        DriveFR.spin(fwd,  abs(s),pct);
        DriveBL.spin(fwd, s/2,pct);
        DriveBR.spin(fwd,  abs(s/2),pct);
      }else
      if(s > 10){
        DriveFL.spin(fwd, s,pct);
        DriveFR.spin(fwd,  -s,pct);
        DriveBL.spin(fwd, s/2,pct);
        DriveBR.spin(fwd, -s/2,pct);
      }else{
        if(s<10 && s > -10){
        turnDone = true;
        }
      }
    
  }else
  if(Controller.ButtonY.pressing() && targetFound == false && turnDone == true && grabDone == false){   //goes forward to the target cube and turns slightly in order to tune the center of the cube to the center of the cube
    switch(targetColor){ 
     case 3: 
   
        s = gcubes[targetid].getAngle();
        d = gcubes[targetid].getDistance();
     break;
      case 2: 
        
        s = ocubes[targetid].getAngle();
        d = ocubes[targetid].getDistance();
      break;
     case 1:
    
        s = pcubes[targetid].getAngle();
        d = pcubes[targetid].getDistance();
     break;
    }
     
    
  if(s < -10){
   if(b<20 && b>-20){
     b =b+2;
    }
  }else
  if(s > 10){
    if(b<20 && b>-20){
      b =b-2;
    }
  }else{
      b = 0;
  }
        
        
          if(d > 15){
            DriveFL.spin(fwd, 40-b,pct);   //the t's are used to bias the wheel values so that it can more precisely turn to the cube
            DriveFR.spin(fwd, 40+b,pct);
            DriveBL.spin(fwd, 40-b,pct);
            DriveBR.spin(fwd, 40+b,pct);
          }else{
            targetFound = true;
          }
        
        
      }else
      if(Controller.ButtonY.pressing() && targetFound == true && turnDone == true && grabDone == false){
       if(Feed.value(pct) == 70){
          ArmSpinnerL.spin(fwd,-70,pct);
          ArmSpinnerR.spin(fwd,-70,pct);
          RampConveyer.spin(fwd,-70,pct);
          DriveFL.spin(fwd, 15,pct);  
          DriveFR.spin(fwd, 15,pct);
          DriveBL.spin(fwd, 15,pct);
          DriveBR.spin(fwd, 15,pct);
      
       }else{
         grabDone = true;
       }
       
       
      }else{
        
        drivecontrol();
        armcontrol();
        rampcontrol();  // allows the driver to drive when the Y button is not being held down
      }

}

void reset(){
  ArmTilterL.setStopping(coast);
  ArmTilterR.stop(coast);
  RampRaiserL.stop(coast);
  RampRaiserR.stop(coast);
  vex::task::sleep(2000);
  pre_auton();
  resetDone = true;
}
