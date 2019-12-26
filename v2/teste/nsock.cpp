#include "nsock.h"
Events nsock::evs;
#ifdef _WIN32
	void nsock::ReceiveFunction(void *arg)
#elif __linux__
	#define SOCKET int
	#define closesocket close
	void* nsock::ReceiveFunction(void *arg)
#endif
{
	nsock* This=(nsock*)arg;
	char buffer[2048];
	memset(buffer,0x0,sizeof(buffer));
	while((recv(This->cliente,buffer,sizeof(buffer),0)>0) && !This->erro){
		string entrada(buffer);
		This->Entrada.push_back(entrada);
		nsock::evs.getEvent(1)->parametros=This;
		nsock::evs.sendSignal(1);
		memset(buffer,0x0,sizeof(buffer));
	}
	map<int,nsock*>::iterator it;
	it=This->connections->find(This->id);
	This->connections->erase(it);
	#ifdef _WIN32
		return;
	#endif
}

nsock::nsock(list<int> *lista,map<int,nsock*> *cn)
{
	this->ordem=lista;
	this->connections=cn;
	memset(&this->DadosCliente,0x0,sizeof(this->DadosCliente));
}
SOCKET* nsock::getCliente()
{
	return &this->cliente;
}
struct sockaddr_in* nsock::getDadosCliente()
{
	return &this->DadosCliente;
}
int nsock::start()
{
	#ifdef _WIN32
		_beginthread(nsock::ReceiveFunction,0,this);
	#elif __linux__
		pthread_create(&this->ThreadCliente,NULL,nsock::ReceiveFunction,this);
	#endif
	return 1;
}
string nsock::getEntrada()
{
	if(this->Entrada.empty()){
		throw std::runtime_error("Erro Sem Dados No buffer");
	}
	string entrada=this->Entrada.front();
	this->Entrada.pop_front();
	return entrada;
}
int nsock::Enviar(string msg)
{
	int status=send(this->cliente,msg.c_str(),msg.length(),0);
	if(status<0){
		this->erro=1;
		map<int,nsock*>::iterator it;
		it=this->connections->find(this->id);
		this->connections->erase(it);
		return 0;
	}
	return 1;
}
void nsock::Close(){
	closesocket(this->cliente);
}
