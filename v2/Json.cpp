#include "Json.h"

void Json::addVar(std::string name,std::string valor){
    this->variaveis[name]=valor;
}

void Json::addList(std::string name,std::vector<std::string> list){
    std::string lista = "[";
    for(int i=0;i<list.size();i++)
        lista+=list[i]+",";
    lista+="]";
    this->variaveis[name] = lista;
}

void Json::addListPath(std::string path,std::string name,std::vector<std::string> list){
    std::vector<std::string> result = Json::split(path,'.');
    Json *temp = this;
    for(int i=0;i<result.size();i++){
        temp = &temp->SubJsons[result[i]];
    }
    std::string lista = "[";
    for(int i=0;i<list.size();i++)
        lista+=list[i]+",";
    lista+="]";
    temp->variaveis[name] = lista;
}

void Json::addVarPath(std::string path,std::string name,std::string valor){
    std::vector<std::string> result = Json::split(path,'.');
    Json *temp = this;
    for(int i=0;i<result.size();i++){
        temp = &temp->SubJsons[result[i]];
    }
    temp->addVar(name,valor);
}

std::string Json::getVal(std::string path){
    return this->variaveis[path];
}

std::string Json::getValPath(std::string path,std::string name){
    std::vector<std::string> result = Json::split(path,'.');
    Json *temp = this;
    for(int i=0;i<result.size();i++){
        temp = &temp->SubJsons[result[i]];
    }
    return temp->variaveis[name];        
}

std::vector<std::string> Json::split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;
    while (getline (ss, item, delim)) {
        result.push_back (item);
    }
    return result;
}

std::string Json::to_string(){
    return GenScopo(*this);
}
std::string Json::GenScopo(Json atual){
    std::string gen = "{";
    for(std::map<std::string,std::string>::iterator x=atual.variaveis.begin();x!=atual.variaveis.end();x++){
            gen+="\""+x->first+"\":\""+x->second+"\",";
    }
    if(atual.SubJsons.size()){
        for(std::map<std::string,Json>::iterator x=atual.SubJsons.begin();x!=atual.SubJsons.end();x++){
            gen+="\""+x->first+"\":";
            gen+="[";
            gen+=Json::GenScopo(x->second);
            gen+="],";
        }
    }
    gen+="\"fim\":\"fim\"}";
    return gen;
}