/*vex-vision-config:begin*/
#include "v5.h"
#include "v5_vcs.h"
vex::vision::signature PURPLE_CUBE = vex::vision::signature (1, 477, 1579, 1028, 5309, 7479, 6394, 3.9, 0);
vex::vision::signature ORANGE_CUBE = vex::vision::signature (2, 6687, 8353, 7520, -1565, -941, -1254, 3.5, 0);
vex::vision::signature GREEN_CUBE = vex::vision::signature (3, -6945, -4503, -5724, -3053, -1483, -2268, 3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera = vex::vision (vex::PORT5, 63, PURPLE_CUBE, ORANGE_CUBE, GREEN_CUBE, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/