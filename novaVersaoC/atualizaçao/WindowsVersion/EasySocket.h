#pragma once
#include <WinSock2.h>
#include <iostream>
#include <process.h>
#include <list>
#include "Events.h"
using namespace std;
class EasySocket
{
private:
	SOCKET conection;
	sockaddr_in InformacoesConection;
	list<string> Entradas;
	Events Eventos;
public:
	static int bffSiz;
	static void ReceiverDefault(void *arg);
	EasySocket(std::string ip, int porta,void(*funcao)(void *arg));
	string getEntrada();
	int qtEntradas();
	void conectar();
	void Enviar(std::string msg);
	void Desconectar();
	~EasySocket();
};

