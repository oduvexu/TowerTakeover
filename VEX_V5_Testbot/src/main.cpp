



//progress for today
//finsihed and tuned the localization class

#include "vex.h"
//#include "Visionsensor.h"
using namespace vex;


vex::brain       Brain;
vex::vision Camera(vex::PORT5);
vex::vision::signature PurpleCube = vex::vision::signature (1, 477, 1579, 1028, 5309, 7479, 6394, 2.8, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 6687, 8353, 7520, -1565, -941, -1254, 3.5, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -6735, -5105, -5920, -2757, -1339, -2048, 3.3, 0);

vex::sonar right(Brain.ThreeWirePort.A);
vex::sonar rear(Brain.ThreeWirePort.C);

vex::motor DriveLF(PORT4,false);
vex::motor DriveRF(PORT3,true);
vex::motor DriveLB(PORT2,false);
vex::motor DriveRB(PORT1,true);
vex::controller Controller;
void findTarget();
void countobjects();      // function counting the number of cubes within the frame
void measureobjects();    //function measuring the position of on cube of each color and printing it to the screen
void printMultiple();     //function reading several different cubes of every color and printing it the the screen
void turntocube();        //turns the robot to the closest green cube when the 'X' button is pushed
void drive();             //drives the robot, switches modes using the 'A' button   
int radians();            
int G_num;                //the number of green cubes in the countobjects function
int O_num;                //the number of orange cubes in the countobjects function
int P_num;                //the number of purple cubes in the countobjects function
int cube_num;             //number of cubes in the countobjects function
int j;                    //used in for loops
int i;                    //used in for loops
int d;                    //used in for loops
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

class location{
  private:
 
    float LB1 = 0;
    float RF1 = 0;
    float LF1 = 0;
    float RB1 = 0;
    float LB2 = 0;
    float RF2 = 0;
    float LF2 = 0;
    float RB2 = 0; 

  public:
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
    rotate = 0;
    xoffset = 144 - (right.distance(distanceUnits::in)+5.5);
    yoffset =  rear.distance(distanceUnits::in)+7.5;

    
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
    LB1 = DriveLB.rotation(vex::rotationUnits::deg);
    LF1 = DriveLF.rotation(vex::rotationUnits::deg);
    RB1 = DriveRB.rotation(vex::rotationUnits::deg);
    RF1 = DriveRF.rotation(vex::rotationUnits::deg);
    Lspin =(((LB1-LB2)+(LF1-LF2))/2);
    Rspin =(((RB1-RB2)+(RF1-RF2))/2);
    s = s+(Lspin+Rspin)/2;
    rotate = rotate +(Lspin-Rspin)/(w);
    Distance = s*0.0174533*2;
    Rdistance = Rspin;
    Ldistance = Lspin;
    x = (Distance*sin(rotate*0.0174533))+xoffset;
    y = (Distance*cos(rotate*0.0174533))+yoffset;

  }
  float getAngle(){
    return rotate;
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

 public:         //variables that wil be modified and references within the program
    bool cubestate;
    int cubeid;              //used in the description of the cube object 
    int cubeColor;           //used in the desfription of the cube object
    int Ccolor = 0;          //used in finding the color priority
    int cubeDist;
    int cubeProx;
    int shape;
    int dif;
    int Dist;
    
 
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
        Camera.takeSnapshot(GreenCube);              //green
      break;
      case 2: 
        Camera.takeSnapshot(OrangeCube);             //orange
      break;
      case 1:
        Camera.takeSnapshot(PurpleCube);             //purple
        break;
    }
    i = Camera.objects[cubeid].width;
    Dist = 1655/i;
    //Dist = 1026*pow(i,-.876);
    return Dist;
 }
  int getProximity(){      // not done yet
   cubeProx = 0;           
   return cubeProx*proxMultiplier;
  }
  int getShape(){
    int s;         
    switch(cubeColor){      //will be used in the priority function to determine the confidence of the shape or the uncertainty
      case 3: 
        Camera.takeSnapshot(GreenCube);
      break;
      case 2: 
        Camera.takeSnapshot(OrangeCube);
      break;
      case 1:
        Camera.takeSnapshot(PurpleCube);
      break;
    }
    s = abs(Camera.objects[cubeid].width-Camera.objects[cubeid].height);
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
        Camera.takeSnapshot(GreenCube);
        ink = 100;
      break;
      case 2: 
        Camera.takeSnapshot(OrangeCube);
         ink = 30;
      break;
      case 1:
        Camera.takeSnapshot(PurpleCube);
         ink = 255;
        break;
   
    }
    
   rectx = Camera.objects[cubeid].originX;
   recty = Camera.objects[cubeid].originY;
   rectw = Camera.objects[cubeid].width;
   recth = Camera.objects[cubeid].height;
   Brain.Screen.setPenColor(kLedColorBlack);
   Brain.Screen.setPenColor(ink);
   Brain.Screen.drawRectangle(rectx, recty, rectw, recth);
 
  }
 bool exists(){
   switch(cubeColor){
      case 3: 
        Camera.takeSnapshot(GreenCube);
        break;
      case 2: 
        Camera.takeSnapshot(OrangeCube);
        break;
      case 1:
        Camera.takeSnapshot(PurpleCube);
        break;
   }
   if(Camera.objects[cubeid].exists){
     cubestate = true;
   }else{
     cubestate = false;
   }
    return cubestate;
  }
  void snap(){
   switch(cubeColor){
      case 0: 
        Camera.takeSnapshot(GreenCube);
      break;
      case 1: 
        Camera.takeSnapshot(OrangeCube);
      break;
      case 2:
        Camera.takeSnapshot(PurpleCube);
        break;
    }
 }
};

