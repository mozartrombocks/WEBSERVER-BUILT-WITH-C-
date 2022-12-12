#pragma once

#include <ws2tcpip.h>

//Need to link to ws2_32.lib
#pragma comment (lib, "ws2_32.lib")
class TcpListener
{
public:

	TcpListener(const char* ipAddress, int port) : m_ipAddress(ipAddress), m_port(port) { } 

	//Initialize the listener
	int init();

	//Run the listener
	int run();

protected:

	//Handler for client connections
	virtual void onClientConnected(int clientSocket); 

	//Handler for client disconnections
	virtual void onClientDisconnected(int clientSocket); 

	//Handler for message when a message is received from the client 
	virtual void onMessageReceived(int clientSocket, const char* msg, int length); 

	//Send a message to a client
	void sendToClient(int clientSocket, const char* msg, int length); 

	//Broadcast a message from a client
	void broadCastToClients(int sendingClients, const char* msg, int length); 



private:

	const char*		m_ipAddress; // IP Address server will run on
	int				m_port;		// Port # for the web server
	int				m_socket;	// Internal file descriptor for the listening socket
	fd_set			m_master;     // A master file descriptor set
};

