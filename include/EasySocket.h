#pragma once
#ifdef _WIN32
	#include <process.h>
	#include <winsock2.h>
#elif __linux__
	#include <pthread.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#define closesocket close
	#define SOCKET int
#endif
#include <iostream>
#include <cstring>
#include <list>
#include "Events.h"
using namespace std;
class EasySocket
{
private:
	SOCKET conection;
	sockaddr_in InformacoesConection;
	list<string> Entradas;
	list<char*> EntradasVector;
	Events *Evs;
	char vazio[6];
	bool closed;
public:
	int Buffersize;

	#ifdef _WIN32
		WSADATA dll;
		static void ReceiverDefault(void *arg);
	#elif __linux__
		pthread_t ThreadCliente;
		static void* ReceiverDefault(void *arg);
	#endif
	
	EasySocket(std::string ip, int porta,void(*funcao)(void *arg),Events *evs);
	string getInput();
	char* getInputVector();
	
	sockaddr_in* getSockAddr();
	list<string>* getInputs();
	int Connect();
	
	int SendMsg(std::string msg);
	int SendMsg(char msg[]);

	bool isClosed();
	void Disconnect();
	~EasySocket();
};