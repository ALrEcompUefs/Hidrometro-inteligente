#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

class Hidrometro
{
	private:
		int id;				// indentificador unico do hidrometro
		string cpf_cliente;	// cpf do cliente dono do hidrometro
		string cep; 		// cep da localidade
		float consumo;		// consumo total do hidrometro
		float vazao; 		// vazao por segundo do hidrometro
		float vazamento;	// vazamento de água presente
		bool bloqueado;		// estado do hidrometro 
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
		void setVazamento(float vazamento); 
		/// @brief Bloqueia o hidrometro impedindo o fluxo de água
		void bloquearHidrometo();
		/// @brief Desbloqueia o hidrometro liberando o fluxo de água
		void desbloquearHidrometo();
		
		int getId();
		string getCpfCliente();
		string getCep();
		float getConsumo();
		float getVazao();
		float getVazamento();
		bool estaBloqueado();
		/// @brief Método para executar a thread que simula o funcionamento do hidrometro
		void executarHidrometro();
};
