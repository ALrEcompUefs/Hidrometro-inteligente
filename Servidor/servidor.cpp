// Servidor
// Implementado rotina de um servidor TCP/IP para atender requisições de clientes
// Implementação baseada nas seguintes produções

#include <iostream>
#include <map>
#include <pthread.h>
#include "servidor.h"

using namespace std;
/* 
|	variaveis para comunicação via sockets
|	globais para facilitar a utilização
*/
	// Servidor UDP
	int sock_udp;
	struct sockaddr_in serv_udp_addr, cliente_udp_addr;

	// Cliente TCP
	int socket_cliente = 0;   //Descritor de arquivo do servidor
    int pt_cliente;
    int valread;    //valor lido
	struct sockaddr_in end_servidor;    //struct para manipular enderecos usados no protocolo
/*
|	Outras variaveis globais
*/
map<int,historico_consumo> hidrometros;
// para compilar g++ servidor.cpp -o servidor -l pthread
int main(int argc, char const* argv[]){
	pthread_t t1,t2;
		
		pthread_create(&t1,NULL,&executarServidor,NULL);
		pthread_create(&t2,NULL,&comunicacao,NULL);


		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
	return 0;
}

int configurarServidor(int port){
	// Creating socket file descriptor
	sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
	if ( sock_udp < 0 ) {
		printf("Erro na criacao do socket servidor");
		return -1;
	}
		
	bzero(&serv_udp_addr, sizeof(serv_udp_addr));
	bzero(&cliente_udp_addr, sizeof(cliente_udp_addr));
		
	// Filling server information
	serv_udp_addr.sin_family = AF_INET; // IPv4
	serv_udp_addr.sin_addr.s_addr = INADDR_ANY;
	serv_udp_addr.sin_port = htons(MY_PORT);
		
	// Bind the socket with the server address
	if ( bind(sock_udp, (const struct sockaddr *)&serv_udp_addr,sizeof(serv_udp_addr)) < 0 ){
		printf("Erro de vinculacao\n");
		return -1;
	}
	
	return 1;
}

void *executarServidor(void *arg){
	int len, n;
	char buffer[MAX];

		configurarServidor(8080);
		len = sizeof(cliente_udp_addr);
		while(true){
			n = recvfrom(sock_udp, (char *)buffer, MAX,MSG_WAITALL, ( struct sockaddr *) &cliente_udp_addr, (socklen_t*) &len);
			buffer[n] = '\0';
			//printf("Client : %s\n", buffer);
			atualizarConsumo(buffer);
		}
	pthread_exit(NULL);
}

void *comunicacao(void *arg){
	char buffer[MAX];
	char *msg = "bloquear hidrometo";
		
		while(true){
			//configurarCliente(4785);
			//enviarMensagem(msg,buffer);
			sleep(30);
		}
	pthread_exit(NULL);
}
int configurarCliente(int port, string ip){
	 /*
    |   Cria o socket e retorna o descritor do arquivo
	|	AF_INET processos conectados por ipv6 ou Ipv4
	|   AF_LOCAL processos posix no mesmo host
	|   SOCK_STREAM define comunicação orientada para conexão 
	|   SOCK_DGRAM define comunicação não orientada a conexão
    */
	if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Falha na criação do socket Cliente\n");
		return -1;
	}

    // Insere os endereços na struct 
	end_servidor.sin_family = AF_INET;      // Insere na struct
	end_servidor.sin_port = htons(port);    // traduz o inteiro sem sinal para  "network byte order" e insere na struct

	  // tradução do endereço IPV4 e IPV6 para string
	if (inet_pton(AF_INET, "127.0.0.1", &end_servidor.sin_addr) <= 0) {
		printf("Endereço invalido  Para o servidor TCP\n");
		return -1;
	}

	return 1;
}

