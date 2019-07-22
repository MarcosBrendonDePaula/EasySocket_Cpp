#include "Events.h"

int Eventos::started;
list<int> Eventos::sinais;
map<int,Evento*> Eventos::eventos;

Eventos::Eventos(){
	this->preparado=0;
	pthread_create(&this->EventProcess,NULL,Eventos::EXEvents,this);
	//pthread_join(this->EventProcess,NULL);
}

void Eventos::EnviarSinal(int id){
	Eventos::sinais.push_back(id);
	if(!(Eventos::started)){
		this->preparado=1;
	}
}

void Eventos::addEvent(Evento* ev){
	Eventos::eventos[ev->Id]=ev;
}
void* Eventos::EXEvents(void *arg){
	Eventos* This=(Eventos*)arg;
	while(true){
		lim:
		if(!This->preparado){
			Sleep(100);
			goto lim;
		}
		if((!(Eventos::sinais.size()))){
			Eventos::started=false;
			This->preparado=0;
			goto lim;
		}
		Eventos::started=true;
		Eventos::eventos[Eventos::sinais.front()]->funcao();
		Eventos::sinais.pop_front();
	}
}
Evento::Evento(int id,void(*funcao)()){
	this->funcao=funcao;
	this->Id=id;
}