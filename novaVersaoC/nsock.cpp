#include "nsock.h"

void* nsock::ReceiveFunction(void *arg)
{
	nsock* This=(nsock*)arg;
	char buffer[2048];
	memset(buffer,0x0,sizeof(buffer));
	while(recv(This->cliente,buffer,sizeof(buffer),0)>0){
		string entrada(buffer);
		This->Entrada.push_back(entrada);
		This->ordem->push_back(This->id);
		EventsMain::Eventosmain.EnviarSinal(1);
		memset(buffer,0x0,sizeof(buffer));
	}
	for(vector<nsock*>::iterator it=This->conexoes->begin();it!=This->conexoes->end();it++)
		if((*it)->id==This->id){
			This->conexoes->erase(it);
		}
	pthread_exit(NULL);
	return NULL;
}

nsock::nsock(list<int> *lista,vector<nsock*> *cn)
{
	this->ordem=lista;
	this->conexoes=cn;
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
void nsock::start()
{
	pthread_create(&this->Process,NULL,nsock::ReceiveFunction,this);
}
string nsock::getEntrada()
{
	string entrada=this->Entrada.front();
	this->Entrada.pop_front();
	return entrada;
}
void nsock::Enviar(string msg)
{
	send(this->cliente,msg.c_str(),msg.length(),0);
}