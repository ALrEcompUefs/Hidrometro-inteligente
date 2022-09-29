#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#define MY_PORT 79799
#define MAX 1024 
//docker build -t n19alisson/hidrometro ./
typedef int ID;
typedef int TIPO_REQUISICAO;
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
int configurarServidor();   // configura servidor UDP
int configurarCliente(int port, string ip,char *msg);    // cria cliente TCP 
void enviarMensagem(int tipo,int id);      // envioa mensagem para servidor TCP

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
void gerarBoleto(int id);
