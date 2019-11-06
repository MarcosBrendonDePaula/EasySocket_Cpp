#include "nsock.h"
Events nsock::evs;
void nsock::ReceiveFunction(void *arg)
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
	if(This->erro)
		return;

	vector<nsock*>::iterator it;
	int posicao=0;
	for(it=This->conexoes->begin();it!=This->conexoes->end();it++){
		if((*it)->id != This->id)
			posicao++;
		else
			break;
	}
	free((*it));
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
	this->erro=0;
	_beginthread(nsock::ReceiveFunction,0,this);
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
void nsock::Enviar(string msg)
{

	int status=send(this->cliente,msg.c_str(),msg.length(),0);
	if(status<0){
		this->erro=1;
		int posicao=0;
		for(vector<nsock*>::iterator it=this->conexoes->begin();it!=this->conexoes->end();it++){
			if((*it)->id != this->id)
				posicao++;
			else{
				free((*it));
				this->conexoes->erase(this->conexoes->begin()+posicao);
				break;
			}
		}
	}
}
