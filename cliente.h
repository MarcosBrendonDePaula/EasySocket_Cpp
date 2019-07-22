#include <winsock2.h>
#include <process.h>
#include <list>
#include <string>
using namespace std;

typedef struct dados{
	int socket;
	std::list<std::string> *Buffer;
	int id;
	std::list<int> *ordem;
}dados;

class Cliente{
private:
	int socket;
	dados dadosTh;
public:
	int id;
	list<string> buffer;
	std::string getFirst();
	Cliente(int *sock,list<int> *ord);
	int getSocket();
	int getID();
	static int contador;
	int* getPointerSocket();
	void Enviar(string msg);
	static list<int> *Ordem;
	static void processamento(void* arg);
	static string charToString(char c[]);
};
