#pragma once
#ifdef _WIN32
	#include <winsock2.h>
#elif __linux__
	#include<netinet/in.h>
	#include<pthread.h>
	#include<unistd.h>
#endif
#include <iostream>
#include <list>
#include <vector>
#include <cstring>
#include "Nsock.h"
#include "Events.h"
#include <map>
using namespace std;
class EasyMultServer{
	private:
		int id,limit;
		struct sockaddr_in DadosSocket;
		#ifdef _WIN32
			WSADATA dll;
			SOCKET ServeSock;
		#elif __linux__
			int ServeSock;
			pthread_t ThreadRecepcao;
		#endif
		Events *Evs;
		bool opened;
		bool Bstart();
	public:
		map<int,Nsock*> Connections;
		static list<int> inputOrders;
		#ifdef _WIN32
			static void Recebimento(void *arg);
		#elif __linux__
			static void* Recebimento(void *arg);
		#endif
		EasyMultServer(int porta,Events *Evs);
		bool Start(void(*Processamento)(void*));
		bool Start(void(*Processamento)(void*),int limit);
		bool Start(void(*Processamento)(void*),int limit,void(*acceptFunction)(void*));
		bool Start(void(*Processamento)(void*),int limit,void(*acceptFunction)(void*),void(*Disconnection)(void*));
		Nsock* getId(int i);
		
		bool isOpened();
		void setLimit(int limit);
		int getLimit();
		int getConnectionsSize();
		bool Close();
};