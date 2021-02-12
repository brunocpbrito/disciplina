#include <string.h>
#include <omnetpp.h>
#include <math.h>

#include "Aluno.h"


using namespace omnetpp;

class Fim : public cSimpleModule {
  private:
    int capacidadeFila;
    cQueue fila;
    int count = 0;
    int countEvasao;
    int countTotal;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void calculaEvasao(Aluno *aluno);
    cHistogram filaStats;
    cHistogram statsEvasao;
};

Define_Module(Fim);

void Fim::initialize() {
    countEvasao = 0;
    countTotal = 0;
}

void Fim::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno *>(msg);
    EV << "final vindo do  \""<< aluno->getNome() << " teve nota \"" << aluno->getNota() << " \n";
    this->calculaEvasao(aluno);
    filaStats.collect(1.0 * aluno->getRaca());
    statsEvasao.collect(1.0 * aluno->getEvadido());
}

void Fim::finish(){


}

void Fim::calculaEvasao(Aluno *aluno){
    countTotal++;
    if(aluno->getEvadido() >= 10){
        countEvasao++;
    }
}



