#include <iostream>
#include "Hidrometro.h"
#include "pthread.h"
#include "comunicacao.h"
#include <cstring>
using namespace std;

char *IP_SERV;
bool COMUNICACAO_HABILITADA = false;
/*
|	Como uma Thread é utilizada para simular o consumo pelo o hidrometro,
|	o hidrometro é instanciado como variavel global a fim de facilitar
|	as operações na thread
*/
	Hidrometro hidro1(0,"0000000","44010010",1.0);	

//Variavéis globais para o cliente UDP
    int sock_udp;
    struct sockaddr_in serv_udp_addr;
	
/* 
|	variaveis para comunicação via sockets
|	globais para facilitar a utilização
*/
	int pt_servidor;	//Descritor de arquivo do servidor
	int novo_socket;	//Descritor de arquivo de um novo socket
	int leitura;		//valor lido
	struct sockaddr_in endereco;	//struct para manipular enderecos usados no protocolo
	int opt = 1;	//configuração da operação de 
	int addrlen = sizeof(endereco);	//tamanho do endereço em bytes

// para compilar - g++ principal.cpp Hidrometro.cpp -o main -l pthread
// para executar ./main ip_servidor porta_hidrometro id CPF CEP
int main(int argc , char *argv[]){
	// variavel para thread
	pthread_t t1,t2,t3,t4;

		// a partir dos argumentos de linha de comando define o ip do servidor
		IP_SERV = argv[1];

		// Obtém e define o endereço ip do hidrometro
		hidro1.setIp();
		hidro1.setId(atoi(argv[3]));
		hidro1.setCPF(argv[4] );
		hidro1.setCep(argv[5]);
		hidro1.setPorta( atoi(argv[2]) );


		pthread_create(&t1,NULL,&executarHidrometro,NULL);	// Atualziação do consumo
		pthread_create(&t2,NULL,&comunicacao,NULL);			// Envio dos dados UDP
		pthread_create(&t3,NULL,&executarServidor,NULL);	// Recebe as requisições TCP
		pthread_create(&t4,NULL,&interface,NULL);
		
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);
		pthread_join(t4, NULL);

		while(true);
	return 0;
}

void* interface(void* arg){
	char opt;
	double aux;

		while(true){
			printf("--Interface do Hidrometro--\n");
			printf("[1] Alterar vazao\n[2] Alterar Vazamento\n[3] Habilitar Comunicacao\n[4] Desabilitar Comunicacao \n[5] Exibir dados\n");
			printf("Escolha Opcao:");
			scanf("%c",&opt);
			getchar();
			switch (opt){
				case '1':	// Alterar vazao
					printf("Informe a Vazao:");
					cin>>aux;
					getchar();
					hidro1.setVazao(aux);
					
					break;
				case '2':	// 	alterar vazamento
					printf("Informe o vazamento:");
					cin>>aux;
					getchar();
					hidro1.setVazamento(aux);
					break;
				case '3':	// habilitar comunicacao
					COMUNICACAO_HABILITADA = true;
					printf("Comunicacao habilitada\n");
					getchar();
					break;
				case '4':	// habilitar comunicacao
					COMUNICACAO_HABILITADA = false;
					printf("Comunicacao desabilitada\n");
					getchar();
					break;
				case '5':	// exibir dados
					printf("\nDados do Hidrometro\n");
					hidro1.exibeMensagem();
					getchar();
					break;
				case '6':
					break;
				default:
					system("clear");
					cout<<"\n DIGITE APENAS numeros de 0-9!!!\n";
					break;
				
			}
			system("clear");
		}
}
int configurarCliente(int port){

		/* 
		|   Cria descritor de arquivo para o socket definindo o
		|   protocolo de transporte como UDP
		*/
		sock_udp = socket(AF_INET, SOCK_DGRAM, 0);

		// se o descritor tiver um valor negativo ocorreu um erro
		if ( sock_udp< 0 ) {    
			printf("erro na criação do socket");
			return -1;
		}
		
		// Limpa area de mémoria para struct do endereço 
		bzero(&serv_udp_addr, sizeof(serv_udp_addr));

		//  Insere os parametros para o endereço na struct
		serv_udp_addr.sin_family = AF_INET;
		serv_udp_addr.sin_port = htons(PORTA_SERV);
		serv_udp_addr.sin_addr.s_addr = inet_addr(IP_SERV );
		
    return 1;
}

