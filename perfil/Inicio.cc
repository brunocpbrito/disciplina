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
}

void Inicio::handleMessage(cMessage *msg) {
    if (portaSaida == gateSize("saida") - 1) {
        portaSaida = 0;
        enviarNovaTurma();
    } else {
        portaSaida++;
    }

    send(msg, "saida", portaSaida);

}

void Inicio::enviarNovaTurma() {
    Aluno * turma = new Aluno();
            turma->setNome("turma");
            scheduleAt(simTime()+tempo(), turma);
    for (int i = 0; i < gateSize("saida"); i++) {
        int rnum = std::rand();
        int nota =  rnum % 10+1;
        int numero = count++;
        SimTime time = simTime();
        Aluno *aluno = new Aluno(numero, "aluno "+std::to_string(numero), nota);
        //geração da raca
        countRaca++;
        indio++;
        int raca = 1;
        aluno->setEvadido(0);
        aluno->setFaltas(0);
        aluno->setRaca(2);
        aluno->setIngressante(true);
        EV << "Enviando aluno \"" << aluno->getNumero() << "\"" << endl;
        racaStats.collect(1.0 * aluno->getEvadido());

        //agenda o envio do aluno num tempo de 1 segundo. A ideia é sincronizar o tempo daqui com o tempo
        //primeiro periodo de modo a nao gerar filas.

        scheduleAt(simTime()+tempo(), aluno);
    }


}

double Inicio::tempo(){
    if(count > 20){
        //tempo contado como dias, a cada 20 alunos, o tempo aumenta para 60 dias para diminuir a fila de espera
        return 6.0;
    }else{
        //tempo relativo dias
        return 6.0;
    }
}

void Inicio::finish(){
    EV << "\n Inicio, indice de evasao" << endl;
    EV << "Negros e indios (+2), brancos (+1)" << endl;
    EV << "Evasao, min:    " << racaStats.getMin() << endl;
    EV << "Evasao, max:    " << racaStats.getMax() << endl;
    EV << "Evasao, mean:   " << racaStats.getMean() << endl;
    EV << "Evasao, stddev: " << racaStats.getStddev() << endl;
    racaStats.recordAs("Evasao");
}


