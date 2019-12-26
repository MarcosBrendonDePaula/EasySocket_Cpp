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
#include "nsock.h"
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
	public:
		map<int,nsock*> Conections;
		static list<int> OrdemDeChegada;
		#ifdef _WIN32
			static void Recebimento(void *arg);
		#elif __linux__
			static void* Recebimento(void *arg);
		#endif
		EasyMultServer(int porta,Events *Evs);
		bool StartServer(void(*Processamento)(void*));
		bool StartServer(void(*Processamento)(void*),int limit);
		bool StartServer(void(*Processamento)(void*),int limit,void(*acceptFunction)(void*));
		nsock* getConID(int i);
		
		bool isOpened();
		void setLimit(int limit);
		int getLimit();
		int getConectionsSize();
		bool CloseServer();
};