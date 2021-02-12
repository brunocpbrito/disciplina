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
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarNovaTurma();
    virtual void finish() override;
    virtual double tempo();
};

Define_Module(Inicio);

void Inicio::initialize() {
    countRaca = 0;
    indio = 0;
    enviarNovaTurma();
    enviarNovaTurma();
    enviarNovaTurma();

}

void Inicio::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno*>(msg);
    send(aluno, "saida", 1);

}

void Inicio::enviarNovaTurma() {
    Aluno *turma = new Aluno();
    turma->setNome("turma");
    send(turma, "saida", 1);
    EV << "\n Enviando turmas de 10 alunos \n" << endl;
    for (int i = 0; i < gateSize("saida"); i++) {
        int rnum = std::rand();
        int nota =  rnum % 10+1;
        int numero = count++;
        SimTime time = simTime();
        Aluno *aluno = new Aluno(numero, "aluno "+std::to_string(numero), nota);
        //geração da raca
        indio++;
        countRaca++;
        int raca = 1;
        aluno->setEvadido(0);
        aluno->setFaltas(0);
        aluno->setRaca(2);
        aluno->setIngressante(true);

        racaStats.collect(1.0 * aluno->getEvadido());

        //agenda o envio do aluno num tempo de 1 segundo. A ideia é sincronizar o tempo daqui com o tempo
        //primeiro periodo de modo a nao gerar filas.

        scheduleAt(simTime()+tempo(), aluno);
    }
    controle = false;

}

double Inicio::tempo(){
    double contagem = 0.0;
    if(count >= 11){
        contagem = contagem + 6.0;
    }
    if(count >= 21){
        contagem = contagem + 6.0;
    }
    if(count >= 31){
            contagem = contagem + 6.0;
       }
    return contagem;
}

void Inicio::finish(){
    EV << countRaca << endl;
}


