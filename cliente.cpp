/*  Cliente
|   Implementa a rotina para um cliente em um protocolo TCP/IP
|   Implementação baseada nas seguintes produções
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080   // PORTA a ser usada

int main(int argc, char const* argv[])
{
	int sock = 0;   //Descritor de arquivo do servidor
    int client_fd;
    int valread;    //valor lido
	struct sockaddr_in end_servidor;    //struct para manipular enderecos usados no protocolo
	char* msg = "Ola sou o cliente";    //mensagem
	char buffer[1024] = { 0 };          // buffer para leitura 

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

    // Finalizada a configuração do socket o cliente pode se comunicar

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

	//Fecha conexão com o servidor
	close(client_fd);
	return 0;
}
