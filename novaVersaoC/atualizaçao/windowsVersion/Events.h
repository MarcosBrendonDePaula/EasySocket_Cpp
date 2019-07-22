#include <iostream>
#include <pthread.h>
#include <map>
#include <list>
#include <windows.h>
#include <unistd.h>
#include <stdio.h>
class Event{
public:
    Event(int ID,void(*funcao)(void*),void *parametro);
    int ID;
    void *parametros;
    void(*funcao)(void*);
};
class Events{
public:
    static void* ExecutorF(void* arg);
    static Events *static_Acess;
    Events();
    void addEvent(Event *E);
    void sendSignal(int ID);
	Event* getEvent(int id);
private:
    bool Debug;
    pthread_t executor;
    std::list<int> LEDP;
    std::map<int,Event*> Leventos;
    bool rodando;
};