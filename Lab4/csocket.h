/*
 * csocket.h
 *
 *  Created on: Feb. 7, 2023
 *      Author: coen421
 */

#ifndef CSOCKET_H_
#define CSOCKET_H_

// header files needed for socket and TCP programming
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//======================================================================


class CSocket{


private:
int sockfd; // socket descriptor
int send_recv_sockfd; // socket descriptor of accepted connection
struct sockaddr_in addr;
struct sockaddr_in client_addr;
public:
unsigned short int port;
int backlog; /* how many pending connections queue will hold */
CSocket();
~CSocket();
int connect_sock(unsigned short int HostPort, char * HostIP);
int bind_sock(unsigned short int port);
int listen_sockfd();
int accept_connection();
int send_msg(char * buff, int len);
int send_msg_to_client(char * buff, int len);
int receive_msg(char * buff, int len);
int receive_msg_from_server(char * buff, int len);
int close_sockfd();
int open();
int close_session();
};




#endif /* CSOCKET_H_ */
