#include "Aluno.h"

Aluno::Aluno() {
    // TODO Auto-generated constructor stub

}

Aluno::~Aluno() {
    // TODO Auto-generated destructor stub
}

Aluno::Aluno(int numero, std::string nome, double nota) {
    this->setNumero(numero);
    this->setNome(nome);
    this->setNota(nota);
}

void Aluno::setNumero(int numero){
    this->numero = numero;
}

void Aluno::setNome(std::string nome){
    this->nome = nome;
}

void Aluno::setNota(int nota){
    this->nota = nota;
}

std::string Aluno::getNome(){
    return this->nome;
}

int Aluno::getNota(){
    return this->nota;
}

int Aluno::getRaca(){
    return this->raca;
}

void Aluno::setRaca(int raca){
    this->raca = raca;
}

int Aluno::getNumero(){
    return this->numero;
}

void Aluno::setEvadido(int evadido){
    this->evadido = evadido;
}

int Aluno::getEvadido(){
    return this->evadido;
}

void Aluno::setFaltas(int faltas){
    this->faltas = faltas;
}

int Aluno::getFaltas(){
    return this->faltas;
}

void Aluno::setQtdMatriculas(int qtde){
    this->qtdMatriculas = qtde;
}

int Aluno::getQtdMatriculas(){
    return this->qtdMatriculas;
}
