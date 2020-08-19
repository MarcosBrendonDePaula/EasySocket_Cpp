#pragma once
#define ThreadMonitor true
#define __Nocheck__ true
#ifdef __linux__
    #include <iostream>
    #include <pthread.h>
#elif _WIN32
    #include <process.h>
    #include <windows.h>
#endif
#include <map>
#include <list>
#include <unistd.h>
#include <stdio.h>

class Event
{
    public:
        Event(int ID,void(*funcao)(void*),void *parametro);
        Event(int ID,void(*funcao)(void*));
        Event(int ID,void(*funcao)());
        int ID;
        void *parametros;
        void(*funcao)(void*);
};

class Events
{
    public:
    #ifdef __linux__
        static void* ExecutorF(void* arg);
        static void* callFunction(void* arg);
    #elif _WIN32
        static void ExecutorF(void* arg);
        static void KeyMonitor(void* arg);
        void OnKeyMonitor(void(*funcao)(void*));
        bool KeySMonitor = false;
    #endif
        Events();
        void addEvent(Event *E);
        void sendSignal(int ID);
        Event* getEvent(int id);
        std::list<int> LEDP;
        bool Paralel;
    private:
        bool Debug;
    #ifdef __linux__
        pthread_t executor;
    #endif
        std::map<int,Event*> Leventos;
        bool rodando;
};