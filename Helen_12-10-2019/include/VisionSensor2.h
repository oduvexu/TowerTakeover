/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PurpleCube2 = vex::vision::signature (1, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature OrangeCube2 = vex::vision::signature (2, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature GreenCube2 = vex::vision::signature (3, -6969, -5237, -6104, -3427, -1945, -2686, 2.5, 0);
vex::vision::signature SIG_42 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_52 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_62 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_72 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera2 = vex::vision (vex::PORT13, 50, PurpleCube2, OrangeCube2, GreenCube2, SIG_42, SIG_52, SIG_62, SIG_72);
/*vex-vision-config:end*/