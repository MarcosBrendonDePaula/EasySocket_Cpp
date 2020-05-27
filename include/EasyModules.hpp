#include "EasyMultServer.h"
#include "EasySocket.h"
#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

namespace EasyModule{
	
    void reciveFile(int socket,string out,bool monitor){
        ofstream file(out,ios::out | ios::binary);
        double size,sizeF,sizeA=0;
        char sizebf[1499]={};
        recv(socket,sizebf,1499,0);
        sizeF = atoi(sizebf);
        do{
            char buff[1499]={},ack[2]={'o','k'};
            size = recv(socket,buff,1499,0);
            send(socket,ack,2,0);
            if(strcmp(buff,"{-endf-}")==0)
                break;
            file.write(buff,size);
            sizeA=sizeA+size;
            if(monitor)
                cout<<(sizeF/1000000)<<" -> "<<(sizeA/1000000)<<endl;
        }while(size==1499);
        file.close();
    }

    void sendFile(int socket,string in,bool monitor){
        ifstream file(in,ios::binary|ios::in|ios::ate);
        int size = file.tellg();
        double sizeA = 0,sizeF=size;
        file.seekg (0, ios::beg);

        send(socket,to_string(size).c_str(),to_string(size).length(),0);
        if(size>=1499){
            char buff[1499]={},ack[2]={};
            file.read(buff,1499);
            do{
                send(socket,buff,1499,0);
                recv(socket,ack,2,0);
                sizeA+=1499;
                if(monitor)
                    cout<<(sizeF/1000000)<<" -> "<<(sizeA/1000000)<<endl;
            }while(file.read(buff,1499));
        }else{
            char buff[size]={};
            file.read(buff,size);
            send(socket,buff,size,0);
        }
        char buffF[]="{-endf-}";
        send(socket,buffF,8,0);
    }
    
    void recivePreFile(int socket,string out,bool monitor){
        double size,sizeA=0;
        char sizebf[1499]={};
        unsigned long long int sizeF;
        recv(socket,sizebf,1499,0);
        
        sizeF = atoi(sizebf);

        int resto = (((int)sizeF)%1499);

        vector<char*> Binarios;
        cout<<"Rec >"<<((sizeF/1499)-(resto>0))<<endl;
        for(int i=0;i<((sizeF/1499)-(resto>0));i++){
            char *buff=new char[1499],ack[2]={'o','k'};
            buff[1499]={};
            recv(socket,buff,1499,0);
            send(socket,ack,2,0);
            Binarios.push_back(buff);
            if(monitor){
                cout<<(i*1499)<<":<->:"<<sizeF<<endl;
            }
        }
        if(resto){
            char *buff=new char[resto];
            size = recv(socket,buff,resto,0);
            Binarios.push_back(buff);
            if(monitor){
                cout<<resto+(Binarios.size()*1499)<<":<->:"<<sizeF<<endl;
            }
        }

        /*do{
            char *buff=new char[1499],ack[2]={'o','k'};
            buff[1499]={};
            size = recv(socket,buff,1499,0);
            send(socket,ack,2,0);
            if(sizeA>=sizeF)
                break;
            Binarios.push_back(buff);
            sizeA=sizeA+size;
            if(monitor)
                cout<<(sizeF/1000000)<<" -> "<<(sizeA/1000000)<<endl;
        }while(true);*/

        ofstream file(out,ios::out | ios::binary);
        for(int i=0;i<Binarios.size()-1;i++){
            file.write(Binarios[i],1499);
        }
        file.write(Binarios[(Binarios.size()-1)],size);
        file.close();
    }

    void sendPreFile(int socket,string in,bool monitor){
        ifstream file(in,ios::binary|ios::in|ios::ate);
        int size = file.tellg();
        file.seekg (0, ios::beg);
        int resto = (size%1499);
        vector<char*> Binarios;
        cout<<"Send >"<<((size/1499)-(resto>0))<<endl;
        for(int i=0;i<((size/1499)-(resto>0));i++){
            char *temp=new char[1499];
            temp[1499]={};
            file.read(temp,1499);
            Binarios.push_back(temp);
        }
        if(resto){
            char *temp=new char[resto];
            temp[resto]={};
            file.read(temp,resto);
            Binarios.push_back(temp);
        }
        file.close();
        //Enviando
        double percent=0;
        send(socket,to_string(size).c_str(),to_string(size).length(),0);
        for(int x=0;x<(Binarios.size()-(resto>0));x++){
            char buffer[2]={};
            send(socket,Binarios[x],1499,0);
            recv(socket,buffer,2,0);
            if(monitor){
                cout<<(x*1499)<<":<->:"<<size<<endl;
            }
        }

        if(resto){
            send(socket,Binarios[Binarios.size()-1],resto,0);
            if(monitor){
                cout<<resto+(Binarios.size()*1499)<<":<->:"<<size<<endl;
            }
        }
    }
    //-------------------------------------------------------------------Nsock
    template<typename __type> bool sendVar(Nsock* socket,__type var){
			char *temp = (char*)&var;
			if(send(*(socket->getClient()),temp,sizeof(__type),0)>=0){return true;}
			return false;
	}

    template<typename __type> __type* ReciveVar(Nsock* socket,__type* rec){
        __type *temp=rec;
        if(recv(*(socket->getClient()),(char*)temp,sizeof(__type),0)>0){return temp;};
        return NULL;
    }

    //-------------------------------------------------------------------EasySocket
    template<typename __type> bool sendVar(EasySocket* socket,__type var){
        char *temp = (char*)&var;
        if(send(socket->getSocket(),temp,sizeof(__type),0)>=0){return true;}
        return false;
	}

    template<typename __type> __type* ReciveVar(EasySocket*  socket,__type* rec){
        __type *temp=rec;
        if(recv(socket->getSocket(),(char*)temp,sizeof(__type),0)>0){return temp;};
        return NULL;
    }

	vector<string> split(string str,char delimiter){
		stringstream ss(str);string token;vector<string> keys;
		while (getline(ss, token, delimiter)){keys.push_back(token);}
		return keys;
	}

}