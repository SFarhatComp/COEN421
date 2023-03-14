//===========================================================================
// standard needed header files.
#include "nedclient.h"

#include <string.h>
#include <iostream>
#include <sys/json.h>
#include <typeinfo>
#include <stdio.h>
#include "csocket.h"
#include <sstream>

#define NED_PORT 40001

using namespace std;


NedClient::NedClient(char * hostaddress)
{
	strcpy(this->hostaddress, hostaddress);
	thread_id = 0;
	start();

	if(thread_id != 0)
		pthread_join(thread_id, NULL);
}

NedClient::~NedClient()
{
	stop();
}

void * thread_stud(void* arg)
{
	NedClient& ned_client = *(NedClient *) arg;
	ned_client.connect_to_server();
	return 0;
}

/* Starts thread which runs the client. */
void NedClient::start()
{
	/* ***** Create the nedclient thread******* */

	memset(buffer,' ',1024);
	memset(param_buffer,' ',128);
	int rc;
	rc = pthread_create (&thread_id, NULL, &thread_stud, (void*) this);// Might need to change null for this pointer
	if (rc)
		printf("ERROR when creating default thread; Code is %d\n", rc);

}


void NedClient::stop()
{
	// close the socket
	nedSocket.close_session();
	cout << "Disconnected from the server." << endl;
	if(thread_id != 0)
		pthread_join(thread_id, NULL);
	thread_id = 0;
}

void NedClient::print_msg(char msg[], int size){
	/******** Print the content of the char array msg ***********/
	for(int i = 0; i < size; i++) {
		cout << msg[i];
	}
	cout << endl;
}


void NedClient::connect_to_server()
{
	cout << "Port: " << NED_PORT <<  "  ip:  " << hostaddress << endl;
	int flag;
	/******** Connect to Ned2 Server ***********/
	flag = nedSocket.connect_sock(NED_PORT,hostaddress);
	if(flag == -1)
	{
		cout << "Error in Function1() call." << endl;

	}
	else {cout << "Server is connected " << endl; }

}


int NedClient::send_command(char cmd[] ,char param_list[])
{

	/*Converting the command from array of char to string. This facilitate  operations like concatenations*/
	string cmd_string=string(cmd);
	string prm_string=string(param_list);

	/******* Creating the json string to build the packet *********/


	string json1 =  "{\"command\":" + cmd_string + ",\"param_list\":"+prm_string+"}";

	printf(json1.c_str());
	/* *****Convert the json String into array of char json_char *******/


	char json_char[1024];
	strcpy(json_char,json1.c_str());


	/*defining the json array to store the second portion of the message to send to the server*/
	char json_pkt[1024];
	/*creating the command portion of the message using the json library*/
	unsigned short cmd_size = jsnprintf(json_pkt,sizeof(json_pkt),json_char);

	/*****Convert the unsigned short cmd_size into an array of two bytes pkt_size*****/
	char pkt_size[2];
	pkt_size[0]=cmd_size &  0xFF;
	pkt_size[1]=(cmd_size>>8) &  0xFF;

	//Make sure we only get the least significants 8 bits

	/*creating the packet to be sent to the server: the first 2 bytes of the packed encode the packet size (specified as an unsigned short )*/
	char msg[cmd_size+2];
	msg[0]=pkt_size[0];
	msg[1]=pkt_size[1];

	//filling the remaining portion of the message with the command itself
	int i;
	for(i=2; i<cmd_size+2; i++)
	{
		msg[i]=json_pkt[i-2];
	}

	//Printing the message sent to the server

	/****** send the packet to the server ********/


	int msg_size=nedSocket.send_msg(msg,sizeof(msg));

	cout << "Message sent to the server: ";

	print_msg(msg,msg_size);

	/****** Read the answer from the server ********/

	//not sure about length of the answ
	int answ_size = nedSocket.receive_msg_from_server(buffer,sizeof(buffer));

	cout << "Answer received: ";

	print_msg(buffer,answ_size);


	return answ_size;

}


void NedClient::calibrate_auto()
{
	/****** send calibration command ********/
	char cmd[] = "\"CALIBRATE\"";
	char parm[] = "[\"AUTO\"]";

	this->send_command(cmd, parm);
}


double* NedClient::get_joints()
{
	//This function sends the command GET_JOINTS to Ned server and return the parameter list as a vector of double


	/*Send get_joint command and store the content in the variable buffer*/
	/*In order to decode the received answer, we need to extract the useful portion of
message. Remember that the first 2 bytes represent the size of the answer. Therefore, extract
the elements from buffer, copying them into a new variable answ. The useful portion of the
message starts at the index 2 and ends at the index answ-size.*/
	char cmd[] = "\"GET_JOINTS\"";
	char parm[] = "[]";

	int answ_size = send_command(cmd,parm);
	char answ[answ_size-2];

	int i;
	for(i=2;i<answ_size;i++){
		answ[i-2]=buffer[i];
		cout<< answ[i-2];
	}
	cout<<endl;

	/*we need to extract the portion of the answer containing the parameters. Such a string
portion starts with the character '[' and ends with the character "]". The function decode()
performs such an operation.*/
	int param_size = decode(answ);
	/*The answer will be stored in param_buffer array*/
	/********Convert the array of char param_buffer into the array of doubles
joints********/

	char temp[param_size-2] ;


	int j =1;
	while (param_buffer[j] != ']'){

		temp[j-1]=param_buffer[j];

		j++;
	}


	string s = string(temp);
	stringstream ss(s);
	string token;


	double *joints = (double*)malloc(sizeof(double)*6);
	i=0;
	while (getline(ss,token,',')){
		joints[i]=stod(token);
		cout << joints[i]<< " ";
		i++;

	}
	cout << endl;


	return joints;
}




