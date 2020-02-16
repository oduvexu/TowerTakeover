



//progress for today
//create the cube class and calculate distance

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
int lastpressed;          //used in changing driving modes
bool cubeFound = false;   //says whether the cube is within the desired frame in the turntocube function

class cube
{ 
  public:
  int cubeid;
  int cubeColor;
  int targetCube;
 
  cube(int ID, int CUBECOLOR){
  cubeid = ID;
  cubeColor = CUBECOLOR;
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
   int distance(){
     int Dist;
     int i;
     Camera.takeSnapshot(GreenCube);
     i = Camera.objects[0].width;
     Dist = 1655/i;
     return Dist;
   }
};

int main() {
  
  Brain.Screen.clearScreen();
  Camera.setSignature(OrangeCube);
  Camera.setSignature(PurpleCube);
  Camera.setSignature(GreenCube);
    while(1) { 
      cube gcube(0,3);
   






      //countobjects();
      //measureobjects();
      //printMultiple();
      //turntocube();
      //drive();

     
      
        
     
     
   
    }
    


}





void turntocube(){           //changes the wheel speeds and positions based on where the largest green cube is and when the 'X' button is being pressed
  if(Controller.ButtonX.pressing() == true && cubeFound == false){ 
      Camera.takeSnapshot(GreenCube);
        if(Camera.largestObject.originX != 158){
          if(Camera.largestObject.originX < 100){
            DriveLF.spin(fwd, -10,pct);
            DriveRF.spin(fwd, 10,pct);
            DriveLB.spin(fwd, -10,pct);
            DriveRB.spin(fwd, 10,pct);
          }else
          if(Camera.largestObject.originX > 200){
            DriveLF.spin(fwd, 10,pct);
            DriveRF.spin(fwd, -10,pct);
            DriveLB.spin(fwd, 10,pct);
            DriveRB.spin(fwd, -10,pct);
          }else
          if(Camera.largestObject.originX > 148 || Camera.largestObject.originX < 168){
            DriveLF.spin(fwd, -2,pct);
            DriveRF.spin(fwd, 2,pct);
            DriveLB.spin(fwd, -2,pct);
            DriveRB.spin(fwd, 2,pct);
          }else
          if(Camera.largestObject.originX < 168){
            DriveLF.spin(fwd, 2,pct);
            DriveRF.spin(fwd, -2,pct);
            DriveLB.spin(fwd, 2,pct);
            DriveRB.spin(fwd, -2,pct);
          }else{
            cubeFound = true;
          }
        }
        
      }

}

void drive(){
   // switched the driving mode
      if(Controller.ButtonA.pressing() == true){ 
       lastpressed = ! lastpressed;
       vex::task::sleep( 200 );    //waits for the button to be released so that it is more stable
        }
  if(lastpressed == 0){
      DriveLF.spin(fwd, Controller.Axis3.value()-Controller.Axis1.value(),pct); //drives normally --- axis 3 controlls forward and back
      DriveRF.spin(fwd, Controller.Axis3.value()+Controller.Axis1.value(),pct); //                    axis 1 turns
      DriveLB.spin(fwd, Controller.Axis3.value()-Controller.Axis1.value(),pct);
      DriveRB.spin(fwd, Controller.Axis3.value()+Controller.Axis1.value(),pct);
    }else{
      DriveLF.spin(fwd, Controller.Axis3.value()-Controller.Axis1.value(),pct); //forward is normal but axis 1 strafes left and right
      DriveRF.spin(fwd, Controller.Axis3.value()+Controller.Axis1.value(),pct);
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