/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PurpleCube = vex::vision::signature (1, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -7069, -5057, -6064, -4451, -3003, -3726, 2.5, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera1 = vex::vision (vex::PORT12, 50, PurpleCube, OrangeCube, GreenCube, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/