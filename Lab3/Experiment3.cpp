#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include "csocket.h"
#include <iostream>
using namespace std;
#define PORT 26376
void * Sender(void* arg){
	CSocket s;
	char buff[100];
	cout<<"This is sender."<<endl;
	char ip_addr[10]="127.0.0.1";
	s.connect_sock(PORT, ip_addr);
	//s.Connect(PORT,"localhost");
	char sender_msg[24]="This is sent by sender.";
	s.send_msg(sender_msg,24);
	s.receive_msg_from_server(buff,100);
	cout<<buff<<endl;
	cout<<"This is sender."<<endl;
	return 0;
}
void * Receiver(void* arg){
	CSocket s;
	char buff[100];
	s.bind_sock(PORT);
	s.listen_sockfd();
	s.accept_connection();
	s.receive_msg(buff,100);
	cout<<buff<<endl;
	char receiver_msg[26]="This is sent by receiver.";
	s.send_msg_to_client(receiver_msg,26);
	// prompt a message
	cout<<"This is receiver."<<endl;
	sleep(1);
	return 0;
}


int main(int argc, char *argv[]) {

	pthread_t sender_ID,receiver_ID;

	int rc_receiver,rc_sender;
	/*****Create sender and receiver threads*****/

	rc_receiver = pthread_create (&receiver_ID, NULL,&Receiver, NULL);
	if (rc_receiver)
		printf("ERROR when creating Receiver thread; Code is %d\n", rc_receiver);


	rc_sender = pthread_create (&sender_ID, NULL,&Sender,NULL);
	if (rc_sender){

		printf("ERROR when creating Sender thread; Code is %d\n", rc_sender);
	}

	pthread_join(receiver_ID, NULL);
	pthread_join(sender_ID, NULL);




	return EXIT_SUCCESS;
}
