#include "EasySocket.h"
#ifdef __linux__
	#define SOCKET int
#endif
EasySocket::EasySocket(std::string ip, int porta,void(*funcao)(void *arg),Events *evs){
	strcpy(this->vazio,"empty");
	this->Buffersize=1024;
	#ifdef _WIN32
		WSAStartup(MAKEWORD(2,2), &this->dll);
    #endif
	this->Evs = evs;
	this->Evs->addEvent(new Event(3,funcao,NULL));
	memset(&this->InformacoesConection, 0x0, sizeof(this->InformacoesConection));
	this->InformacoesConection.sin_port = htons(porta);
	this->InformacoesConection.sin_addr.s_addr = inet_addr(ip.c_str());
	this->InformacoesConection.sin_family = AF_INET;
	if ((this->conection = socket(AF_INET, SOCK_STREAM, 0))<0)
		std::cout << "Erro ao Criar Socket" << std::endl;
}

int EasySocket::Connect(){
	if (connect(this->conection, (struct sockaddr*)&this->InformacoesConection, sizeof(this->InformacoesConection)) < 0){
		std::cout << "Erro De Conexao Verifique IP/Porta" << std::endl;
		this->closed=true;
		return 0;
	}
	this->closed=false;
	#ifdef _WIN32
		_beginthread(EasySocket::ReceiverDefault, 0, this);
	#elif __linux__
		pthread_create(&this->ThreadCliente,NULL,EasySocket::ReceiverDefault,this);
	#endif
	return 1;
}

#ifdef _WIN32
	void EasySocket::ReceiverDefault(void *arg){
#elif __linux__
	void* EasySocket::ReceiverDefault(void *arg){
#endif
	EasySocket *sk = (EasySocket*)arg;
	char buffer[sk->Buffersize];
	memset(buffer, 0x0, sizeof(buffer));
	while (recv(sk->conection, buffer, sizeof(buffer), 0)>0)
	{
		char *x=(char*)malloc(sizeof(buffer));
		strcpy(x,buffer);
		sk->EntradasVector.push_back(x);
		string nm(buffer);
		sk->Entradas.push_back(nm);
		memset(buffer, 0x0, sizeof(buffer));
		sk->Evs->getEvent(3)->parametros=sk;
		sk->Evs->sendSignal(3);
	}
	closesocket(sk->conection);
	sk->closed=true;
#ifdef _WIN32
	return;
#elif __linux__
	return NULL;
#endif
}

sockaddr_in* EasySocket::getSockAddr(){
	return &this->InformacoesConection;
}

string EasySocket::getInput(){
	string temp = this->Entradas.front();
	this->Entradas.pop_front();
	free(this->EntradasVector.front());
	this->EntradasVector.pop_front();
	return temp;
}

char* EasySocket::getInputVector(){
	if(this->EntradasVector.empty())
		return this->vazio;
	char *atual = this->EntradasVector.front();
	this->EntradasVector.pop_front();
	this->Entradas.pop_front();
	return atual;
}

list<string>* EasySocket::getInputs(){
	return &this->Entradas;
}

int EasySocket::SendMsg(std::string msg){
	if ((send(this->conection, msg.c_str(), msg.length(), 0)) < 0){
		std::cout << "Erro ao enviar"<< std::endl;
		return 0;
	}
	return 1;
}

void EasySocket::Disconnect(){
	closesocket(this->conection);
}

bool EasySocket::isClosed(){
	return this->closed;
}

SOCKET EasySocket::getSocket(){
	return this->conection;
}

EasySocket::~EasySocket(){
	
}
