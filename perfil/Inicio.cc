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
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarNovaMensagem();
    virtual void finish() override;
};

Define_Module(Inicio);

void Inicio::initialize() {
    countRaca = 0;
    indio = 0;
    enviarNovaMensagem();
}

void Inicio::handleMessage(cMessage *msg) {
    send(msg, "saida");
    enviarNovaMensagem();
}

void Inicio::enviarNovaMensagem() {
    SimTime time = simTime()+exponential(0.5);
    //std::string text = std::to_string(count++);
    //cMessage *postMsg = new cMessage(text.c_str());
    int rnum = std::rand();
    int nota =  rnum % 10+1;
    int numero = count++;

    Aluno *aluno = new Aluno(numero, "aluno "+std::to_string(numero), nota);
    //gera��o da raca
    countRaca++;
    indio++;
    int raca = 1;
    aluno->setEvadido(1);
    if(countRaca == 3){
        raca = 2;
        countRaca = 0;
        aluno->setEvadido(2);
    }
    if(indio == 10){
        raca = 3;
        indio = 0;
        aluno->setEvadido(2);
     }

    aluno->setRaca(raca);
    EV << "Enviando \"" << aluno->getNome() << "\"" << endl;
    racaStats.collect(1.0 * aluno->getEvadido());
    scheduleAt(time, aluno);
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


