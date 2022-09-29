#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>
 #include <net/if.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
using namespace std;

class Hidrometro
{
	private:
		int id;				// indentificador unico do hidrometro
		string cpf_cliente;	// cpf do cliente dono do hidrometro
		string cep; 		// cep da localidade
		float consumo;		// consumo total do hidrometro
		float vazao; 		// vazao por segundo do hidrometro
		float vazao_real;	// vazamento de água presente
		bool bloqueado;		// estado do hidrometro
		string ip; 
		int porta;

	public:
		Hidrometro(int id,string cpf,string cep,float vazao);
		/// @brief Exibe atributos do hidrometro
		void exibeMensagem();
		
		/// @brief Altera o indentificador dohidrometro
		/// @param id 	novo id
		void setId(int id);
		/// @brief Altera o cpf do dono da Residencia do hidrometro
		/// @param cpf 	novo cpf
		void setCPF(string cpf);
		/// @brief Altera cep da residencia do hidrometro
		/// @param cep  novo cep
		void setCep(string cep);
		/// @brief Altera a vazão definida para o hidrometro
		/// @param vazao vazão atual
		void setVazao(float vazao); 
		/// @brief Altera o vazamento no hidrometro
		/// @param vazamento valor do vazamento
		void setvazao_real(float vazao_real); 
		/// @brief Bloqueia o hidrometro impedindo o fluxo de água
		void bloquearHidrometo();
		/// @brief Desbloqueia o hidrometro liberando o fluxo de água
		void desbloquearHidrometo();
		void setIp();
		void setPorta(int porta);

		string getIp();
		int getId();
		string getCpfCliente();
		string getCep();
		float getConsumo();
		float getVazao();
		float getvazao_real();
		bool estaBloqueado();
		/// @brief Método para executar a thread que simula o funcionamento do hidrometro
		void executarHidrometro();
		/// @brief Método para formatar mensagem a ser enviada com dados do hidrometro
		/// @return mensagem formatada 
		string getMensagem();
		string getHostIp();
		int getPorta();
};
