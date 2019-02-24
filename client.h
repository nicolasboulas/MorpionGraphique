#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class client
{
	private :
	string ipAddress = "192.168.1.15";
	int port = 12345;
	WSADATA data;
	WORD ver;
	int wsResult;

	SOCKET sock;

	sockaddr_in hint;

	int connResult;

	char buf[4096];

	int coordonnees[2];

public : 
	client();
	bool Premier();
	bool envoyerCase(string  x, string y);
	bool recevoirCase();
	string CroixRond();
};
