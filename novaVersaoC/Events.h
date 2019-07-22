#include <iostream>
#include <list>
#include <map>
#include <windows.h>
#include <pthread.h>
using namespace std;
class Evento{
public:
	Evento(int id,void(*funcao)());
	void(*funcao)();
	int Id;
};

class Eventos{
private:
	pthread_t EventProcess;
	int preparado;
public:
	Eventos();
	static void *EXEvents(void *arg);
	static int started;
	static list<int> sinais;
	static map<int,Evento*> eventos;
	void EnviarSinal(int id);
	static void addEvent(Evento* ev);
};