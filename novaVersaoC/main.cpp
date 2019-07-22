#include <iostream>
#include "EasyMultServer.h"
#include <winsock2.h>
#include <sstream>
//#include "EasySocket.h"
using namespace std;

//servidor
EasyMultServer sv(25567);
//fução de tratamento
void tratamento(){
	cout<<"Id:"<<EasyMultServer::OrdemDeChegada.front()<<endl;
	if(sv.getConID(EasyMultServer::OrdemDeChegada.front())!=NULL){
		string entrada=sv.getConID(EasyMultServer::OrdemDeChegada.front())->getEntrada();
		if(entrada.find("file")==0){
			for(int i=0;i<10;i++){
				for(int x=0;x<10;x++){
					sv.getConID(EasyMultServer::OrdemDeChegada.front())->Enviar("astron\n");
				}
			}
		}
	}
	EasyMultServer::OrdemDeChegada.pop_front();
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