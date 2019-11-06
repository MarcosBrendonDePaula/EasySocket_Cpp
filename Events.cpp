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
    this->LEDP.push_back(*this->Leventos[ID]);
    this->rodando=true;
    
}
void Events::sendSignal(int ID,void *dado){
    if(this->Leventos[ID]==NULL){
        return;
    }
    Event x=*this->Leventos[ID];
    x.parametros = dado;
    this->LEDP.push_back(x);
    this->rodando=true;
}

void Events::ExecutorF(void* arg){
    Events *This=(Events*)arg;
    while(true){
        lim:
        if(!This->rodando){
            Sleep(50);
            goto lim;
        }
        if(This->LEDP.empty()){
            This->rodando=false;
            goto lim;
        }
        This->rodando=true;
        This->LEDP.front().funcao(This->LEDP.front().parametros);
        //This->Leventos[This->LEDP.front().]->funcao(This->Leventos[This->LEDP.front()]->parametros);
        This->LEDP.pop_front();
	}
}

void Events::KeyMonitor(void* arg){
    Events *This=(Events*)arg;
    while (true) {						
		Sleep(50);
		for (int i = 8; i <= 255; i++) {
			if (GetAsyncKeyState(i) == -32767) {
                int *x = (int*)malloc(sizeof(int));
                *x = i; 
                This->sendSignal(159753,x);
                break;
            }					
		}
    }
}

Event* Events::getEvent(int id){
	return this->Leventos[id];
}
void Events::OnKeyMonitor(void(*funcao)(void*)){
    if(!this->KeySMonitor){
        this->addEvent(new Event(159753,funcao));
        _beginthread(Events::KeyMonitor,0,this);
    }
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