cube gcubes[10];      //creates objects for the cubes
cube ocubes[10];                           
cube pcubes[10];
location robot;
int main() {
  
  Brain.Screen.clearScreen();
 
  Camera.setSignature(OrangeCube);
  Camera.setSignature(PurpleCube);
  Camera.setSignature(GreenCube);

  
  while(1) { 
      
      
        for(i = 0; i<=9; i++){               //cycles through 10 objects for each color and sets them to be an object
         Camera.takeSnapshot(GreenCube);  
         if(Camera.objects[i].exists){
           gcubes[i].setData(i,3);
          }
          Camera.takeSnapshot(OrangeCube);
         if(Camera.objects[i].exists){
           ocubes[i].setData(i,2);
          }
          Camera.takeSnapshot(PurpleCube);
         if(Camera.objects[i].exists){
           pcubes[i].setData(i,1);
          }
      }
       
      
  if(Controller.ButtonA.pressing()){
  
    robot.setHome();
    robot.s = 0;
   DriveLF.setRotation(0, vex::rotationUnits::deg);
   DriveLB.setRotation(0, vex::rotationUnits::deg);
   DriveRF.setRotation(0, vex::rotationUnits::deg);
   DriveRB.setRotation(0, vex::rotationUnits::deg);
  }
if(Controller.ButtonX.pressing()){
       
       findTarget();
       
      }
      //drive();
      turntocube();
  
    robot.updatePosition();
    Brain.Screen.setCursor(4, 0);
    Brain.Screen.print("X position: %f          ", robot.getX());
    Brain.Screen.setCursor(5, 0);
    Brain.Screen.print("Y position: %f          ", robot.getY());
    Brain.Screen.setCursor(6, 0);
    Brain.Screen.print("Left rotation: %f          ", robot.Lspin);
    Brain.Screen.setCursor(7, 0);
    Brain.Screen.print("Right rotation: %f          ", robot.Rspin);
    Brain.Screen.setCursor(8, 0);
    Brain.Screen.print("Orientation: %f          ", robot.getAngle());
    Brain.Screen.setCursor(9, 0);
    Brain.Screen.print("distance: %f          ", robot.Distance);
   
  
}
}





void findTarget(){
  
  targetFound = false;
  turnDone = false;
  currentPriority = 0;
  t = 0;
  for(j = 0; j<=9; j++){                            //cycles through each color and sets the cube to the target if it has the highest priority
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
  Brain.Screen.setCursor(5,0);
  //Brain.Screen.print("here: %u                ",j);     //prints the cube number it is checking for debugging purposes
  vex::task::sleep(5);          
  }
   /*Brain.Screen.setCursor(2,0);
   Brain.Screen.print("Target color: %u                ",targetColor);
   Brain.Screen.setCursor(3,0);
   Brain.Screen.print("Target priority: %u               ",currentPriority);
   Brain.Screen.setCursor(4,0);
   Brain.Screen.print("Target id: %u               ",targetid); */
}


