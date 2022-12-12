#include "MultiClientChat.h"
#include <string>
//Handler for client connections
void MultiClientChat::onClientConnected(int clientSocket)
{
	  //Send a welcome message to the connected client
	  std::string welcomeMsg = "Welcome to the chat server!\r\n";

	  /*send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);*/
	  sendToClient(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1);
}

//Handler for client disconnections
void MultiClientChat::onClientDisconnected(int clientSocket)
{

}

//Handler for message when a message is received from the client 
void MultiClientChat::onMessageReceived(int clientSocket, const char* msg, int length)
{
	broadCastToClients(clientSocket, msg, length); 
}

 