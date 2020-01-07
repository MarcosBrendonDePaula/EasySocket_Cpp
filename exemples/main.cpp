#include <iostream>
#include "EasyMultServer.h"
#include "EasySocket.h"
using namespace std;
EasyMultServer *sv;
void tratamento(void *arg){
	Nsock* socket=(Nsock*)arg;
    cout<<"Chegou:"<<socket->getInput()<<endl;
}
void Aceitado(void *arg){
    Nsock *aceitado = (Nsock*)arg;
    cout<<"id aceita:"<< aceitado->id << endl;
}

void tratamento1(void *arg){

}

int main(){
    Events *ev=new Events();
    sv = new EasyMultServer(25565,ev);
	sv->Start(&tratamento,10,&Aceitado);
    EasySocket *con;
	while(true){
		int x;
		cin>>x;
        con = new EasySocket("127.0.0.1",25565,&tratamento1,ev);
        con->conectar();
    }
}