double* NedClient::get_pose()
{
	//This funciton send the GET_POSE command to ned server and return the parameter list as a vector of double


	char cmd[] = "\"GET_POSE\"";
	char parm[] = "[]";

	int answ_size = send_command(cmd,parm);


	/*Send get_pose command and store the content in the variable buffer*/


	/*In order to decode the received answer, we need to extract the useful portion of the message. Remember that the first 2 bytes represent the size of the answer. Therefore, extract the elements from the buffer, copying them into a new variable answ. The useful portion of the message starts at the index 2 and ends at the index answ-size.*/

	char answ[answ_size-2];

	int i;
	for(i=2;i<answ_size;i++){
		answ[i-2]=buffer[i];
	}



	/*we need to extract the portion of the answer containing the parameters. Such a string portion starts with the character '[' and ends with the character "]". The function decode() performs such an operation. */

	int param_size=decode(answ);
	/*The answer will be stored in param_buffer array*/

	/********Convert the array of char param_buffer into the array of doubles  pose********/

	char temp[param_size-2] ;


		int j =1;
		while (param_buffer[j] != ']'){

			temp[j-1]=param_buffer[j];

			j++;
		}


		string s = string(temp);
		stringstream ss(s);
		string token;


	double *pose = (double*)malloc(sizeof(double)*6);
	i=0;
		while (getline(ss,token,',')){
			pose[i]=stod(token);
			cout << pose[i]<< " ";
			i++;

		}
		cout << endl;


	return pose;

}

int NedClient::decode(char message[])
{
	//This function decodes the message and store the useful portion of message containing the parameters into the global variable param_buffer.
	//The parameter portion is stored as an array of char.
	//The function returns the size of the parameters' portion.

	int i=0;

	int start_index;
	int end_index;
	while(1){

		if (message[i]=='['){
			start_index=i;
		}
		if (message[i]==']'){
			end_index=i;
			break;
		}
		i++;
	}

	//Then we copy the desired portion of the message (we know the start and end indexes previously computed)
	for(i=0;i<end_index-start_index+1;i++){
		param_buffer[i]=message[start_index+i];
	}

	return end_index-start_index+1;

}

void NedClient::move_joints(double joint_pos[])
{

//error check
const type_info& type = typeid(joint_pos[0]);
const char* type_name;
type_name= type.name();
if (type_name[0] != 'd' ){
throw std::invalid_argument("The input parameter must be an array of double containing 6 joint positions");
}

/*****Convert the array of double in input into an array of char *****/

string temp2 ="[";
for (int i =0 ; i< 6;i++){

	if (i==5){
		temp2+=(to_string(joint_pos[i]));
	}
	else {temp2+=(to_string(joint_pos[i])+ ", ");}
	 // might need to add a separator
}

temp2+=']';

char param[temp2.length()];

strcpy(param,temp2.c_str());
/* Hint: First, write the array of doubles into a proper string, then convert the string into
an array of chars */
/*Send the move_joints command*/


	char cmd[] = "\"MOVE_JOINTS\"";
	send_command(cmd,param);



}
void NedClient::move_pose(double pose[])
{
//error check
const type_info& type = typeid(pose[0]);
const char* type_name;
type_name= type.name();
if (type_name[0] != 'd' ){
throw std::invalid_argument("The input parameter must be an array of double containing the pose x, y, z, roll, pitch, yaw");
}

/*****Convert the array of double in input into an array of char *****/

/* Hint: First, write the array of doubles into a proper string, then convert the string into
an array of chars */

string temp2 ="[";
for (int i =0 ; i< 6;i++){

	if (i==5){
		temp2+=(to_string(pose[i]));
	}
	else {temp2+=(to_string(pose[i])+ ", ");}
	 // might need to add a separator
}

temp2+=", \"\"";
temp2+=']';

char param[temp2.length()];

strcpy(param,temp2.c_str());
/* Hint: First, write the array of doubles into a proper string, then convert the string into
an array of chars */
/*Send the move_joints command*/


	char cmd[] = "\"MOVE_POSE\"";
	send_command(cmd,param);




/*Send the move_pose command*/
}
void NedClient::update_tool()
{

/****** send update_tool command ********/

	char cmd[] = "\"UPDATE_TOOL\"";
	char parm[] = "[]";
	send_command(cmd,parm);


}
void NedClient::grasp_with_tool()
{
/****** send grasp_with_tool command ********/


	char cmd[] = "\"GRASP_WITH_TOOL\"";
	char parm[] = "[]";
	send_command(cmd,parm);

}
void NedClient::release_with_tool()
{
/****** send release_with_tool command ********/



	char cmd[] = "\"RELEASE_WITH_TOOL\"";
	char parm[] = "[]";
	send_command(cmd,parm);
}




