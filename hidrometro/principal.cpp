#include <iostream>
#include "Hidrometro.h"
#include "pthread.h"
#include "cliente.h"
using namespace std;

/*
|	Como uma Thread é utilizada para simular o consumo pelo o hidrometro,
|	o hidrometro é instanciado como variavel global a fim de facilitar
|	as operações na thread
*/
	Hidrometro hidro1(1,"08756487612","44010010",3.5);	

/* 
|	variaveis para comunicação via sockets
|	globais para facilitar a utilização
*/
	int sock = 0;   //Descritor de arquivo do servidor
	int client_fd;
	int valread;    //valor lido
	struct sockaddr_in end_servidor;    //struct para manipular enderecos usados no protocolo

// para compilar -g++ principal.cpp Hidrometro.cpp -o main -l pthread
int main(){
	// variavel para thread
	pthread_t t1,t2;

		pthread_create(&t1,NULL,&executarHidrometro,NULL);
		pthread_create(&t2,NULL,&comunicacao,NULL);

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);

		while(true);
	return 0;
}

int configurarCliente(int PORT){
		/*
		|   Cria o socket e retorna o descritor do arquivo
		|	AF_INET processos conectados por ipv6 ou Ipv4
		|   AF_LOCAL processos posix no mesmo host
		|   SOCK_STREAM define comunicação orientada para conexão 
		|   SOCK_DGRAM define comunicação não orientada a conexão
		*/
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("Falha na criação do socket\n");
			return -1;
		}

		// Insere os endereços na struct 
		end_servidor.sin_family = AF_INET;      // Insere na struct
		end_servidor.sin_port = htons(PORT);    // traduz o inteiro sem sinal para  "network byte order" e insere na struct

		// tradução do endereço IPV4 e IPV6 para string
		if (inet_pton(AF_INET, "127.0.0.1", &end_servidor.sin_addr) <= 0) {
			printf("Endereço invalido \n");
			return -1;
		}
	return 0;
}

int enviarMensagem(char *msg,char *buffer){
		// conecta o socket do cliente ao servidor
		if ((client_fd = connect(sock, (struct sockaddr*)&end_servidor, sizeof(end_servidor))) < 0) {
			printf("Falha na conexão\n");
			return -1;
		}

		/*
		|   Envia mensagem ao Servidor e espera a resposta
		*/
		send(sock, msg, strlen(msg), 0);
		printf("Hello message sent\n");
		valread = read(sock, buffer, 1024);
		printf("%s\n", buffer);
		buffer[0]='\0';	// limpa o buffer

		//Fecha conexão com o servidor
		close(client_fd);

	return 0;
}

/*
|	Procedimento que chama o método que executa o consumo do hidrometro
|	é um do tipo ponteiro void para ser passado como parametro pela 
|	função que executa a thread
*/
void* executarHidrometro(void* arg){
		while(true){
			hidro1.executarHidrometro();
			sleep(30);
		}
		pthread_exit(NULL);
}

void* comunicacao(void* arg){
	char buffer[1024];
	char *msg ="Boa noite";

		while(true){
			configurarCliente(8080);	
			enviarMensagem(msg,buffer);
			sleep(10);
			msg="Bom dia agora";
			configurarCliente(8080);
			enviarMensagem(msg,buffer);
		}

	pthread_exit(NULL);
}
