#ifndef HIDROMETRO_H
#define HIDROMETRO_H
#include <iostream>
#include <string>
using namespace std;

class Hidrometro
{
	private:
		int id;				// indentificador unico do hidrometro
		string cpf_cliente;	// cpf do cliente dono do hidrometro
		string cep; 		// cep da localidade
		float consumo;		// consumo total do hidrometro
		float vazao; 		// vazao por segundo do hidrometro
		bool bloqueado;		// estado do hidrometro 
	public:
		Hidrometro(int id,string cpf,string cep,float vazao);
		void exibeMenssagem();
		void setId(int id);
		void setCPF(string cpf);
		void setCep(string cep);
		void setVazao(float vazao); 
		void bloquearHidrometo();
		void desbloquearHidrometo();
		
		int getId();
		string getCpfCliente();
		string getCep();
		float getConsumo();
		float getVazao();
		bool estaBloqueado();
};

#endif
