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
    this->setProcessando(false);
}

void Aluno::setNumero(int numero){
    this->numero = numero;
}

void Aluno::setNome(std::string nome){
    this->nome = nome;
}

void Aluno::setNota(double nota){
    this->nota = nota;
}

void Aluno::setProcessando(bool processando){
    this->processando = processando;
}

void Aluno::setIngressante(bool ingressante){
    this->ingressante = ingressante;
}

std::string Aluno::getNome(){
    return this->nome;
}

double Aluno::getNota(){
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

bool Aluno::getProcessando(){
    return this->processando;
}

bool Aluno::getIngressante(){
    return this->ingressante;
}
