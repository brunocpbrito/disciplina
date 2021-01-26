#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class PrimeiroPeriodo : public cSimpleModule {
  private:
    int capacidadeFila;
    cQueue fila;
    bool controle = false;
    int countEvasao;
    int portaSaida = 0;
    int portaEntrada = 0;

    cHistogram faltasStats;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual Aluno * calculaEvasao(Aluno *aluno);
};

Define_Module(PrimeiroPeriodo);

void PrimeiroPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");
    countEvasao = 0;
}

void PrimeiroPeriodo::handleMessage(cMessage *msg) {

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


Aluno * PrimeiroPeriodo::calculaEvasao(Aluno *aluno){
    //relativo a faltas

        int rnum = std::rand();
        int porcentagem =  rnum % 100+1;
        if(porcentagem <= 20){
            aluno->setFaltas(10);
            //adiciona valor a evasao para numero de falta grande
            aluno->setEvadido(aluno->getEvadido()+2);
        }else{
            aluno->setFaltas(0);
            aluno->setEvadido(aluno->getEvadido()+1);
        }
     //relativo a notas
        int rnumNota = std::rand();
        int porcentagemNota =  rnumNota % 100+1;
        if(porcentagemNota <= 20){
            aluno->setNota(5);
            //adiciona valor a evasao para nota baixa
            aluno->setEvadido(aluno->getEvadido()+2);
        }else{
            aluno->setNota(10);
            aluno->setEvadido(aluno->getEvadido()+1);
        }

        return aluno;
}

void PrimeiroPeriodo::finish(){
    EV << "\n Primeiro periodo, indice de evasao" << endl;
    EV << "Muitas faltas (+2) com aumento de 10 faltas, poucas faltas (+1) sem aumento" << endl;
    EV << "Evasao, min:    " << faltasStats.getMin() << endl;
    EV << "Evasao, max:    " << faltasStats.getMax() << endl;
    EV << "Evasao, mean:   " << faltasStats.getMean() << endl;
    EV << "Evasao, stddev: " << faltasStats.getStddev() << endl;
    faltasStats.recordAs("Evasao");
}