void turntocube(){           
 if(Controller.ButtonY.pressing() == true && targetFound == false && turnDone == false){   //turns to the target cube, then sets turndone to true so that it will go to the cube
   switch(targetColor){ 
     case 3: 
        Camera.takeSnapshot(GreenCube);
     break;
      case 2: 
        Camera.takeSnapshot(OrangeCube);
      break;
     case 1:
        Camera.takeSnapshot(PurpleCube);
     break;
    }
    if(turnDone == false){
      if(Camera.objects[targetid].centerX < 138){
        DriveLF.spin(fwd, -20,pct);
        DriveRF.spin(fwd,  20,pct);
        DriveLB.spin(fwd, -20,pct);
        DriveRB.spin(fwd,  20,pct);
      }else
      if(Camera.objects[targetid].centerX > 178){
        DriveLF.spin(fwd,  20,pct);
        DriveRF.spin(fwd, -20,pct);
        DriveLB.spin(fwd,  20,pct);
        DriveRB.spin(fwd, -20,pct);
      }else{
        turnDone = true;
      }
    }
  }else
  if(Controller.ButtonY.pressing() == true && targetFound == false && turnDone == true){   //goes forward to the target cube and turns slightly in order to tune the center of the cube to the center of the cube
    switch(targetColor){ 
      case 3: 
        d = gcubes[targetid].getDistance();
      break;
      case 2: 
        d = ocubes[targetid].getDistance();
      break;
      case 1:
        d = pcubes[targetid].getDistance();
        break;
    }
    switch(targetColor){ 
      case 3: 
        Camera.takeSnapshot(GreenCube);
      break;
      case 2: 
        Camera.takeSnapshot(OrangeCube);
      break;
      case 1:
        Camera.takeSnapshot(PurpleCube);
      break;
    }
    
  if(Camera.objects[targetid].centerX < 138){
   if(t<20 && t>-20){
     t++;
    }
  }else
  if(Camera.objects[targetid].centerX > 178){
    if(t<20 && t>-20){
      t--;
    }
  }else{
      t = 0;
  }
        
        if(d != 13){
          if(d > 14){
            DriveLF.spin(fwd, 40-t,pct);   //the t's are used to bias the wheel values so that it can more precisely turn to the cube
            DriveRF.spin(fwd, 40+t,pct);
            DriveLB.spin(fwd, 40-t,pct);
            DriveRB.spin(fwd, 40+t,pct);
          }else{
            targetFound = true;
          }
        }
        
      }else{
        drive();  // allows the driver to drive when the X button is not being held down
      }

}

void drive(){
   // switched the driving mode
      if(Controller.ButtonA.pressing() == true){ 
       lastpressed = ! lastpressed;
       vex::task::sleep( 200 );    //waits for the button to be released so that it is more stable
       }
  if(lastpressed == 0){
      DriveLF.spin(fwd, Controller.Axis3.value()+Controller.Axis4.value()+Controller.Axis1.value(),pct); //forward is normal but axis 1 strafes left and right
      DriveRF.spin(fwd, Controller.Axis3.value()-Controller.Axis4.value()-Controller.Axis1.value(),pct);
      DriveLB.spin(fwd, Controller.Axis3.value()-Controller.Axis4.value()+Controller.Axis1.value(),pct);
      DriveRB.spin(fwd, Controller.Axis3.value()+Controller.Axis4.value()-Controller.Axis1.value(),pct);
    }else{
      
      DriveLF.spin(fwd, Controller.Axis3.value()+Controller.Axis1.value(),pct); //drives normally --- axis 3 controlls forward and back
      DriveRF.spin(fwd, Controller.Axis3.value()-Controller.Axis1.value(),pct); //                    axis 1 turns
      DriveLB.spin(fwd, Controller.Axis3.value()+Controller.Axis1.value(),pct);
      DriveRB.spin(fwd, Controller.Axis3.value()-Controller.Axis1.value(),pct);
    }
    
}

