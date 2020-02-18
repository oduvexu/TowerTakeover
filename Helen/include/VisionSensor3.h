/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PurpleCube3 = vex::vision::signature (1, 1147, 1793, 1470, 6443, 7937, 7190, 2.5, 0);
vex::vision::signature OrangeCube3 = vex::vision::signature (2, 6639, 9621, 8130, -1875, -1351, -1612, 2.5, 0);
vex::vision::signature GreenCube3 = vex::vision::signature (3, -7081, -5123, -6102, -3763, -2145, -2954, 2.5, 0);
vex::vision::signature SIG_43 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_53 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_63 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_73 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera3 = vex::vision (vex::PORT14, 50, PurpleCube3, OrangeCube3, GreenCube3, SIG_43, SIG_53, SIG_63, SIG_73);
/*vex-vision-config:end*/