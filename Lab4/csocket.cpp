#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include "csocket.h"
using std::cout;
using std::endl;
CSocket::CSocket()
{
backlog = 10; /* set the default number of pending connections queue to 10 */
send_recv_sockfd = -1;
sockfd = -1;
open();
}
CSocket::~CSocket()
{
close_session();
close_sockfd();
}
/* Open TCP socket and save the handler in sockfd. */
int CSocket::open()
{
// ******open a TCP socket and save the handler in sockfd*******

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	return 0;
}
/* Close the session if valid. */
int CSocket::close_session()
{
//******* check that is session socket created or not ****
	if (send_recv_sockfd!=-1){
	close( send_recv_sockfd );
	}
// *******close the session socket if the socket descriptor is valid ******
	send_recv_sockfd=-1;

return 0;
}
/* Close the listen socket. */
int CSocket::close_sockfd()
{

	if (sockfd!=-1){
		close(sockfd );
		}

	sockfd = -1;
return 0;
}
/* Bind socket to a port. */
int CSocket::bind_sock(unsigned short int port)
{
int returnval; // the return value of function
/* host byte order */

addr.sin_family = AF_INET;
/* short, network byte order */
addr.sin_port = htons(port);
addr.sin_addr.s_addr = INADDR_ANY; // local host
/* zero the rest of the struct */
memset(&(addr.sin_zero), 0, 8);
// to make sure it does not block when socket related functions are called
int flags;
flags = fcntl(sockfd, F_GETFL, 0);
if(flags == -1)
{
cout << "Error in fcntl() call." << endl;
return -1;
}
// To make sure the server does not block the other threads to use this port
int option = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

// ******* use addr to bind the socket******

//addr_cli
returnval = bind(sockfd, (sockaddr*)&addr, sizeof(addr) );
return returnval;
}
/* Connect to a server on ip and port specified. */
int CSocket::connect_sock(unsigned short int HostPort, char * HostIP)
{
// the return value of function
int returnval=-1; //ASDNASJDNASONDOASND
/* host byte order */
addr.sin_family = AF_INET;
/* short, network byte order */
addr.sin_port = htons(HostPort);
addr.sin_addr.s_addr = inet_addr(HostIP);
/* zero the rest of the struct */
memset(&(addr.sin_zero), 0, 8);


// ************connect to the server***********

returnval = connect(sockfd, (sockaddr*)&addr, sizeof(addr) );

return returnval;
}
/* Make socket listen. */
int CSocket::listen_sockfd()
{
// the return value of function
int returnval;
returnval =listen( sockfd, backlog );

return returnval;
}
/* Accept the incoming connection. */
int CSocket::accept_connection()
{
// *********accept the connection from client**********

	socklen_t size = sizeof(client_addr);

	send_recv_sockfd = accept( sockfd , (sockaddr*)&client_addr, &size);
return send_recv_sockfd;
}


/* Send data to server from client */
int CSocket::send_msg(char * buff, int len)
{
int returnvalue;

returnvalue = send( sockfd, buff, len , 0 );


// **********send data**********


return returnvalue;
}
/* Send data to client from server. */
int CSocket::send_msg_to_client(char * buff, int len)
{

int returnvalue;


returnvalue = send(send_recv_sockfd, buff, len , 0 );


return returnvalue;
}
/* Receive data from server. */
int CSocket::receive_msg_from_server(char * buff, int len)
{
int returnvalue;
//********** receive data **********

returnvalue = recv( sockfd, buff, len, 0 );
return returnvalue;
}
/* Receive data from client. */
int CSocket::receive_msg(char * buff, int len)
{
int returnvalue;

// **********receive data **********


returnvalue = recv( send_recv_sockfd, buff, len, 0 );
return returnvalue;
}
