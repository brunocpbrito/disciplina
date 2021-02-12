#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"
#include <random>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>


using namespace omnetpp;

class Inicio : public cSimpleModule {
  private:
    int capacidadeFila;
    int countRaca;
    int indio;
    cQueue fila;
    int count = 0;
    cHistogram racaStats;
    int portaSaida = 0;
    bool controle =false;
    double i = 0.0;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarNovaTurma();
    virtual void finish() override;
    virtual double tempo();
};

Define_Module(Inicio);

void Inicio::initialize() {

    for (int var = 0; var < 10; ++var) {
        enviarNovaTurma();
    }
}

void Inicio::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno*>(msg);
    send(aluno, "saida", 1);

}

void Inicio::enviarNovaTurma() {
    Aluno *turma = new Aluno();
    turma->setNome("turma");
    send(turma, "saida", 1);
    //EV << "\n Enviando turmas de 10 alunos \n" << endl;
    for (int i = 0; i < 10; ++i) {
        int rnum = std::rand();
        int nota =  rnum % 10+1;
        int numero = ++count;

        Aluno *aluno = new Aluno(numero, "aluno "+std::to_string(numero), nota);
        aluno->setIngressante(true);
        //EV << "\n Enviando aluno \"" << aluno->getNumero()   << "\" como Ingressante. \n" << endl;
        //agenda o envio do aluno num tempo de 1 segundo. A ideia é sincronizar o tempo daqui com o tempo
        //primeiro periodo de modo a nao gerar filas.

        scheduleAt(simTime()+tempo(), aluno);
    }
    controle = false;

}

double Inicio::tempo(){
    double contagem = 0.0;
    int i = 10;
    if(count > i){
        contagem = contagem + 6.0;
        i = i +10;
        while(count > i){
            contagem = contagem + 6.0;
            i = i +10;
        }
    }

    return contagem;
}

void Inicio::finish(){
    EV << countRaca << endl;
}


