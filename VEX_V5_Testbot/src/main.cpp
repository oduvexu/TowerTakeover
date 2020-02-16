
#include "vex.h"
//#include "Visionsensor.h"
using namespace vex;
int32_t Green_x;
int Green_y;
// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
vex::vision Camera(vex::PORT5);
vex::vision::signature PurpleCube = vex::vision::signature (1, 477, 1579, 1028, 5309, 7479, 6394, 2.7, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 6687, 8353, 7520, -1565, -941, -1254, 2, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -6735, -5105, -5920, -2757, -1339, -2048, 3.3, 0);
int count;
int G_num;
int O_num;
int P_num;
int cube_num;
// define your global instances of motors and other devices here


int main() {
  Camera.setSignature(OrangeCube);
  Camera.setSignature(PurpleCube);
  Camera.setSignature(GreenCube);
    // Print to the screen that the program has started.
    //Brain.Screen.print("Camera values");

    while(1) { 
      
      Camera.takeSnapshot(OrangeCube);
      O_num = Camera.objectCount;
      Camera.takeSnapshot(GreenCube);
      G_num = Camera.objectCount;
      Camera.takeSnapshot(PurpleCube);
      P_num = Camera.objectCount;

      cube_num = G_num + O_num + P_num;

      
      if(count != cube_num){
        Brain.Screen.clearScreen();
      }
      
      Brain.Screen.print(cube_num);
      count = cube_num;
      
        
     
     
    
    }
}
