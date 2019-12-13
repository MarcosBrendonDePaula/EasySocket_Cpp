#include "EasySocket.h"
#include <iostream>
#include <cstring>
#include <process.h>

EasySocket::EasySocket(std::string ip, int porta,void(*funcao)(void *arg)){
	Events::static_Acess->addEvent(new Event(3,funcao,NULL));
	memset(&this->InformacoesConection, 0x0, sizeof(this->InformacoesConection));
	this->InformacoesConection.sin_port = htons(porta);
	this->InformacoesConection.sin_addr.s_addr = inet_addr(ip.c_str());
	this->InformacoesConection.sin_family = AF_INET;
	if ((this->conection = socket(AF_INET, SOCK_STREAM, 0))<0)
		std::cout << "Erro ao Criar Socket" << std::endl;
}

int EasySocket::bffSiz = 1024;
int EasySocket::conectar(){
	if (connect(this->conection, (struct sockaddr*)&this->InformacoesConection, sizeof(this->InformacoesConection)) < 0){
		std::cout << "Erro De Conexao Verifique IP/Porta" << std::endl;
		this->closed=true;
		return 0;
	}
	this->closed=false;
	_beginthread(EasySocket::ReceiverDefault, 0, this);
	return 1;
}

int EasySocket::Enviar(std::string msg){
	if ((send(this->conection, msg.c_str(), msg.length(), 0)) < 0){
		std::cout << "Erro ao enviar"<< std::endl;
		return 0;
	}
	return 1;
}

void EasySocket::ReceiverDefault(void *arg){
	EasySocket *sk = (EasySocket*)arg;
	char buffer[1024];
	memset(buffer, 0x0, sizeof(buffer));
	while (recv(sk->conection, buffer, sizeof(buffer), 0)>0)
	{
		string nm(buffer);
		sk->Entradas.push_back(nm);
		memset(buffer, 0x0, sizeof(buffer));
		Events::static_Acess->sendSignal(3);
	}
	std::cout << "Desconectado" << std::endl;
	closesocket(sk->conection);
	sk->closed=true;
}

string EasySocket::getEntrada(){
	string temp = this->Entradas.front();
	this->Entradas.pop_front();
	return temp;
}

int EasySocket::qtEntradas(){
	return this->Entradas.size();
}

void EasySocket::Desconectar(){
	closesocket(this->conection);
}

bool EasySocket::isClosed(){
	return this->closed;
}

EasySocket::~EasySocket()
{
	
}
