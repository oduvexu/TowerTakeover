/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PurpleCube = vex::vision::signature (1, 763, 2123, 1442, 6129, 8971, 7550, 2.7, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 7763, 8545, 8154, -1713, -1165, -1440, 3, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -6969, -5237, -6104, -3427, -1945, -2686, 2.5, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera1 = vex::vision (vex::PORT12, 93, PurpleCube, OrangeCube, GreenCube, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/