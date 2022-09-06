/*
|   Arquivo de cabeçalho para implementar um socket cliente
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int configurarCliente(int PORT);
int enviarMensagem(char *msg,char *buffer);
void* executarHidrometro(void* arg);
void* comunicacao(void* arg);
