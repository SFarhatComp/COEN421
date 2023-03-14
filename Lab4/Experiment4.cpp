#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <sys/json.h>

#include "ctimer.h"
#include "nedclient.h"

using namespace std;
int main(int argc, char *argv[]) 
{

	char NED_IP[16]="192.168.141.172";
	cTimer timer(5,0);
	NedClient ned(NED_IP);
	ned.calibrate_auto();


	/*********Calibrate the robot and display the joints position and the pose***********/

	for (int i =0 ; i<10 ; i++){


		ned.get_joints();
		ned.get_pose();
		timer.waitTimer();






	}



}
