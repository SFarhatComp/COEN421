#ifndef CQSTREAMCLIENT_H_
#define CQSTREAMCLIENT_H_

#include <pthread.h>
#include "csocket.h"
#include <string.h>

class NedClient
{
private:
	pthread_t thread_id;	// Handle of running thread. 

	char buffer[1024];
	char param_buffer[128];

	bool is_data_copied2buff, is_data_sent;
	friend void * thread_stud(void* arg);


	void print_msg(char msg[],int size);
	void connect_to_server();
	int send_command(char* command, char* param_list);
	void start();
	void stop();
	int decode(char json_msg[]);

public:
	char hostaddress[200]; //Ned2 IP address
	CSocket nedSocket;     //Ned2 Socket

	NedClient(char * hostaddress);
	~NedClient();

	void client();
	void calibrate_auto();
	double* get_joints();
	double* get_pose();
	void move_joints(double joint_pos[]);
	void move_pose(double pose[]);
	void update_tool();
	void grasp_with_tool();
	void release_with_tool();


};

#endif /*NedClient_H_*/
