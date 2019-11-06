#include <iostream>
#include <map>
#include <list>
#include <process.h>
#include <windows.h>
#include <unistd.h>
#include <stdio.h>
class Event{
public:
    Event(int ID,void(*funcao)(void*),void *parametro);
    Event(int ID,void(*funcao)(void*));
    Event(int ID,void(*funcao)());
    int ID;
    void *parametros;
    void(*funcao)(void*);
};
class Events{
public:
    static void ExecutorF(void* arg);
    static void KeyMonitor(void* arg);
    void OnKeyMonitor(void(*funcao)(void*));
    static Events *static_Acess;
    Events();
    bool KeySMonitor=false;
    void addEvent(Event *E);
    void sendSignal(int ID);
    void sendSignal(int ID,void* dado);
	Event* getEvent(int id);
private:
    bool Debug;
    std::list<Event> LEDP;
    std::map<int,Event*> Leventos;
    bool rodando;
};