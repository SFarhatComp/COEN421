#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <sys/json.h>
#include "nedclient.h"
#include "cTimer.h"
using namespace std;


int main(int argc, char *argv[])
{
char NED_IP[16]="192.168.141.172";
/********Construct the Ned client object *******/
//cTimer timer(5,0);
NedClient ned(NED_IP);

/**** Calibrate the robot******/
ned.calibrate_auto();


double joints[6] = {0.0,0.0,0.0,0.0,0.0,0.0};


double pose[6]={0.31,-0.006,0.276,0.051,1.121,0.097};
ned.move_pose(pose);


//Safe Pick Initial


/********Define all the poses that Ned2 needs to reach in order to pick and place an object.
Define the poses as arrays of double*******/


/**** Update the grip tool******/

ned.update_tool();

/**** Open the grip******/
ned.release_with_tool();


/****Move the robot to the pick pose passing by the intermediate poses ******/

double PickPose[6]={0.345,-0.024,0.169,0.085,1.220,0.063};
ned.move_pose(PickPose);
ned.grasp_with_tool();





/**** Open the grip******/




double SafePickPose[6]={0.252, -0.083, 0.258, -0.068, 0.924, -0.335};

ned.move_pose(SafePickPose);

/****Move the robot to the place pose passing by the intermediate poses ******/
double safePlacePose[6]= {0.276, -0.207, 0.209, 1.206, 1.476, 0.530};


ned.move_pose(safePlacePose);


ned.release_with_tool();
/**** Close the grip******/


/****Move the robot back to the rest pose passing by the intermediate poses ******/

ned.move_joints(joints);

return EXIT_SUCCESS;
}


