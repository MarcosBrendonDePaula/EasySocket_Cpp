#include "EasyMultServer.h"
#include "EasySocket.h"
#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

namespace EasyModule{
	typedef unsigned long long int ulli;
    class File{
        public:
            vector<char*> Binary;
            ulli FileSize;
            int Mod;
            bool FileOpen(string filename){
                ifstream file(filename,ios::binary|ios::in|ios::ate);
                if(!file.is_open()){
                    return false;
                }
                this->FileSize = file.tellg();
                file.seekg (0, ios::beg);
                this->Mod = (this->FileSize%1499);
                
                for(ulli i=0;i<=(this->FileSize-this->Mod);i+=1499){
                    char *temp = new char[1499];
                    temp[1499] = {};
                    file.read(temp,1499);
                    Binary.push_back(temp);
                }

                if(this->Mod){
                    char *temp=new char[Mod];
                    temp[Mod]={};
                    file.read(temp,Mod);
                    Binary.push_back(temp);
                }
                file.close();
                return true;
            }

            void ClearMem(){
                for(auto i:this->Binary){
                    free(i);
                }
            }
            bool FileSave(const string local){
                if(local.empty()||Binary.empty())
                    return false;
                ofstream file(local,ios::out | ios::binary);
                for(int i=0;i<(Binary.size()-1);i++){
                    file.write(Binary[i],1499);
                }
                file.write(Binary.back(),Mod);
                file.close();
                return true;
            }
    };

    File recivePreFile(int socket,string out,bool monitor){
        char ack[2]={'o','k'}; //miniAck
        char FileSBf[1499]={};
        recv(socket,FileSBf,1499,0);

        File temp;
        temp.FileSize = atoi(FileSBf);
        temp.Mod = (temp.FileSize%1499);

        for(int i=0;i<=temp.FileSize;){
            char MaxBuffer[1499]={};

            int SizeR = recv(socket,MaxBuffer,1499,0);
            if(SizeR==-1){
                cout<<SizeR<<endl;
                temp.Binary.clear();
                return temp;
            }
            send(socket,ack,2,0);

            char *tempc = new char[SizeR];
            memcpy(tempc,MaxBuffer,SizeR);
            temp.Binary.push_back(tempc);

            i+=SizeR;
            if(monitor){
                cout<<(i/1000000)<<"Mb <->"<<(temp.FileSize/1000000)<<"Mb"<<endl;
            }
        }
        return temp;
    }

    void sendPreFile(int socket,File& temp,bool monitor){
        send(socket,to_string(temp.FileSize).c_str(),to_string(temp.FileSize).length(),0);
        char buffer[2]={};
        ulli BitAtual=0;
        for(int i=0;i<(temp.Binary.size()-1);i++){
            send(socket,temp.Binary[i],1499,0);
            recv(socket,buffer,2,0);
            if(monitor){
                cout<<(BitAtual/1000000)<<"Mb <->"<<(temp.FileSize/1000000)<<"Mb"<<endl;
                BitAtual+=1499;
            }
        }
        send(socket,temp.Binary.back(),temp.Mod,0);
        recv(socket,buffer,2,0);
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