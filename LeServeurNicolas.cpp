// LeServeurNicolas.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>
#include "partie.h"

using namespace std;

int StartWs()
{
	WSAData wsa;
	return WSAStartup(MAKEWORD(2, 0), &wsa);
}

#define MAX 2

int fullClientsArray(SOCKET* sarr)
{

	int compteur = 0;
	for (int i = 0; i != MAX; ++i)
	{
		if (sarr[i] != INVALID_SOCKET)
			++compteur;
	}

	return compteur;
}


int main()
{
	SOCKET sConnection;
	SOCKET clients[MAX];
	FD_SET fdSet;
	SOCKADDR_IN sinType;


	Partie laPartie;
	bool debutPartie = false;
	char buf[4096];
	bool tourJoueur1 = false;

	int codeRetour = StartWs();
	if (codeRetour != 0)
	{
		return 1;
	}

	sConnection = socket(AF_INET, SOCK_STREAM, 0);
	if (sConnection == INVALID_SOCKET)
		return 1;

	for (int i = 0; i != MAX; ++i)
		clients[i] = INVALID_SOCKET;


	memset(&sinType, 0, sizeof(sinType));
	sinType.sin_addr.s_addr = INADDR_ANY;
	sinType.sin_family = AF_INET;
	sinType.sin_port = htons(12345);

	codeRetour = bind(sConnection, (SOCKADDR*)&sinType, sizeof(sinType));
	if (codeRetour == SOCKET_ERROR)
		return 1;

	codeRetour = listen(sConnection, MAX);
	if (codeRetour == SOCKET_ERROR)
		return 1;

	while (fullClientsArray(clients) != MAX)
	{
		FD_ZERO(&fdSet);
		FD_SET(sConnection, &fdSet);

		codeRetour = select(0, &fdSet, nullptr, nullptr, nullptr);
		if (FD_ISSET(sConnection, &fdSet))
			for (int i = 0; i != MAX; ++i)
				if (clients[i] == INVALID_SOCKET)
					clients[i] = accept(sConnection, nullptr, nullptr);
	}



	while (1)
	{

		if (debutPartie == false)
		{
			string reponse = "La partie va commencer joueur 1";
			string reponse2 = "la partie va commencer joueur 2";

			send(clients[0], reponse.c_str(), reponse.size(), 0);

			send(clients[1], reponse2.c_str(), reponse2.size(), 0);
		}

		ZeroMemory(buf, 4096);
		recv(clients[0], buf, 4096, 0);

		string symbole = "cr";

		if (buf[0] == 's' && buf[1] == 'y' && debutPartie == false)
		{
			send(clients[0], symbole.c_str(), symbole.size(), 0);
		}



		ZeroMemory(buf, 4096);
		recv(clients[0], buf, 4096, 0);
		string premierTour = "o";
		string secondTour = "n";

		if (buf[0] == 'f' && buf[1] == 'i' && debutPartie == false)
		{
			int tirage = laPartie.tirage();

			switch (tirage)
			{
			case 0 :
				send(clients[0], premierTour.c_str(), premierTour.size(), 0);
				tourJoueur1 = true;
				break;
			case 1 :
				send(clients[0], secondTour.c_str(), secondTour.size(), 0);
				break;
			}
			
			debutPartie = true;
			
		}

		
		string CaseValide = "o";
		string CaseNonValide = "n";

		if (tourJoueur1 == true && debutPartie == true)
		{
			ZeroMemory(buf, 4096);
			recv(clients[0], buf, 4096, 0);
			

			if (buf[0] == 's' && buf[1] == 'c')
			{
				if (laPartie.EstCordonneValide((int)buf[2], (int)buf[3]))
				{
					send(clients[0], CaseValide.c_str(), CaseValide.size(), 0);
				}
				else
				{
					send(clients[0], CaseNonValide.c_str(), CaseNonValide.size(), 0);
				}
			}
		}
		else
		{
			ZeroMemory(buf, 4096);
			recv(clients[1], buf, 4096, 0);
		
			if (buf[0] == 's' && buf[1] == 'c')
			{
				if (laPartie.EstCordonneValide((int)buf[2], (int)buf[3]))
				{
					send(clients[1], CaseValide.c_str(), CaseValide.size(), 0);
				}
				else
				{
					send(clients[1], CaseNonValide.c_str(), CaseNonValide.size(), 0);
				}
			}
		}
		
		

		
	}
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
