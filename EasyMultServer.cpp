#include "EasyMultServer.h"
//static membros
list<int> EasyMultServer::OrdemDeChegada;

void EasyMultServer::Recebimento(void *arg){
		EasyMultServer* This=(EasyMultServer*)arg;
		This->id=1;
		while(true){
			nsock* atual=new nsock(&EasyMultServer::OrdemDeChegada,&This->Conexoes);
			SOCKET *cliente=atual->getCliente();
			struct sockaddr_in* DadosCliente=atual->getDadosCliente();
			int tam=sizeof(*DadosCliente);
			*cliente=accept(This->ServeSock,(struct sockaddr*)DadosCliente,&tam);
			if(*cliente>0){
				cout<<"Conexao Aceita"<<endl;
				atual->id=This->id;
				This->Conexoes.push_back(atual);
				atual->start();
				This->id++;
			}else{
				free(atual);
				cout<<"Conexao recusada ou com erro"<<endl;
			}
		}
}

nsock* EasyMultServer::getConID(int i)
{
		vector<nsock*>::iterator it;
		for(it=this->Conexoes.begin();it!=this->Conexoes.end();it++){
			nsock *c=*it;
			if(c->id==i){
				return *it;
			}
		}
		return NULL;
}

EasyMultServer::EasyMultServer(int porta){
	WSAStartup(MAKEWORD(2,2), &this->dll);
    this->ServeSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&this->DadosSocket,0, sizeof(this->DadosSocket));
    this->DadosSocket.sin_family = AF_INET;
    this->DadosSocket.sin_port = htons(porta);
    this->DadosSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	
}
void EasyMultServer::StartServer(void(*Processamento)(void*)){
	Events::static_Acess->addEvent(new Event(1,Processamento,NULL));
	if (bind(this->ServeSock, (struct sockaddr *) &this->DadosSocket, sizeof(this->DadosSocket)) == SOCKET_ERROR){
		closesocket(this->ServeSock);
		cout << "Erro ao associar a porta" << endl;
	}
	if (listen(this->ServeSock, 500) == SOCKET_ERROR){
		cout << "falha ao ficar na escuta" << endl;
	}
	_beginthread(EasyMultServer::Recebimento,0,this);
	//pthread_create(&this->Process,NULL,EasyMultServer::Recebimento,this);
}