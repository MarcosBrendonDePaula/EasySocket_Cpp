#include <iostream>
#include <string>
#include "cliente.h"
#include <winsock2.h>
using namespace std;

typedef struct dadoss{
	int socket;
	list<Cliente*> *C;
	list<int> *ord;
}dadoss;

class EasyMultServer{
private:
	int servidor;
    struct sockaddr_in enderecoServer;
    WSADATA dados;
public:

	std::list<Cliente*> Clientes;
	std::list<int> OrdemChegada;
	
	list<Cliente*> *GetListClientes(){
		return &this->Clientes;
	}
	/**
	* retorna um ponteiro que contem O cliente Com A id Informada 
	*/
	Cliente* getConID(int i){
		list<Cliente*>::iterator it;
		for(it=Clientes.begin();it!=Clientes.end();it++){
			Cliente *c=*it;
			if(c->id==i){
				return *it;
			}
		}
		return NULL;
	}
	
	EasyMultServer(int porta){
        this->servidor = socket(AF_INET, SOCK_STREAM, 0);
	    memset(&this->enderecoServer,0, sizeof(enderecoServer));
    	this->enderecoServer.sin_family      = AF_INET;
    	this->enderecoServer.sin_port        = htons(porta);
    	this->enderecoServer.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	void start(){
		if (bind(this->servidor, (struct sockaddr *) &this->enderecoServer, sizeof(this->enderecoServer)) == SOCKET_ERROR){
			closesocket(servidor);
			cout << "Erro ao associar a porta" << endl;
		}
		if (listen(servidor, 1) == SOCKET_ERROR){
			WSACleanup();
			closesocket(servidor);
			cout << "falha ao ficar na escuta" << endl;
		}

		dadoss *novo = (dadoss*)malloc(sizeof(dadoss));
		novo->socket = this->servidor;
		novo->C = &this->Clientes;
		novo->ord = &this->OrdemChegada;
		_beginthread(EasyMultServer::proc, 0, novo);
	}

	static void proc(void* arg){
		dadoss *atual=(dadoss*)arg;
		list<Cliente*> *Clt=atual->C;
		int servidor=atual->socket;
		while(true){
			int sockTemporario=0;
	        struct sockaddr_in addrTemporario;
	        int transfer_length = 0;
	        transfer_length = sizeof(addrTemporario);
			
	        cout<<"Aguardando Conexao"<<endl;
	        sockTemporario=accept(servidor,(struct sockaddr *) &sockTemporario,&transfer_length);		
			if(sockTemporario == INVALID_SOCKET){
	            cout<<"Ocorreu Uma tentativa de Conexao E o servidor foi Obrigado a Recusar"<<endl;
			}else{
				Clt->push_back(new Cliente(&sockTemporario,atual->ord));
			}
    	} 
	}
};
