#include "Hidrometro.h"

Hidrometro::Hidrometro(int id,string cpf,string cep,float vazao){
	this->id =id;
	this->cpf_cliente=cpf;
	this->cep =cep;
	this->vazao =vazao;
	this->bloqueado =true;
	this->consumo = 0; 
}

void Hidrometro::exibeMenssagem(){
	cout << "id:"<<id <<"\ncliente:"<<cpf_cliente << "\ncep:" << cep << "\n vazao" << vazao;
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