void printMultiple(){                    // prints the locations and sizes of up to three cubes per color on the screen
  Brain.Screen.printAt(0,16,"Location:   OriginX      OriginY    W:");
  Camera.takeSnapshot(OrangeCube);
  for(i=0; i<=2; i++){           //goes through the first, second, then third of each color
  
  if(Camera.objects[i].exists ){
  Brain.Screen.setCursor(2+i,0);
  Brain.Screen.print("Orange%d:", i);
  Brain.Screen.setCursor(2+i,15);
  Brain.Screen.print(Camera.objects[i].originX);
  Brain.Screen.setCursor(2+i,28);
  Brain.Screen.print(Camera.objects[i].originY);
  Brain.Screen.setCursor(2+i,36);
  Brain.Screen.print(Camera.objects[i].width);

  }else{
  Brain.Screen.setCursor(2+i,0);
  Brain.Screen.clearLine();
    } 
  }

    Camera.takeSnapshot(GreenCube);
  for(i=0; i<=2; i++){
    
  if(Camera.objects[i].exists ){
  Brain.Screen.setCursor(5+i,0);
  Brain.Screen.print("Green%d:", i);
  Brain.Screen.setCursor(5+i,15);
  Brain.Screen.print(Camera.objects[i].originX);
  Brain.Screen.setCursor(5+i,28);
  Brain.Screen.print(Camera.objects[i].originY);
  Brain.Screen.setCursor(5+i,36);
  Brain.Screen.print(Camera.objects[i].width);

  }else{
  Brain.Screen.setCursor(5+i,0);
  Brain.Screen.clearLine();
    } 
  }

   Camera.takeSnapshot(PurpleCube);
  for(i=0; i<=2; i++){
    
  if(Camera.objects[i].exists ){
  Brain.Screen.setCursor(8+i,0);
  Brain.Screen.print("Purple%d:", i);
  Brain.Screen.setCursor(8+i,15);
  Brain.Screen.print(Camera.objects[i].originX);
  Brain.Screen.setCursor(8+i,28);
  Brain.Screen.print(Camera.objects[i].originY);
  Brain.Screen.setCursor(8+i,36);
  Brain.Screen.print(Camera.objects[i].width);

  }else{
  Brain.Screen.setCursor(8+i,0);
  Brain.Screen.clearLine();
    } 
  }
   
}

void measureobjects(){                 //prints only the largest cube of each color to the screen

  Camera.takeSnapshot(GreenCube);     

  if(Camera.largestObject.exists){
    Brain.Screen.setCursor(2, 0);
    Brain.Screen.print("Number of Green cubes: %d                  ", Camera.objectCount);
    Brain.Screen.setCursor(3,0);
    Brain.Screen.print("Location: X: %d", Camera.largestObject.originX);
    Brain.Screen.print("   Y: %d", Camera.largestObject.originY);
    Brain.Screen.print("   W: %d", Camera.largestObject.width);
    Brain.Screen.print("   H: %d", Camera.largestObject.height);
    Brain.Screen.print("   A: %d", Camera.largestObject.id);
  }else{
 
    Brain.Screen.setCursor(3,0);
    Brain.Screen.clearLine();
    Brain.Screen.setCursor(2,0);
    Brain.Screen.print("Largest Green cube: NONE IN FRAME");
  }
  
  
  
    Camera.takeSnapshot(OrangeCube);

  if(Camera.largestObject.exists){
    Brain.Screen.setCursor(4,0);
    Brain.Screen.print("Number of Orange cubes: %d                  ", Camera.objectCount);
    Brain.Screen.setCursor(5,0);
    Brain.Screen.print("Location: X: %d", Camera.largestObject.originX);
    Brain.Screen.print("   Y: %d", Camera.largestObject.originY);
    Brain.Screen.print("   W: %d", Camera.largestObject.width);
    Brain.Screen.print("   H: %d      ", Camera.largestObject.height);
  }else{
     
    Brain.Screen.setCursor(5,0);
    Brain.Screen.clearLine(); 
    Brain.Screen.setCursor(4,0);
    Brain.Screen.print("Largest Orange cube: NONE IN FRAME");
  }
    Camera.takeSnapshot(PurpleCube);

  if(Camera.largestObject.exists){
    Brain.Screen.setCursor(6,0);
    Brain.Screen.print("Number of Purple cubes: %d                  ", Camera.objectCount);
    Brain.Screen.setCursor(7,0);
    Brain.Screen.print("Location: X: %d", Camera.largestObject.originX);
    Brain.Screen.print("   Y: %d", Camera.largestObject.originY);
    Brain.Screen.print("   W: %d", Camera.largestObject.width);
    Brain.Screen.print("   H: %d      ", Camera.largestObject.height);
  }else{
     
    Brain.Screen.setCursor(7,0);
    Brain.Screen.clearLine();  
    Brain.Screen.setCursor(6,0);
    Brain.Screen.print("Largest Purple cube: NONE IN FRAME");
  }
  
}

void countobjects(){
  Camera.takeSnapshot(OrangeCube);
  O_num = Camera.objectCount;
  Camera.takeSnapshot(GreenCube);
  G_num = Camera.objectCount;
  Camera.takeSnapshot(PurpleCube);
  P_num = Camera.objectCount;
  cube_num = G_num + O_num + P_num;
  Brain.Screen.printAt(0, 19,"Number of blocks: %d ", cube_num);

}