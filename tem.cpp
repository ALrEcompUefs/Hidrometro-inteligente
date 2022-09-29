// Servidor
// Implementado rotina de um servidor TCP/IP para atender requisições de clientes
// Implementação baseada nas seguintes produções

#include <iostream>
#include <map>
#include <queue>
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
/*
|	Outras variaveis globais
*/
	map<int,historico_consumo> hidrometros;	 // Guarda os hidrometros cadastrados
	queue<pair<ID,TIPO_REQUISICAO>> fila;	//Fila para requisições de comunicação com hidrometros

// para compilar g++ servidor.cpp -o servidor -l pthread
// para executar ./servidor
int main(int argc, char const* argv[]){
	pthread_t t1,t2,t3;

		pthread_create(&t1,NULL,&interface,NULL);
		pthread_create(&t2,NULL,&executarServidor,NULL);
		pthread_create(&t3,NULL,&comunicacao,NULL);
		
		pthread_join(t1,NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);
		
	return 0;
}

int configurarServidor(){
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

void *interface(void *arg){
	char opt;
	int valor;
		while (true){
			system("clear");
			printf("--- Servidor ---\n");
			printf("[1] Listar Hidrometros\n[2] Visualizar Consumo \n[3] Bloquear Hidrometro\n[4] Desbloquear Hidrometro\n");
			printf("Escolha:");
			scanf("%c",&opt);
			getchar();
			switch (opt){
				case '1':
					system("clear");
					listarHidrometros();
					break;
				case '2':
					system("clear");
					printf("Informe o id:");
					cin>>valor;
					getchar();
					visualizarConsumo(valor);
					break;
				case '3':
					system("clear");
					printf("Informe o id:");
					cin>>valor;
					getchar();
					fila.push(make_pair(valor,1)); //cria par de solicitação
					break;
				case '4':
					system("clear");
					printf("Informe o id:");
					cin>>valor;
					getchar();
					fila.push(make_pair(valor,2)); //cria par de solicitação
					break;
				default:
					system("clear");
					printf("INFORME APENAS DIGITOS DE 0-9!!\n");
					break;
			}
			getchar();	// aguarda entrada do teclado para continuar
		}
	pthread_exit(NULL);
}

void *executarServidor(void *arg){
	int len, n;
	char buffer[MAX];
		configurarServidor(MY_PORT);
		len = sizeof(cliente_udp_addr);
		while(true){
			n = recvfrom(sock_udp, (char *)buffer, MAX,MSG_WAITALL, ( struct sockaddr *) &cliente_udp_addr, (socklen_t*) &len);
			buffer[n] = '\0';
			//printf("Client : %s\n", buffer);
			atualizarConsumo(buffer);
		}
	pthread_exit(NULL);
}

/**
 *	Método para execução da thread de comunicação 
 * 	Na comunicação o servidor abre um socket cliente que envia uma menssagem para um hidrometro
 * 	Dois tipos de mensagem são enviadas, de bloqueio ou de desbloqueio
 *	Existe uma fila com as requisições para um hidrometro que contém o tipo e id do hidrometro
 */
void *comunicacao(void *arg){
	pair<ID,TIPO_REQUISICAO> requisicao;// para de uma requisição
		while(true){

			if(!fila.empty()){
				requisicao = fila.front();	//pega o primeiro elemento
				fila.pop(); 	// remove o primeiro elemento
				// agora envia mensagem de bloqueio ou desbloqueio para o hidrometro

				// se a operação for do tipo é um solicitação de bloqueio
				if(requisicao.second == 1){
					enviarMensagem(1,requisicao.first);
				}
				else{	// se for do tipo 2 é uma solicitação de desbloqueio
					enviarMensagem(2,requisicao.first);
				}
			}
		}
	pthread_exit(NULL);
}
/*
*	Cria um cliente TCP e faz o envio de uma mensagem
*	a Mensagem é uma solicitação de bloqueio ou desbloqueio de hidrometro
*/
int configurarCliente(int port, string ip,char *msg){
	// Cliente TCP
	int socket_cliente = 0;   //Descritor de arquivo do servidor
    int pt_cliente;
    int valread;    //valor lido
	struct sockaddr_in end_servidor;    //struct para manipular enderecos usados no protocolo
	char buffer[MAX];
        /*
        |   Cria o socket e retorna o descritor do arquivo
        |	AF_INET processos conectados por ipv6 ou Ipv4
        |   AF_LOCAL processos posix no mesmo host
        |   SOCK_STREAM define comunicação orientada para conexão 
        |   SOCK_DGRAM define comunicação não orientada a conexão
        */
        if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            //printf("Falha na criação do socket Cliente\n");
            return -1;
        }

        // Insere os endereços na struct 
        end_servidor.sin_family = AF_INET;      // Insere na struct
        end_servidor.sin_port = htons(port);    // traduz o inteiro sem sinal para  "network byte order" e insere na struct
        end_servidor.sin_addr.s_addr = inet_addr(ip.c_str());

        // tradução do endereço IPV4 e IPV6 para string
        if (inet_pton(AF_INET, "127.0.0.1", &end_servidor.sin_addr) <= 0) {
            //printf("Endereço invalido  Para o servidor TCP\n");
            return -1;
        }
        // conecta o socket do cliente ao servidor
            if ((pt_cliente = connect(socket_cliente, (struct sockaddr*)&end_servidor, sizeof(end_servidor))) < 0) {
                //printf("Falha na conexão com servidor TCP\n");
                return -1;
            }
            
            //Envia mensagem ao Servidor e espera a resposta		
            //send(socket_cliente, msg, strlen(msg), 0);
            //printf("Bloqueio do Hidrometro solicitado\n");
            //valread = read(socket_cliente, buffer, MAX);
            //Fecha conexão com o servidor
            close(pt_cliente);

	return 1;
}

/*
*	Atualiza o consumo de um historico de Hidrometro
*   recebe como parametro a mensagem do hidrometro
*/
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
}

/**
 * 	Separa os dados de uma string e insere na struct passada como argumento
 */
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


/*
|	Método que busca um hidrometro pelo id e exibe seu consumo se existir
*/
void visualizarConsumo(int id){

	if( hidrometros.find(id) == hidrometros.end()){
		printf("Hidrometro nao cadastrado\n");
	}
	else{
		printf("Cliente ");
		cout<< hidrometros[id].cliente<<"\n";
		printf("Consumo:%.3f\n",hidrometros[id].consumo);
	}
}
/*
|	Método que exibe os hidrometros salvos no servidor
*/
void listarHidrometros(){
	for (map<int,historico_consumo>::iterator it= hidrometros.begin(); it!= hidrometros.end(); it++){
		cout<<"Hidrometro:"<<it->second.id<<"	Cliente:"<<it->second.cliente<<"\n";
	}
}

void enviarMensagem(int tipo,int id){
	historico_consumo hidro;
		if( hidrometros.find(id) == hidrometros.end() ){
			//printf("Hidrometro nao cadastrado\n")
		}		
		hidro= hidrometros[id];  // obtém hidrometro com o id solicitado
		/*
		*	configura o cliente para enviar mensagem ao hidrometro para a porta e Ip especificados
		*	E Envia a mensagem para o hidrometro
		*/
	if(tipo == 1)
		configurarCliente(hidro.porta,hidro.ip,"bloquear");
	else
		configurarCliente(hidro.porta,hidro.ip,"desbloquear");
}
