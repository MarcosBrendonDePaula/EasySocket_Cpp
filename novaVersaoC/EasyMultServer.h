#include <winsock2.h>
#include <iostream>
#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include "nsock.h"
//#include "Eventsmain.h"
using namespace std;
class EasyMultServer{
	private:
		int id;
		SOCKET ServeSock;
		struct sockaddr_in DadosSocket;
		pthread_t Process;
		WSADATA dll;
	public:
		vector<nsock*> Conexoes;
		static list<int> OrdemDeChegada;
		static void *Recebimento(void *arg);
		EasyMultServer(int porta);
		void StartServer(void(*Processamento)());
		nsock* getConID(int i);
};