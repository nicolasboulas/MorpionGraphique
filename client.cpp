#include "pch.h"
#include "client.h"


client::client()
{
	ver = MAKEWORD(2, 3);
	wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cout << "can't start" << wsResult << endl;
	}


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cout << "can't create" << WSAGetLastError() << endl;
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cout << "Can't Connect" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
	}

}

bool client::envoyerCase(string x, string y)
{
	string lbf = "sc" + x + y;

	send(sock, lbf.c_str(), 4096, 0);
	

	ZeroMemory(buf, 4096);
	int rec = recv(sock, buf, lbf.size(), 0);

	std::cout << buf << endl;

	


	return buf[0] == 'o';
}


bool client::recevoirCase()
{
	ZeroMemory(buf, 4096);
	recv(sock, buf, 4096, 0);
	coordonnees[0] = buf[2];
	coordonnees[1] = buf[3];
	return buf[4] != 'f';
}

string client::CroixRond()
{
	string reponse;
	string question = "symbole";
	send(sock, question.c_str(), question.size() ,0);
	recv(sock, buf, 4096, 0);
	reponse = buf[0];
	reponse = +buf[1];
	return reponse;
}

bool client::Premier()
{
	string premier = "first";
	send(sock, premier.c_str(), premier.size(), 0);

	ZeroMemory(buf, 4096);
	recv(sock, buf, 4096, 0);
	return buf[0] == 'o';
}