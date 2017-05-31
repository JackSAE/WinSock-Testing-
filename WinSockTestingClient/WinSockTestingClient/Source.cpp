//Client
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS﻿
#pragma warning(disable:4996)

#include <WinSock2.h>
#include <iostream>

SOCKET Connection;

void ClientThread()
{
	char buffer[256];
	while (true)
	{
		recv(Connection, buffer, sizeof(buffer), NULL);
		std::cout << "Message Recived: " << buffer << std::endl;
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local IP 
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPV4 Socket //Could use IPV6 (AF_INET6)


	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set up connection socket
	if (connect(Connection, (SOCKADDR*)&addr, addrlen) != 0) //If connection fails
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	else
	{
		std::cout << "Connected!" << std::endl;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

		char buffer[256];
		while (true)
		{
			std::cin.getline(buffer, sizeof(buffer));
			send(Connection, buffer, sizeof(buffer), NULL);
		}
	}
	system("Pause");

	return 0;
}