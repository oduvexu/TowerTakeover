/*vex-vision-config:begin*/
#include "v5.h"
#include "v5_vcs.h"
vex::vision::signature PURPLE_CUBE = vex::vision::signature (1, 477, 1579, 1028, 5309, 7479, 6394, 3.5, 0);
vex::vision::signature ORANGE_CUBE = vex::vision::signature (2, 6687, 8353, 7520, -1565, -941, -1254, 2.4, 0);
vex::vision::signature GREEN_CUBE = vex::vision::signature (3, -6735, -5105, -5920, -2757, -1339, -2048, 3.3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Camera = vex::vision (vex::PORT5, 56, PURPLE_CUBE, ORANGE_CUBE, GREEN_CUBE, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/