int enviarMensagem(char *msg,char *buffer){
	// conecta o socket do cliente ao servidor
	if ((pt_cliente = connect(socket_cliente, (struct sockaddr*)&end_servidor, sizeof(end_servidor))) < 0) {
		printf("Falha na conexão com servidor TCP\n");
		return -1;
	}

    /*
    |   Envia mensagem ao Servidor e espera a resposta
    */
	send(socket_cliente, msg, strlen(msg), 0);
	printf("Bloqueio do Hidrometro solicitado\n");
	valread = read(socket_cliente, buffer, 1024);
	printf("%s\n", buffer);

	//Fecha conexão com o servidor
	close(pt_cliente);

	return 1;
} 

void atualizarConsumo(char* dados){
	historico_consumo data,hidro;

		// insere os dados da mensagem na struct
		inserirDados( &data,dados);
		if( hidrometros.find(data.id) == hidrometros.end()){
			// como o elemento não existe então insere um novo
			hidrometros.insert(make_pair(data.id,data));
		}
		else{	// se ja existir, então atualiza o consumo
			hidrometros[data.id].consumo = data.consumo;
		}
		hidro = hidrometros[data.id];
		printf("Id:%d Cliente:%s Consumo:%.2f Ip:%s Porta:%d\n",hidro.id,hidro.cliente.c_str()
				,hidro.consumo,hidro.ip.c_str() , hidro.porta);
}


void inserirDados(historico_consumo *data, char* msg){
	string str;
	int p1,p2;

		str += msg;
		// Separa o Id
		p1 = str.find("-");
		p2 = str.find("-",p1+1);
		data->id = stoi(str.substr(p1+1,p2-p1-1));
		//	Separa o CPF
		p1 = p2;
		p2 = str.find("-",p1+1);
		data->cliente =str.substr(p1+1,p2-p1-1);
		//	Separa o CEP
		p1 = p2;
		p2 = str.find("-",p1+1);
		data->cep = str.substr(p1+1,p2-p1-1);
		// Separa a vazao
		p1 = p2;
		p2 = str.find("-",p1+1);
		data->vazao = stof(str.substr(p1+1,p2-p1-1));
		// Separa o consumo
		p1 = p2;
		p2 = str.find("-",p1+1);
		data->consumo = stof(str.substr(p1+1,p2-p1-1));
		// separa o IP
		p1 = p2;
		p2 = str.find("-",p1+1);
		data->ip = str.substr(p1+1,p2-p1-1);
		// separa a porta
		p1 = p2;
		p2 = str.find("-",p1+1);
		data->porta = stoi(str.substr(p1+1,p2-p1-1));
}

int bloquearHidrometro(int id){
	historico_consumo hidro;
	char *msg ="bloquear";
	char buffer[MAX];

		if( hidrometros.find(id) == hidrometros.end() )
			return -1; // não existe o hidrometro cadastrado no sistema
		else{	
			hidro= hidrometros[id];
			/* 
			|	configura o cliente para enviar mensagem ao hidrometro para a porta e Ip especificados
			*/
			configurarCliente(hidro.porta,hidro.ip);
			// envia a mensagem
			enviarMensagem(msg,buffer);
			if( strcmp(buffer,"bloqueado") != 0)	// se não recebeu confirmação de bloqueio
				return 1;	
		}
	return 0;	// mensagem enviada com sucesso e hidrometro bloqueado
}

int desbloquearHidrometro(int id){
	historico_consumo hidro;
	char *msg ="desbloquear";
	char buffer[MAX];

		if( hidrometros.find(id) == hidrometros.end() )
			return -1; // não existe o hidrometro cadastrado no sistema
		else{	
			hidro= hidrometros[id];
			/* 
			|	configura o cliente para enviar mensagem ao hidrometro para a porta e Ip especificados
			*/
			configurarCliente(hidro.porta,hidro.ip);
			// envia a mensagem
			enviarMensagem(msg,buffer);
			if( strcmp(buffer,"desbloqueado") != 0)	// se não recebeu confirmação de desbloqueio
				return 1;	
		}
	return 0;	// mensagem enviada com sucesso e hidrometro bloqueado
}