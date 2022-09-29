/*
|   Arquivo de cabeçalho para implementar socket cliente udp e socket servidor TCP
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTA_SERV 79799 
#define MAX 1024
// Métodos para configurar cliente UDP e envio de dados
int configurarCliente(int port);
void enviarMensagem(char *msg);

// Métodos para configurar Servidor TCP e recebimento de dados
int configurarServidor(int port);

// Métodos para execução de threads
void* executarHidrometro(void* arg);
void* executarServidor(void *arg);
void* comunicacao(void* arg);
void* interface(void* arg);