void enviarMensagem(char *msg){
    // envia mensagem para o servidor sem espera resposta de confirmação
    sendto( sock_udp, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *) &serv_udp_addr, sizeof( serv_udp_addr ));
}

int configurarServidor(int port){
        /*
		|	Cria o socket e retorna o descritor do arquivo
		|	AF_INET processos conectados por ipv6 ou Ipv4
		|   AF_LOCAL processos posix no mesmo host
		|   SOCK_STREAM define comunicação orientada para conexão 
		|   SOCK_DGRAM define comunicação não orientada a conexão
		*/

		pt_servidor = socket(AF_INET, SOCK_STREAM, 0);
		if ( pt_servidor == 0) {
			printf("Falha na criação do socket\n");
			return -1;
		}

		bzero(&endereco, sizeof(endereco)); // zera espaço de mémoria para struct do endereço

		/*  
		|	Força a conexão do servidor na porta definida
		|	pt_servidor é o socket mestre do sevidor
		*/
		if (setsockopt(pt_servidor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
			printf("Erro na conexão\n");
			return -1;
		}

		/*
		|	Insere os parametros do enderço na struct
		*/
		endereco.sin_family = AF_INET;			// Insere na struct
		endereco.sin_addr.s_addr = INADDR_ANY;	// Insere na struct
		endereco.sin_port = htons(port);		// traduz o inteiro sem sinal para  "network byte order" e insere na struct

		/*
		|	Vincula o servidor a porta definida, de modo forçado
		*/ 
		if (bind( pt_servidor , (struct sockaddr*)&endereco, sizeof(endereco)) < 0) {
			printf("Falha na vinculação\n");
			return -1;
		}
		
		/*
		|	Nesta etapa a configuração do servidor ja foi feita e ele pode ser colocado em operação
		*/
		//printf("\n Servidor vinculado a porta %d",port);
		//printf("\n----Servidor Iniciado----\n---- Aguardando conexões ---\n");

		// Coloca o servidor em modo de escuta esperando conexões	
		if (listen(pt_servidor, 15) < 0) {
			printf("Erro na lista de espera\n");
			return -1;
		}

    return 1;
}

/*
|	Procedimento que chama o método que executa o consumo do hidrometro
|	é um do tipo ponteiro void para ser passado como parametro pela 
|	função que executa a thread
*/
void* executarHidrometro(void* arg){
		while(true){
			hidro1.executarHidrometro();
			sleep(10);
		}
		pthread_exit(NULL);
}

void* comunicacao(void* arg){
	char buffer[MAX];
	string txt;
	char *msg = new char [MAX];

		
		while(true){
			
			// Se o hidrometro não esta bloqueado e a comunicacao habilitada, Envia a mensagem
			if(COMUNICACAO_HABILITADA && !hidro1.estaBloqueado()){
				// obtém a mensagem
				txt = hidro1.getMensagem();
				strcpy(msg,txt.c_str());

				// envia mensagem
				configurarCliente(PORTA_SERV);	
				enviarMensagem(msg);
			
			}
			sleep(10);
		}

	pthread_exit(NULL);
}

void* executarServidor(void *arg){
	char buffer[MAX];
	char *msg = "Hidrometo bloqueado!!";

	configurarServidor( hidro1.getPorta());
	while(1){
			// Quando uma conexão é solicitada 
			if ((novo_socket = accept(pt_servidor, (struct sockaddr*)&endereco, (socklen_t*)&addrlen)) < 0) {
				printf("\nErro de conexão com socket cliente\n");
				exit(EXIT_FAILURE);
			}
			// Com a conexão feita agora realiza a leitura da mensagem enviada pelo cliente
			leitura = read(novo_socket, buffer, 1024);	// lê a mensagem e armazena no buffer
			printf("%s\n", buffer);	// exibe mensagem recebida
			
			// Bloqueia o Hidrometro
			hidro1.bloquearHidrometo();
			// Aqui vai ter de bloquear o hidrometro e confirmar depois

			bzero(buffer, sizeof(buffer)); // zera espaço de mémoria para struct do endereço
			send(novo_socket, msg, strlen(msg), 0);	// responde solicitação do cliente
			
			// Fecha conexão com o socket
			close(novo_socket);
		}
	shutdown(pt_servidor, SHUT_RDWR);
}