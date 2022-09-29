#include "Hidrometro.h"
#include <string>
using namespace std;

Hidrometro::Hidrometro(int id,string cpf,string cep,float vazao){
	this->id =id;
	this->cpf_cliente=cpf;
	this->cep =cep;
	this->vazao =vazao;
	this->bloqueado =false;
	this->consumo = 0; 
	this->vazao_real =0;
}

void Hidrometro::exibeMensagem(){
	cout <<"id:"<<id <<"\ncliente:"
		 <<cpf_cliente << "\nCEP:" << cep << "\nvazao:" 
		 << vazao<<"\nIp:"<<ip<<"\n"<<"Bloqueado:"<<bloqueado<<"\n";
} 

void Hidrometro::setId(int id){
	this->id=id;
}

void Hidrometro::setCPF(string cpf){
	this->cpf_cliente= cpf;
}

void Hidrometro::setCep(string cep){
	this->cep=cep;
}
void Hidrometro::setVazao(float vazao){
	this->vazao=vazao;
}
void Hidrometro::setvazao_real(float vazao_real){
	 this->vazao_real = vazao_real;
}
void Hidrometro::bloquearHidrometo(){
	this->bloqueado= true;
}
void Hidrometro::desbloquearHidrometo(){
	this->bloqueado= false;
}

int Hidrometro::getId(){
	return this->id;
}
string Hidrometro::getCpfCliente(){
	return this->cpf_cliente;
}

string Hidrometro::getCep(){
	return this->cep;
}

float Hidrometro::getConsumo(){
	return this->consumo;
}

float Hidrometro::getVazao(){
	return this->vazao;
}

bool Hidrometro::estaBloqueado(){
	return this->bloqueado;
}

string Hidrometro::getIp(){
	return this->ip;
}

void Hidrometro::setIp(){
	this->ip = getHostIp();
}
void Hidrometro::setPorta(int porta){
	this->porta = porta;
}

int Hidrometro::getPorta(){
	return this->porta;
}

void Hidrometro::executarHidrometro(){
	float consumo_real = vazao - vazao_real;
		
		if( consumo_real < 0)
			consumo_real = 0;

		if(!this->bloqueado)	// se não estiver bloqueado, realiza o consumo
			this->consumo += consumo_real*10;
}

string Hidrometro::getMensagem(){
	string msg;
	return "-"+ to_string(id)
			+"-"+ this->cpf_cliente
			+"-"+ this->cep
			+"-"+ to_string(vazao)
			+"-"+ to_string(consumo)
			+"-"+ this->ip+"-"
			+to_string(this->porta) +"-";
}

string Hidrometro::getHostIp(){
	int fd; // ponteiro descritor de arquivo
    struct ifreq ifr;
	char *ccard = "eth0";
    char   endIP[22];
	string ip;
	
        endIP[0]=0;

        //cria o socket
        fd = socket(AF_INET, SOCK_DGRAM, 0);

        // Define o IPV4
        ifr.ifr_addr.sa_family = AF_INET;

        // Copia valor para struct
        strncpy(ifr.ifr_name, ccard, IFNAMSIZ-1);
        ioctl(fd, SIOCGIFADDR, &ifr);

        // fecha socket
        close(fd);

      // cria string com o endereço ip
      sprintf(endIP,"%s" , inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
      ip+= endIP;
	return ip;
}