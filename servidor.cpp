// Servidor
// Implementado rotina de um servidor TCP/IP para atender requisições de clientes
// Implementação baseada nas seguintes produções


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080	// PORTA DO SERVDOR

// para compilar g++ servidor.cpp -o servidor
int main(int argc, char const* argv[])
{
	int pt_servidor;	//Descritor de arquivo do servidor
	int novo_socket;	//Descritor de arquivo de um novo socket
	int leitura;		//valor lido
	struct sockaddr_in endereco;	//struct para manipular enderecos usados no protocolo
	int opt = 1;	//configuração da operação de 
	int addrlen = sizeof(endereco);	//tamanho do endereço em bytes
	char buffer[1024] = { 0 };		// buffer para mensagem string
	char* msg = "Ola sou o servidor";

		/*
		|	Cria o socket e retorna o descritor do arquivo
		|	AF_INET processos conectados por ipv6 ou Ipv4
		|   AF_LOCAL processos posix no mesmo host
		|   SOCK_STREAM define comunicação orientada para conexão 
		|   SOCK_DGRAM define comunicação não orientada a conexão
		*/

		pt_servidor = socket(AF_INET, SOCK_STREAM, 0);
		if ( pt_servidor== 0) {
			printf("Falha na criação do socket\n");
			exit(EXIT_FAILURE);
		}

		bzero(&endereco, sizeof(endereco)); // zera espaço de mémoria para struct do endereço

		/*  
		|	Força a conexão do servidor na porta definida
		|	pt_servidor é o socket mestre do sevidor
		*/
		if (setsockopt(pt_servidor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
			printf("Erro na conexão\n");
			exit(EXIT_FAILURE);
		}

		/*
		|	Insere os parametros do enderço na struct
		*/
		endereco.sin_family = AF_INET;			// Insere na struct
		endereco.sin_addr.s_addr = INADDR_ANY;	// Insere na struct
		endereco.sin_port = htons(PORT);		// traduz o inteiro sem sinal para  "network byte order" e insere na struct

		/*
		|	Vincula o servidor a porta definida, de modo forçado
		*/ 
		if (bind( pt_servidor , (struct sockaddr*)&endereco, sizeof(endereco)) < 0) {
			printf("Falha na vinculação\n");
			exit(EXIT_FAILURE);
		}
		printf("\n Servidor vinculado a porta %d",PORT);
		/*
		|	Nesta etapa a configuração do servidor ja foi feita e ele pode ser colocado em operação
		*/
		printf("\n----Servidor Iniciado----\n---- Aguardando conexões ---\n");

		// Coloca o servidor em modo de escuta esperando conexões	
		if (listen(pt_servidor, 5) < 0) {
			printf("Erro na lista de espera\n");
			exit(EXIT_FAILURE);
		}

		while(1){
			
			// Quando uma conexão é solicitada 
			if ((novo_socket = accept(pt_servidor, (struct sockaddr*)&endereco, (socklen_t*)&addrlen)) < 0) {
				printf("Erro de conexão com socket cliente\n");
				exit(EXIT_FAILURE);
			}
			// Com a conexão feita agora realiza a leitura da mensagem enviada pelo cliente
			leitura = read(novo_socket, buffer, 1024);	// lê a mensagem e armazena no buffer
			printf("%s\n", buffer);	// exibe mensagem recebida
			bzero(buffer, sizeof(buffer)); // zera espaço de mémoria para struct do endereço
			send(novo_socket, msg, strlen(msg), 0);	// responde solicitação do cliente
			printf("Hello message sent\n");
			
			// Fecha conexão com o socket
			close(novo_socket);
			// encerra o servidor
		}
		
	shutdown(pt_servidor, SHUT_RDWR);
	return 0;
}
