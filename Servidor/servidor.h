#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#define MY_PORT 7979
#define MAX 1024 
//docker build -t n19alisson/hidrometro ./

using namespace std;

typedef struct historico_consumo{
    int id;
    string cep;
    string cliente;
    string ip;
    int porta;
    float vazao;
    float consumo;
    bool bloqueado;
}historico_consumo;

// Métodos de configuração da comunicação
int configurarServidor(int port);   // configura servidor UDP
int configurarCliente(int port, string ip);    // cria cliente TCP 
int enviarMensagem(char *msg,char* buffer);      // envioa mensagem para servidor TCP

// Métodos para execução da thread
void *executarServidor(void *arg);
void *comunicacao(void *arg);
void *interface(void *arg);
//Métodos de operação no servidor
void atualizarConsumo(char* dados);     //atualiza consumo de hidrometro
void inserirDados(historico_consumo *data, char* msg);  // processa uma mensagem e insere os dados em uma struct
int bloquearHidrometro(int id);
int desbloquearHidrometro(int id);
void visualizarConsumo(int id);
void listarHidrometros();