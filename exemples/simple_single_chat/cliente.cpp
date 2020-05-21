#include "../include/EasySocket.h"
#include "../include/Events.h"
using namespace std;
void RecEvent(void *arg)
{
    EasySocket *SK=(EasySocket*)arg;
    cout<<SK->getInput()<<endl;
}

int main()
{
    //The management class for the current instance
    Events Eventos;
    
    string ip;
    int porta;
    
    cout<<"type the IP address"<<endl;
    cin>>ip;
    cout<<"type the port"<<endl;
    cin>>porta;
    
    EasySocket Conexao(ip,porta,&RecEvent,&Eventos);
    string msg;
    if(Conexao.Connect())
    {
        while(!Conexao.isClosed())
        {
            cin>>msg;
            Conexao.SendMsg(msg);
        }
    }else
    {
        cout<<"Error"<<endl;
    }
    return 0;
}