#include "cliente.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <process.h>
#include <stdexcept>
using namespace std;
int Cliente::contador=0;

Cliente::Cliente(int *sock,list<int> *ord){
	this->socket=*sock;
	this->dadosTh.socket=this->socket;
	this->dadosTh.Buffer=&this->buffer;
	this->dadosTh.id=Cliente::contador;
	this->dadosTh.ordem=ord;
	this->id=Cliente::contador;
	Cliente::contador++;
	 _beginthread(Cliente::processamento,1,&this->dadosTh);
}

int Cliente::getSocket(){
	return this->socket;
}

int* Cliente::getPointerSocket(){
	return &this->socket;
}

int Cliente::getID(){
	return this->id;
}

std::string Cliente::getFirst(){
		if(this->buffer.size()==0)
			return "Vazio";
		std::string strings=this->buffer.front();
		this->buffer.pop_front();
		return strings;
}

void Cliente::Enviar(std::string msg){
	int tam=msg.length();
	char ms[tam];
	for(int i=0;i<tam;i++){
		ms[i]=msg[i];
	}
	send(this->socket,ms,tam,0);
}

std::string Cliente::charToString(char c[]){
	std::string str="";
	for(int i=0;i<strlen(c);i++){
		str+=c[i];
	}
	return str;
}

void Cliente::processamento(void* arg){
	dados *atuais=(dados*)arg;
	int socket=atuais->socket;
	int id=atuais->id;
	char Rec[1024];
	memset(Rec,0,sizeof(Rec));
	list<std::string> *Buffer=atuais->Buffer;
	while(recv(socket,Rec,sizeof(Rec),0)!= 0){
		string str=Cliente::charToString(Rec);
		Buffer->push_back(str);
		atuais->ordem->push_back(id);
		memset(Rec,0,sizeof(Rec));
	}
	std::cout<<"socket: "<<socket<<" desconectado"<< std::endl;
	closesocket(socket);
}
