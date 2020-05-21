#include "Nsock.h"
#ifdef _WIN32
	void Nsock::ReceiveFunction(void *arg)
#elif __linux__
	#define SOCKET int
	#define closesocket close
	void* Nsock::ReceiveFunction(void *arg)
#endif
{
	Nsock* This=(Nsock*)arg;
	char buffer[This->BufferSize];
	memset(buffer,0x0,sizeof(buffer));
	while((recv(This->cliente,buffer,sizeof(buffer),0)>0)){
		string entrada(buffer);
		This->Entrada.push_back(entrada);
		This->Evs->getEvent(1)->parametros=This;
		This->Evs->sendSignal(1);
		memset(buffer,0x0,sizeof(buffer));
	}

	if(This->Evs->getEvent(3)){
		This->Evs->getEvent(3)->parametros = This;
		This->Evs->sendSignal(3);
	}
	This->connections->erase(This->id);
#ifdef _WIN32
	return;
#elif __linux__
	return NULL;
#endif

}

Nsock::Nsock(list<int> *lista,map<int,Nsock*> *cn,Events *Evs)
{
	strcpy(this->vazio,"empty");
	this->BufferSize = 1024;
	this->Evs = Evs;
	this->ordem=lista;
	this->connections=cn;
	memset(&this->DadosCliente,0x0,sizeof(this->DadosCliente));
}


SOCKET* Nsock::getClient()
{
	return &this->cliente;
}


struct sockaddr_in* Nsock::getSocketInfo()
{
	return &this->DadosCliente;
}

int Nsock::start()
{
	#ifdef _WIN32
		_beginthread(Nsock::ReceiveFunction,0,this);
	#elif __linux__
		pthread_create(&this->ThreadCliente,NULL,Nsock::ReceiveFunction,this);
	#endif
	return 1;
}


const string Nsock::getIP()
{
	string ip(inet_ntoa(this->DadosCliente.sin_addr));
	return ip;
}


const int Nsock::getPort()
{
	return ((int) ntohs(this->DadosCliente.sin_port));
}


string Nsock::getInput()
{
	if(this->Entrada.empty()){
		//throw std::runtime_error("Erro Sem Dados No buffer");
		return "empty";
	}
	string entrada=this->Entrada.front();
	this->Entrada.pop_front();
	return entrada;
}

int Nsock::SendMsg(string msg)
{
	int status=send(this->cliente,msg.c_str(),msg.length(),0);
	if(status<0){
		this->erro=1;
		map<int,Nsock*>::iterator it;
		it=this->connections->find(this->id);
		this->connections->erase(it);
		return 0;
	}
	return 1;
}

void Nsock::Close(){
	closesocket(this->cliente);
}