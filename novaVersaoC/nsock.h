#include <winsock2.h>
#include <iostream>
#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include "Eventsmain.h"
using namespace std;
class nsock{
	private:
		SOCKET cliente;
		struct sockaddr_in DadosCliente;
		pthread_t Process;
		list<string> Entrada;
		list<int> *ordem;
		vector<nsock*> *conexoes;
		//void(*ProcessFunction)(void*);
	public:
		int id; 
		static void *ReceiveFunction(void *arg);
		nsock(list<int> *lista,vector<nsock*> *cn);
		SOCKET* getCliente();
		struct sockaddr_in* getDadosCliente();
		void start();
		string getEntrada();
		void Enviar(string msg);
};