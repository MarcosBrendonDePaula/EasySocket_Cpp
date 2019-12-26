#include "EasyMultServer.h"

list<int> EasyMultServer::OrdemDeChegada;
#ifdef _WIN32
	void EasyMultServer::Recebimento(void *arg){
		EasyMultServer* This=(EasyMultServer*)arg;
		This->id=1;
		while(This->opened){
			nsock* atual=new nsock(&EasyMultServer::OrdemDeChegada,&This->Conections);
			#ifdef _WIN32
				SOCKET *cliente=atual->getCliente();
			#elif __linux__
				int *cliente=atual->getCliente();
			#endif
			struct sockaddr_in* DadosCliente=atual->getDadosCliente();
			int tam=sizeof(*DadosCliente);
			*cliente=accept(This->ServeSock,(struct sockaddr*)DadosCliente,&tam);
			if((*cliente>0)*(This->limit>This->Conections.size())*This->opened){
				cout<<"Conexao Aceita"<<endl;
				if(Events::static_Acess->getEvent(2)){
					Events::static_Acess->getEvent(2)->parametros = atual;
					Events::static_Acess->sendSignal(2);
				}
				atual->id=This->id;
				This->Conections[This->id]=atual;
				atual->start();
				This->id++;
			}else{
				atual->Close();
				free(atual);
			}
		}
		return;
	}
#elif __linux__
	#define closesocket close
	void* EasyMultServer::Recebimento(void *arg){
		EasyMultServer* This=(EasyMultServer*)arg;
		This->id=1;
		while(This->opened){
			nsock* atual=new nsock(&EasyMultServer::OrdemDeChegada,&This->Conections);
			#ifdef _WIN32
				SOCKET *cliente=atual->getCliente();
			#elif __linux__
				int *cliente=atual->getCliente();
			#endif
			struct sockaddr_in* DadosCliente=atual->getDadosCliente();
			socklen_t tam=sizeof(*DadosCliente);
			*cliente=accept(This->ServeSock,(struct sockaddr*)DadosCliente,&tam);
			if((*cliente>0)*(This->limit>This->Conections.size())*This->opened){
				cout<<"Conexao Aceita"<<endl;
				if(Events::static_Acess->getEvent(2)){
					Events::static_Acess->getEvent(2)->parametros = atual;
					Events::static_Acess->sendSignal(2);
				}
				atual->id=This->id;
				This->Conections[This->id]=atual;
				atual->start();
				This->id++;
			}else{
				atual->Close();
				free(atual);
			}
		}
	}
#endif

nsock* EasyMultServer::getConID(int i)
{
	return this->Conections[i];
}

EasyMultServer::EasyMultServer(int porta){
	#ifdef _WIN32
		WSAStartup(MAKEWORD(2,2), &this->dll);
    #endif
	this->ServeSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&this->DadosSocket,0, sizeof(this->DadosSocket));
    this->DadosSocket.sin_family = AF_INET;
    this->DadosSocket.sin_port = htons(porta);
    this->DadosSocket.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool EasyMultServer::StartServer(void(*Processamento)(void*)){
	this->limit=999999;
	Events::static_Acess->addEvent(new Event(1,Processamento,NULL));
	if (bind(this->ServeSock, (struct sockaddr *) &this->DadosSocket, sizeof(this->DadosSocket)) < 0){
		this->opened=false;
		closesocket(this->ServeSock);
		return false;
	}
	if (listen(this->ServeSock, 500) < 0){
		this->opened=false;
		return false;
	}
	this->opened=true;
	#ifdef _WIN32
		_beginthread(EasyMultServer::Recebimento,0,this);
	#elif __linux__
		pthread_create(&this->ThreadRecepcao,NULL,EasyMultServer::Recebimento,this);
	#endif
	return true;
}

bool EasyMultServer::StartServer(void(*Processamento)(void*),int limit){
	this->limit=limit;
	Events::static_Acess->addEvent(new Event(1,Processamento,NULL));
	if (bind(this->ServeSock, (struct sockaddr *) &this->DadosSocket, sizeof(this->DadosSocket)) < 0){
		closesocket(this->ServeSock);
		this->opened=false;
		return false;
	}
	if (listen(this->ServeSock, this->limit) < 0){
		closesocket(this->ServeSock);
		this->opened=false;
		return false;
	}
	this->opened=true;
	#ifdef _WIN32
		_beginthread(EasyMultServer::Recebimento,0,this);
	#elif __linux__
		pthread_create(&this->ThreadRecepcao,NULL,EasyMultServer::Recebimento,this);
	#endif
	return true;
}
bool EasyMultServer::StartServer(void(*Processamento)(void*),int limit,void(*acceptFunction)(void*)){
	this->limit=limit;
	Events::static_Acess->addEvent(new Event(2,acceptFunction,NULL));
	Events::static_Acess->addEvent(new Event(1,Processamento,NULL));
	if (bind(this->ServeSock, (struct sockaddr *) &this->DadosSocket, sizeof(this->DadosSocket)) < 0){
		closesocket(this->ServeSock);
		this->opened=false;
		return false;
	}
	if (listen(this->ServeSock, this->limit) < 0){
		closesocket(this->ServeSock);
		this->opened=false;
		return false;
	}
	this->opened=true;
	#ifdef _WIN32
		_beginthread(EasyMultServer::Recebimento,0,this);
	#elif __linux__
		pthread_create(&this->ThreadRecepcao,NULL,EasyMultServer::Recebimento,this);
	#endif
	return true;
}
bool EasyMultServer::isOpened(){
	return this->opened;
}

bool EasyMultServer::CloseServer(){
	this->opened=false;
	return true;
}

void EasyMultServer::setLimit(int limit){
	this->limit=limit;
}
int EasyMultServer::getLimit(){
	return this->limit;
}