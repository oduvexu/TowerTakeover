



//progress for today
//worked on the priority function of the cubes to decide which cube to pick up

#include "vex.h"
//#include "Visionsensor.h"
using namespace vex;
int32_t Green_x;
int Green_y;


vex::brain       Brain;
vex::vision Camera(vex::PORT5);
vex::vision::signature PurpleCube = vex::vision::signature (1, 477, 1579, 1028, 5309, 7479, 6394, 2.8, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 6687, 8353, 7520, -1565, -941, -1254, 3.5, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -6735, -5105, -5920, -2757, -1339, -2048, 3.3, 0);

vex::motor DriveLF(PORT1,false);
vex::motor DriveRF(PORT2,true);
vex::motor DriveLB(PORT3,false);
vex::motor DriveRB(PORT4,true);
vex::controller Controller;
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
int j;                    //used in for loops
int i;                    //used in for loops
int d;                    //used in for loops
int lastpressed;          //used in changing driving modes
bool cubeFound = false;   //says whether the cube is within the desired frame in the turntocube function
int colorMultiplier = 0;
int distMultiplier = 500;
int proxMultiplier = 1;
int shapeMultiplier = 1;
int greenPriority = 1;   //biases for the priority function
int orangePriority = 1;  //1 is highest priority, 3 is lowest
int purplePriority = 1;
int currentPriority = 0;
int targetColor = 0;
int targetid = 0;
int targetDist;
bool targetFound = false;
bool turnDone = false;
int cubePriority = 0;
int gPriority = 0;
int oPriority = 0;
int pPriority = 0;
int t = 0;




class cube{
 public:
 bool cubestate;
 int cubeid; //used in the description of the cube object Ex: gcube()
 int cubeColor; //used in the desfription of the cube object
 int Ccolor = 0; //used in finding the priority
 int cubeDist;
 int cubeProx;
 int shape;
 int dif;
 
 void setData(int ID, int CUBECOLOR){   //used to set the color of each object and its id
   cubeid = ID;
   cubeColor = CUBECOLOR;
 }
 int getID(){
   return cubeid;                      //call this to get any cubes id
 }
 int getDistance(){                    //calculates the distance from the camera to the cube
    int Dist;
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
  int getProximity(){
      // not done yet
   cubeProx = 1;           
   return cubeProx;
  }
  int getShape(){          //not done yet
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
       shape = 1;
       return shape;
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
    cubePriority = (distMultiplier/getDistance());   
    // uses the distance and the color of the cubes to calculate the priority of each
    //colorMuliplier is set to 0 so we are pretty much only using the distance right now
     
    return cubePriority;
 }
 void draw(){
   
 
   int rectx;
   int recty;
   int rectw;
   int recth;
   int ink;
   double mag = .9;
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
    
   rectx = Camera.objects[cubeid].originX/mag;
   recty = Camera.objects[cubeid].originY/mag;
   rectw = Camera.objects[cubeid].width/mag;
   recth = Camera.objects[cubeid].height/mag;
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
cube gcubes[10];      //all of this stuff creates the cubes as objects
cube ocubes[10];                           
cube pcubes[10];
int main() {
  
  Brain.Screen.clearScreen();
 
  Camera.setSignature(OrangeCube);
  Camera.setSignature(PurpleCube);
  Camera.setSignature(GreenCube);
    while(1) { 
      
      
        for(i = 0; i<=9; i++){
         Camera.takeSnapshot(GreenCube);  //cycles through 10 objects for each color and sets them to be an object
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
      if(Controller.ButtonX.pressing()){
       
       findTarget();
       
      }
      //drive();
      turntocube();
   
  
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
   Brain.Screen.print("here: %u                ",j);     //prints thecube number it is checking for debugging purposes
   vex::task::sleep(5);          
  }
   Brain.Screen.setCursor(2,0);
   Brain.Screen.print("Target color: %u                ",targetColor);
   Brain.Screen.setCursor(3,0);
   Brain.Screen.print("Target priority: %u               ",currentPriority);
   Brain.Screen.setCursor(4,0);
   Brain.Screen.print("Target id: %u               ",targetid);
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
          if(d > 13){
            DriveLF.spin(fwd, -40-t,pct);   //the t's are used to bias the wheel values so that it can more precisely turn to the cube
            DriveRF.spin(fwd, -40+t,pct);
            DriveLB.spin(fwd, -40-t,pct);
            DriveRB.spin(fwd, -40+t,pct);
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
      DriveLF.spin(fwd, -Controller.Axis3.value()-Controller.Axis4.value()+Controller.Axis1.value(),pct); //forward is normal but axis 1 strafes left and right
      DriveRF.spin(fwd, -Controller.Axis3.value()+Controller.Axis4.value()-Controller.Axis1.value(),pct);
      DriveLB.spin(fwd, -Controller.Axis3.value()+Controller.Axis4.value()+Controller.Axis1.value(),pct);
      DriveRB.spin(fwd, -Controller.Axis3.value()-Controller.Axis4.value()-Controller.Axis1.value(),pct);
    }else{
      
      DriveLF.spin(fwd, -Controller.Axis3.value()+Controller.Axis1.value(),pct); //drives normally --- axis 3 controlls forward and back
      DriveRF.spin(fwd, -Controller.Axis3.value()-Controller.Axis1.value(),pct); //                    axis 1 turns
      DriveLB.spin(fwd, -Controller.Axis3.value()+Controller.Axis1.value(),pct);
      DriveRB.spin(fwd, -Controller.Axis3.value()-Controller.Axis1.value(),pct);
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