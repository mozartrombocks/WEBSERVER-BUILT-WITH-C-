#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include "WebServer.h"
#include <fstream>
#include <vector>

//Handler for client connections
void WebServer::onClientConnected(int clientSocket)
{

}

//Handler for client disconnections
void WebServer::onClientDisconnected(int clientSocket)
{

}

//Handler for message when a message is received from the client 
void WebServer::onMessageReceived(int clientSocket, const char* msg, int length)
{
	// GET /index.html HTTP/1.1

	// Parse out the document requested
	std::istringstream iss(msg); 
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>()); 

	// Open the document in the local file system
	std::string content = "<h1>Error:404 - Not found</h1>";
	std::string htmlFile = "/index.html"; 
	int errorCode = 404;

	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		htmlFile = parsed[1];
		if (htmlFile == "/")
		{
			htmlFile = "/index.html"; 
		}
	}

		// Open the document in the local file system
		std::ifstream f(".\\wwwroot" + htmlFile);

		if (f.good())
		{
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			content = str;
			errorCode = 200;

			//std::string output = oss.str(); 
			//size = output.size() + 1; 

		}
		f.close();
	

	// Write the document back to the 
	std::ostringstream oss;
	oss << "HTTP/1.1 "<< errorCode << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << content.size() << "\r\n"; 
	oss << "\r\n";
	oss << content;

	std::string output = oss.str(); 
	int size = output.size() + 1; 

	sendToClient(clientSocket, output.c_str(), size);
}

