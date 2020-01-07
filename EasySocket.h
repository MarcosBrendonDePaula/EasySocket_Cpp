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
	Events *Evs;
	bool closed;
public:
	static int bffSiz;
	#ifdef _WIN32
		static void ReceiverDefault(void *arg);
	#elif __linux__
		pthread_t ThreadCliente;
		static void* ReceiverDefault(void *arg);
	#endif
	
	EasySocket(std::string ip, int porta,void(*funcao)(void *arg),Events *evs);
	string getEntrada();
	int qtEntradas();
	int conectar();
	int Enviar(std::string msg);
	bool isClosed();
	void Desconectar();
	~EasySocket();
};