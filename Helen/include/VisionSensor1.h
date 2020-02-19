/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PurpleCube = vex::vision::signature (1, 297, 901, 600, 6657, 7575, 7116, 3, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 7763, 8545, 8154, -1713, -1165, -1440, 3, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -6969, -5237, -6104, -3427, -1945, -2686, 2.5, 0);
vex::vision::signature RedGoal = vex::vision::signature (4, 5805, 7537, 6672, -691, -201, -446, 5.1, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera1 = vex::vision (vex::PORT12, 24, PurpleCube, OrangeCube, GreenCube, RedGoal, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/