#include "../include/EasySocket.h"
#include "../include/Events.h"
using namespace std;
void recepcao(void *arg)
{
    EasySocket *SK=(EasySocket*)arg;
    cout<<SK->getInput()<<endl;
}

int main()
{
    Events Eventos;
    string ip;
    int porta;
    cout<<"Informe o IP"<<endl;
    cin>>ip;
    cout<<"Informa A porta"<<endl;
    cin>>porta;
    EasySocket Conexao(ip,porta,&recepcao,&Eventos);
    if(Conexao.Connect())
    {
        while(!Conexao.isClosed())
        {
            cin>>ip;
            Conexao.SendMsg(ip);
        }
    }
    return 0;
}