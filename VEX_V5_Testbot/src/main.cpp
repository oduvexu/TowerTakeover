

// Robot Configuration: 
// [Smart Port]    [Name]        [Type]           [Description]       [Reversed]
// Motor Port 1    LeftMotor     V5 Smart Motor    Left side motor     false
// Motor Port 10   RightMotor    V5 Smart Motor    Right side motor    true



#include "vex.h"
//#include "Visionsensor.h"
using namespace vex;
int32_t Green_x;
int Green_y;


vex::brain       Brain;
vex::vision Camera(vex::PORT5);
vex::vision::signature PurpleCube = vex::vision::signature (1, 477, 1579, 1028, 5309, 7479, 6394, 2.7, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 6687, 8353, 7520, -1565, -941, -1254, 2, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -6735, -5105, -5920, -2757, -1339, -2048, 3.3, 0);

vex::motor DriveLF(PORT1,false);
vex::motor DriveRF(PORT2,true);
vex::motor DriveLB(PORT3,false);
vex::motor DriveRB(PORT4,true);
vex::controller Controller;

void countobjects();
void measureobjects();
int count;
int G_num;
int O_num;
int P_num;
int cube_num;






int main() {
  
  Camera.setSignature(OrangeCube);
  Camera.setSignature(PurpleCube);
  Camera.setSignature(GreenCube);
    while(1) { 
     
      countobjects();
      measureobjects();
      DriveLF.spin(fwd, Controller.Axis3.value()+Controller.Axis1.value(),pct);
      DriveRF.spin(fwd, Controller.Axis3.value()-Controller.Axis1.value(),pct);
      DriveLB.spin(fwd, Controller.Axis3.value()+Controller.Axis1.value(),pct);
      DriveRB.spin(fwd, Controller.Axis3.value()-Controller.Axis1.value(),pct);
      

      //--------------------(Y, x) from top left
      Brain.Screen.printAt(0, 19,"Number of blocks: %d", cube_num);
     
      
      
        
     
     
    
    }
    


}









void measureobjects(){

Camera.takeSnapshot(GreenCube);

if(Camera.largestObject.exists){
Brain.Screen.setCursor(2, 0);
//--------Space used to clear not in frame
Brain.Screen.print("Largest Green cube: %d                  ", Camera.largestObject.id);
Brain.Screen.setCursor(3,0);
Brain.Screen.print("Location: X: %d", Camera.largestObject.originX);
Brain.Screen.print("   Y: %d", Camera.largestObject.originY);
Brain.Screen.print("   W: %d", Camera.largestObject.width);
Brain.Screen.print("   H: %d     ", Camera.largestObject.height);
  }else{
//----------(line to be cleared, hue)  
Brain.Screen.setCursor(3,0);
Brain.Screen.clearLine(); //clearing the line showing the coordinates
Brain.Screen.setCursor(2,0);
Brain.Screen.print("Largest Green cube: NONE IN FRAME");
  }
Camera.takeSnapshot(OrangeCube);

if(Camera.largestObject.exists){
Brain.Screen.setCursor(4,0);
Brain.Screen.print("Largest Orange cube: %d                 ", Camera.largestObject.id);
Brain.Screen.setCursor(5,0);
Brain.Screen.print("Location: X: %d", Camera.largestObject.originX);
Brain.Screen.print("   Y: %d", Camera.largestObject.originY);
Brain.Screen.print("   W: %d", Camera.largestObject.width);
Brain.Screen.print("   H: %d      ", Camera.largestObject.height);
  }else{
    //----------(line to be cleared, hue)  
    Brain.Screen.setCursor(5,0);
Brain.Screen.clearLine(); //clearing the line showing the coordinates
Brain.Screen.setCursor(4,0);
Brain.Screen.print("Largest Orange cube: NONE IN FRAME");
  }
  Camera.takeSnapshot(PurpleCube);

if(Camera.largestObject.exists){
Brain.Screen.setCursor(6,0);
Brain.Screen.print("Largest Purple cube: %d                 ", Camera.largestObject.id);
Brain.Screen.setCursor(7,0);
Brain.Screen.print("Location: X: %d", Camera.largestObject.originX);
Brain.Screen.print("   Y: %d", Camera.largestObject.originY);
Brain.Screen.print("   W: %d", Camera.largestObject.width);
Brain.Screen.print("   H: %d      ", Camera.largestObject.height);
  }else{
    //----------(line to be cleared, hue)  
Brain.Screen.setCursor(7,0);
Brain.Screen.clearLine();  //clearing the line showing the coordinates
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

}