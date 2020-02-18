/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PurpleCube = vex::vision::signature (1, 521, 1123, 822, 4821, 6245, 5534, 3, 0);
vex::vision::signature OrangeCube = vex::vision::signature (2, 7087, 7999, 7542, -1581, -1019, -1300, 3, 0);
vex::vision::signature GreenCube = vex::vision::signature (3, -5801, -5075, -5438, -2487, -1673, -2080, 2.5, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera1 = vex::vision (vex::PORT12, 89, PurpleCube, OrangeCube, GreenCube, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/