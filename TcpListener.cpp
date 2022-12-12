#include "TcpListener.h"
#include <iostream>
#include <string>
#include <sstream>

int TcpListener::init()
{
	// Initialize winsock 
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		return wsOk;
	}

	// Create a socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		return WSAGetLastError();
	}

	// Bind the socket to an ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress, &hint.sin_addr);

	if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	// Tell Winsock the socket is for listening
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) 
	{
		return WSAGetLastError(); 
	}

	FD_ZERO(&m_master);

	FD_SET(m_socket, &m_master);

	return 0;
}

int TcpListener::run()
{
	while (true)
	{
		fd_set copy = m_master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == m_socket)
			{
				// Accept a new connection
				SOCKET client = accept(m_socket, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &m_master);

				//TODO : client_connected
				onClientConnected(client); 

			}

			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0) 
				{
					//Drop the client
					onClientDisconnected(sock); 
					closesocket(sock);
					FD_CLR(sock, &m_master);

				}
				else {

					onMessageReceived(sock, buf, bytesIn); 
					/*for (int i = 0; i < m_master.fd_count; i++)
					{
						SOCKET outSocket = m_master.fd_array[i]; 
						if (outSocket != m_socket && outSocket != sock)
						{

						}
					 }*/
					 

				}
			}
		}
	}

	FD_CLR(m_socket, &m_master);
	closesocket(m_socket); 

	while (m_master.fd_count > 0)
	{
		SOCKET sock = m_master.fd_array[0];

		FD_CLR(sock, &m_master); 
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
}

//Send a message to a client
void TcpListener::sendToClient(int clientSocket, const char* msg, int length)
{
	send(clientSocket, msg, length, 0); 
}

//Broadcast a message from a client
void TcpListener::broadCastToClients(int sendingClient, const char* msg, int length)
{
	for (int i = 0; i < m_master.fd_count; i++)
	{
		SOCKET outSock = m_master.fd_array[i]; 
		if (outSock != m_socket && outSock != sendingClient)
		{
			sendToClient(outSock, msg, length);
		}
	}
}

//Handler for client connections
void TcpListener::onClientConnected(int clientSocket)
{

}

//Handler for client disconnections
void TcpListener::onClientDisconnected(int clientSocket)
{

}

//Handler for message when a message is received from the client 
void TcpListener::onMessageReceived(int clientSocket, const char* msg, int length)
{

}

