#include "nsock.h"
Events nsock::evs;
void nsock::ReceiveFunction(void *arg)
{
	nsock* This=(nsock*)arg;
	char buffer[2048];
	memset(buffer,0x0,sizeof(buffer));
	while(recv(This->cliente,buffer,sizeof(buffer),0)>0){
		string entrada(buffer);
		This->Entrada.push_back(entrada);
		/*if(!This->evs.getEvent(1)->funcao)
			This->ordem->push_back(This->id);*/
		nsock::evs.getEvent(1)->parametros=This;
		nsock::evs.sendSignal(1);
		memset(buffer,0x0,sizeof(buffer));
	}
	int posicao=-1;
	for(vector<nsock*>::iterator it=This->conexoes->begin();it!=This->conexoes->end();it++){
		posicao++;
		if(!(*it)->id-This->id){
			free((*it));
			break;
		}
	}
	This->Entrada.clear();
	This->conexoes->erase(This->conexoes->begin()+posicao);
	return;
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
	_beginthread(nsock::ReceiveFunction,0,this);
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
