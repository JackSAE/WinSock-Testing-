//Server
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS﻿
#pragma warning(disable:4996)

#include <WinSock2.h>
#include <iostream>

SOCKET Connections[100];
int connectionCounter = 0;

void clientHandlerThread(int index)
{
	char buffer[256];
	while (true)
	{
		recv(Connections[index], buffer, sizeof(buffer), NULL);
		for (int i = 0; i < connectionCounter; ++i)
		{
			if (i == index)
				continue;
			send(Connections[i], buffer, sizeof(buffer), NULL);
		}
	}
}

int main()
{
	//WinSock Setup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) // If WSAStartup returns anything other than 0, Then an error has occured
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //Length of the address (Required for accept call)
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local IP (ONLY BROADCASTING LOCALLY)
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPV4 Socket //Could use IPV6 (AF_INET6)

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create a socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); // Bind the address to the socket
	listen(sListen, SOMAXCONN); // Places sListen socket in a state in which it is listening for an incoming connection!!!!

	SOCKET newConnection; //Socket to hold client's conneciton
	for (int i = 0; i < 100; ++i)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept the connection

		if (newConnection == 0) // If the connection doens't connect
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else // If the conenction does connect
		{
			std::cout << "Client Connected!" << std::endl;
			char MOTD[256] = "YAY IT WORKS"; //Creates the message buffer
			send(newConnection, MOTD, sizeof(MOTD), NULL); //Sends the message 
			Connections[i] = newConnection;
			connectionCounter += 1;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandlerThread, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");
	return 0;
}