#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class TerceiroPeriodo : public cSimpleModule {
  private:
    int capacidadeFila;
    cQueue fila;
    bool controle = false;
    int countEvasao;

    cHistogram faltasStats;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual Aluno * calculaEvasao(Aluno *aluno);
};

Define_Module(TerceiroPeriodo);

void TerceiroPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");
    countEvasao = 0;
}

void TerceiroPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno1 = dynamic_cast<Aluno *>(msg);
    Aluno *aluno2 = this->calculaEvasao(aluno1);
    faltasStats.collect(1.0 * aluno2->getEvadido());
    send(aluno2, "saida");
}

Aluno * TerceiroPeriodo::calculaEvasao(Aluno *aluno){
        int rnum = std::rand();
        int porcentagem =  rnum % 100+1;
        //Faltas
        if(porcentagem <= 20){
            aluno->setFaltas(aluno->getFaltas()+10);
            aluno->setNota(5);
            //adiciona valor a evasao para numero de falta grande
            aluno->setEvadido(aluno->getEvadido()+2);
        }else{
            aluno->setNota(10);
            aluno->setEvadido(aluno->getEvadido()+1);
        }
        //Relativo a quantidade de matriculas
        int rnumMatricula = std::rand();
        int porcentagemMatricula =  rnumMatricula % 100+1;
        if(porcentagemMatricula <= 20){
             aluno->setQtdMatriculas(1);
             aluno->setEvadido(aluno->getEvadido()+2);
        }else{
             aluno->setQtdMatriculas(0);
             aluno->setEvadido(aluno->getEvadido()+1);
        }

        return aluno;
}

void TerceiroPeriodo::finish(){
    EV << "\n Terceiro Periodo, indice de evasão" << endl;
    EV << "Muitas faltas (+2) com aumento de 10 faltas e notas 5, poucas faltas (+1) sem aumento com nota 10" << endl;
    EV << "Evasao, min:    " << faltasStats.getMin() << endl;
    EV << "Evasao, max:    " << faltasStats.getMax() << endl;
    EV << "Evasao, mean:   " << faltasStats.getMean() << endl;
    EV << "Evasao, stddev: " << faltasStats.getStddev() << endl;
    faltasStats.recordAs("Evasao");
}



