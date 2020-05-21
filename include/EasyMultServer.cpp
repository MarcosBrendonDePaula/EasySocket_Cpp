#include "EasyMultServer.h"
list<int> EasyMultServer::inputOrders;
#ifdef _WIN32
	void EasyMultServer::Recebimento(void *arg){
		EasyMultServer* This=(EasyMultServer*)arg;
		This->id=1;
		while(This->opened){
			Nsock* atual=new Nsock(&EasyMultServer::inputOrders,&This->Connections,This->Evs);
			#ifdef _WIN32
				SOCKET *cliente=atual->getClient();
			#elif __linux__
				int *cliente=atual->getClient();
			#endif
			struct sockaddr_in* DadosCliente=atual->getSocketInfo();
			int tam=sizeof(*DadosCliente);
			*cliente=accept(This->ServeSock,(struct sockaddr*)DadosCliente,&tam);
			if((*cliente>0)*(This->limit>This->Connections.size())*This->opened){
				if(This->Evs->getEvent(2)){
					This->Evs->getEvent(2)->parametros = atual;
					This->Evs->sendSignal(2);
				}
				atual->id=This->id;
				This->Connections[This->id]=atual;
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
			Nsock* atual=new Nsock(&EasyMultServer::inputOrders,&This->Connections,This->Evs);
			#ifdef _WIN32
				SOCKET *cliente=atual->getClient();
			#elif __linux__
				int *cliente=atual->getClient();
			#endif
			struct sockaddr_in* DadosCliente=atual->getSocketInfo();
			socklen_t tam=sizeof(*DadosCliente);
			*cliente=accept(This->ServeSock,(struct sockaddr*)DadosCliente,&tam);
			if((*cliente>0)*(This->limit>This->Connections.size())*This->opened){
				if(This->Evs->getEvent(2)!=NULL){
					This->Evs->getEvent(2)->parametros = atual;
					This->Evs->sendSignal(2);
				}
				atual->id=This->id;
				This->Connections[This->id]=atual;
				atual->start();
				This->id++;
			}else{
				atual->Close();
				free(atual);
			}
		}
		return NULL;
	}
#endif

Nsock* EasyMultServer::getId(int i)
{
	return this->Connections[i];
}

EasyMultServer::EasyMultServer(int porta,Events *Evs){
	#ifdef _WIN32
		WSAStartup(MAKEWORD(2,2), &this->dll);
    #endif
	this->Evs=Evs;
	this->ServeSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&this->DadosSocket,0, sizeof(this->DadosSocket));
    this->DadosSocket.sin_family = AF_INET;
    this->DadosSocket.sin_port = htons(porta);
    this->DadosSocket.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool EasyMultServer::Start(void(*Processamento)(void*)){
	this->limit=999999;
	this->Evs->addEvent(new Event(1,Processamento,NULL));
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

bool EasyMultServer::Start(void(*Processamento)(void*),int limit){
	this->limit=limit;
	this->Evs->addEvent(new Event(1,Processamento,NULL));
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
bool EasyMultServer::Start(void(*Processamento)(void*),int limit,void(*acceptFunction)(void*)){
	this->limit=limit;
	this->Evs->addEvent(new Event(2,acceptFunction,NULL));
	this->Evs->addEvent(new Event(1,Processamento,NULL));
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

bool EasyMultServer::Start(void(*Processamento)(void*),int limit,void(*acceptFunction)(void*),void(*Disconnection)(void*)){
	this->limit=limit;
	this->Evs->addEvent(new Event(2,acceptFunction,NULL));
	this->Evs->addEvent(new Event(1,Processamento,NULL));
	this->Evs->addEvent(new Event(3,Disconnection,NULL));
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

bool EasyMultServer::Close(){
	this->opened=false;
	return true;
}

void EasyMultServer::setLimit(int limit){
	this->limit=limit;
}
int EasyMultServer::getLimit(){
	return this->limit;
}