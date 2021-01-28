#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class SextoPeriodo : public cSimpleModule {
  private:
    int capacidadeFila;
    cQueue fila;
    bool controle = false;
    int countEvasao;
    int portaSaida = 0;
    int portaEntrada = 0;

    Aluno *processando;
    cHistogram faltasStats;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual Aluno * calculaEvasao(Aluno *aluno);
};

Define_Module(SextoPeriodo);

void SextoPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");
    countEvasao = 0;
}

void SextoPeriodo::handleMessage(cMessage *msg) {

    if (portaSaida == gateSize("saida") - 1) {
        portaSaida = 0;
    } else {
        portaSaida++;
    }

    if (portaEntrada == gateSize("entrada") - 1) {
        portaEntrada = 0;
    } else {
        portaEntrada++;
    }

    Aluno *aluno1 = dynamic_cast<Aluno *>(msg);
    Aluno *aluno2 = this->calculaEvasao(aluno1);
    faltasStats.collect(1.0 * aluno2->getEvadido());

    send(aluno2, "saida", portaSaida);
}

Aluno * SextoPeriodo::calculaEvasao(Aluno *aluno){
        int rnum = std::rand();
        int porcentagem =  rnum % 100+1;
        //nota
        if(porcentagem <= 20){
            //faltas com nota baixa
            if (aluno->getNota() == 5 && aluno->getFaltas() > 19 && aluno->getQtdMatriculas()) {
                aluno->setEvadido(aluno->getEvadido()+4);
            } else if(aluno->getNota() == 5 && aluno->getFaltas() > 19){
                aluno->setEvadido(aluno->getEvadido()+3);
            }else if(aluno->getNota() == 5){
                aluno->setEvadido(aluno->getEvadido()+2);
            }else {
                aluno->setEvadido(aluno->getEvadido()+1);
            }
        }
        return aluno;
}

void SextoPeriodo::finish(){
    EV << "\n Sexto Periodo" << endl;
    EV << "nota baixa e muitas faltas +3, nota baixa +2, demais +1" << endl;
    EV << "Evasao, min:    " << faltasStats.getMin() << endl;
    EV << "Evasao, max:    " << faltasStats.getMax() << endl;
    EV << "Evasao, mean:   " << faltasStats.getMean() << endl;
    EV << "Evasao, stddev: " << faltasStats.getStddev() << endl;
    faltasStats.recordAs("Evasao");
}



