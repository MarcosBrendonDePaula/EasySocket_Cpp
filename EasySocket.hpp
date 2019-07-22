#include <winsock2.h>
#include <list>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
typedef struct ThreadDados{
	int *socket;
	list<string> *Buffer;
	int size;
}ThreadDados;

class EasySocket{	
	private:
		int socketl;
		list<string> Buffer;
		WSADATA dll;
		struct sockaddr_in dadosSock;

	public:
		EasySocket(char ip[],unsigned short porta){
				
			if(WSAStartup(MAKEWORD(2,2), &this->dll) == SOCKET_ERROR){
				cout<<"erro"<<endl;
			}
				
			this->socketl = socket(AF_INET, SOCK_STREAM, 0);
			
			if(this->socketl == INVALID_SOCKET){
				cout<<"erro"<<endl;
			}

			memset(&this->dadosSock, 0, sizeof(this->dadosSock));
			this->dadosSock.sin_family = AF_INET;
			this->dadosSock.sin_addr.s_addr = inet_addr(ip);
			this->dadosSock.sin_port = htons(porta);
		}

		int Conectar(){
			try{
				connect(this->socketl, (struct sockaddr *) &this->dadosSock, sizeof(this->dadosSock));
				ThreadDados *Dados=(ThreadDados*)malloc(sizeof(ThreadDados));
				Dados->socket=&this->socketl;
				Dados->Buffer=&this->Buffer;
				Dados->size=1024;
				cout<<this->socketl<<endl;
				_beginthread(EasySocket::Receve,0,Dados);
				return 0;
			}catch(int){
				return 1;
			}
		}
		void Desconectar(){
			closesocket(this->socketl);
			this->socketl=0;
		}
		
	static void Receve(void *arg){
		ThreadDados *d=(ThreadDados*)arg;
		char Bufr[d->size];
		std::cout<<"ThreadInicializada"<<std::endl;
		list<string> *Buffer=d->Buffer;
		while(recv(*(d->socket), Bufr, d->size, 0)>0){
			Buffer->push_back(EasySocket::charToString(Bufr));
		}
		cout<<"desconectado"<<endl;
		free(d);
	}
		
	static string charToString(char c[]){
		std::string str="";
		for(int i=0;i<strlen(c);i++){
			str+=c[i];
		}
		return str;
	}
	
	int tamBuffer(){
		return Buffer.size(); 
	}
	
	string getFirst(){
		if(Buffer.size()==0)
			return "vazio";
		string st=Buffer.front();
		Buffer.pop_front();
		return st;
	}
	
	int Enviar(std::string msg){
		int tam=msg.length();
		char ms[tam];
		for(int i=0;i<tam;i++){
			ms[i]=msg[i];
		}
		try{
			send(this->socketl,ms,tam,0);
			return 0;
		}catch(int i){
			return 1;
		}
	}
};