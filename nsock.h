#include <winsock2.h>
#include <process.h>
#include <iostream>
#include <list>
#include <vector>
#include <cstring>
#include "Events.h"

using namespace std;
class nsock{
	private:
		SOCKET cliente;
		struct sockaddr_in DadosCliente;
		list<string> Entrada;
		list<int> *ordem;
		vector<nsock*> *conexoes;
		//void(*ProcessFunction)(void*);
	public:
		int id;
		static Events evs;
		static void ReceiveFunction(void *arg);
		nsock(list<int> *lista,vector<nsock*> *cn);
		SOCKET* getCliente();
		struct sockaddr_in* getDadosCliente();
		void start();
		string getEntrada();
		void Enviar(string msg);
};