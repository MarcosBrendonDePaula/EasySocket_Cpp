#include "Events.h"
Events* Events::static_Acess;

/*
    Construtor Eventos
*/
Events::Events(){
    //this->Debug=Debug;
    this->rodando=0;
    Events::static_Acess=this;
    _beginthread(Events::ExecutorF,0,this);

}

void Events::addEvent(Event *E){
    this->Leventos[E->ID]=E;
}

void Events::sendSignal(int ID){
    if(this->Leventos[ID]==NULL){
        return;
    }
    this->LEDP.push_back(ID);
    this->rodando=true;
    
}

void Events::ExecutorF(void* arg){
    Events *This=(Events*)arg;
    	while(true){
		lim:
		if(!This->rodando){
            Sleep(500);
			goto lim;
		}
		if(This->LEDP.empty()){
			This->rodando=false;
			goto lim;
		}
		This->rodando=true;
		This->Leventos[This->LEDP.front()]->funcao(This->Leventos[This->LEDP.front()]->parametros);
		This->LEDP.pop_front();
	}
}
Event* Events::getEvent(int id){
	return this->Leventos[id];
}
/*
    Construtor Evento
*/
Event::Event(int ID,void(*funcao)(void*),void *parametro){
    this->funcao=funcao;
    this->ID=ID;
    this->parametros=parametro;
}
Event::Event(int ID,void(*funcao)(void*)){
    this->funcao=funcao;
    this->ID=ID;
    this->parametros=NULL;
}