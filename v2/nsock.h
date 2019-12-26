#pragma once
#ifdef _WIN32
	#include <winsock2.h>
	#include <process.h>
#elif __linux__
	#include<netinet/in.h>
	#include<pthread.h>
	#include<unistd.h>
#endif
#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>
#include <cstring>
#include "Events.h"
#include <map>

using namespace std;
class nsock{
	private:
		#ifdef _WIN32
			SOCKET cliente;
		#elif __linux__
			int cliente;
			pthread_t ThreadCliente;
		#endif
		struct sockaddr_in DadosCliente;
		list<string> Entrada;
		list<int> *ordem;
		//vector<nsock*> *conexoes;
		map<int,nsock*> *connections;
		Events *Evs;
		int erro;
	public:
		int id;
		nsock(list<int> *lista,map<int,nsock*> *cn,Events *Evs);
		#ifdef _WIN32
			static void ReceiveFunction(void *arg);
			SOCKET* getCliente();
		#elif __linux__
			static void* ReceiveFunction(void *arg);
			int* getCliente();
		#endif
		struct sockaddr_in* getDadosCliente();
		int start();
		void Close();
		string getEntrada();
		int Enviar(string msg);
};