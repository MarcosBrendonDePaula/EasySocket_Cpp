#include <iostream>
#include "EasyMultServer.h"
#include <winsock2.h>
#include <sstream>
//#include "EasySocket.h"
using namespace std;

//servidor
EasyMultServer sv(25567);
//fução de tratamento
void tratamento(void *arg){
	nsock* socket=(nsock*)arg;
	cout<<"Recebido:"<<socket->getEntrada()<<endl;
}
int main(){
		//--------EXEMPLO SERVIDOR-----------//
	//inicializando servidor
	sv.StartServer(&tratamento);
	while(true){
		int x;
		try{
			cin>>x;
		}catch(int i){
			cout<<"Eroo "<<i<<endl;
		}
		//so para manter a thread main aberta 
		//e esperando algo que não tera para reduzir o uso de cpu
	}
}