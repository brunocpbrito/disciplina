#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class QuintoPeriodo : public cSimpleModule {
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

Define_Module(QuintoPeriodo);

void QuintoPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");
    countEvasao = 0;
}

void QuintoPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno1 = dynamic_cast<Aluno *>(msg);
    Aluno *aluno2 = this->calculaEvasao(aluno1);
    faltasStats.collect(1.0 * aluno2->getEvadido());

    //contagem de evadidos
    if(aluno2->getEvadido() >= 10){
       countEvasao++;
    }
    send(aluno2, "saida");
}

Aluno * QuintoPeriodo::calculaEvasao(Aluno *aluno){
        int rnum = std::rand();
        int porcentagem =  rnum % 100+1;
        //trabalham
        if(porcentagem < 20){
            //trablha, negro e faltas em execesso
            if (aluno->getRaca() == 2 && aluno->getFaltas() > 19) {
                aluno->setEvadido(aluno->getEvadido()+3);
            //trabalha, negro e poucas faltas
            } else if (aluno->getRaca() == 2 && aluno->getFaltas() < 20) {
                aluno->setEvadido(aluno->getEvadido()+2);
            }else{
                //trabalha e nao é negro
                aluno->setEvadido(aluno->getEvadido()+1);
            }
            aluno->setFaltas(aluno->getFaltas()+10);
            //adiciona valor a evasao para numero de falta grande
        }else{
            //aluno->setFaltas(0);
            aluno->setEvadido(aluno->getEvadido()+1);
        }
        return aluno;
}

void QuintoPeriodo::finish(){
    EV << "\n Quinto Periodo" << endl;
    EV << "Trabalham: aluno negro com faltas +3, negros e poucas faltas +2, brancos +1" << endl;
    EV << "Evasao, min:    " << faltasStats.getMin() << endl;
    EV << "Evasao, max:    " << faltasStats.getMax() << endl;
    EV << "Evasao, mean:   " << faltasStats.getMean() << endl;
    EV << "Evasao, stddev: " << faltasStats.getStddev() << endl;
    EV << "  Quantidade de aluno evadidos" << endl;
    EV << "Total:    " << countEvasao << "\n" <<endl;
    faltasStats.recordAs("Evasao");
}



