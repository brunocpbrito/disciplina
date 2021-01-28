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
        EV << "Enviando \"" << aluno->getNome() << "\"" << endl;
        racaStats.collect(1.0 * aluno->getEvadido());
        scheduleAt(simTime()+1.0, aluno);
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


