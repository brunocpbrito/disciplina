#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class PrimeiroPeriodo : public cSimpleModule {
  private:
    int capacidadeFila;
    cQueue fila;

    int portaSaida = 0;
    int portaEntrada = 0;

    double tempoProcessamento = 1;

    virtual void processar(Aluno *msg);
    virtual void colocarFila(Aluno *msg);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    double notaAleatoria(){
       double f = (double)rand() / RAND_MAX;
       return f * (100.0);
    };
};

Define_Module(PrimeiroPeriodo);

void PrimeiroPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");
}


void PrimeiroPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno *>(msg);
    EV << "Recebeu \"" << aluno->getNumero() << "\", status processamento: " << aluno->getProcessando() << ", status fila: "<< aluno->getEstaNaFila() << endl;

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

    // criei a variavel processando para n�o trabalhar com evadido
    if (aluno->getProcessando()) {

        // seta a nota aleatoria
        aluno->setNota(notaAleatoria());

        // se o aluno esta sendo processado e vai para o proximo periodo ou retornar para o mesmo periodo, processando e estaNaFila voltam a ser false
        aluno->setProcessando(false);
        aluno->setEstaNaFila(false);

        // deixei isso como estava
        aluno->setFaltas(0);
        aluno->setRaca(2);

        // se nota maior que 70, entra na porta saida que leva para o proximo periodo
        if (aluno->getNota() >= 70.0) {
            send(aluno, "saida", portaSaida);
        }
        // sen�o, entra na porta saida que leva para o periodo atual
        else {
            send(aluno, "saida", portaSaida + 40);
        }


        if (fila.isEmpty()) {
            Aluno *proximo = check_and_cast<Aluno *>(fila.pop());
            processar(proximo);
        }
    // se o aluno est� na fila, manda processar
    } else if (aluno->getEstaNaFila()) {
        processar(aluno);
    // sen�o est� processando, nem est� na fila, tenta colocar na fila
    } else {
        colocarFila(aluno);
    }
}


void PrimeiroPeriodo::processar(Aluno *aluno) {
    simtime_t tempoServico = exponential(tempoProcessamento);
    EV << "Processando \"" << aluno->getNumero() << "\" por " << tempoServico << "s." << endl;
    // aluno est� processsanod, seta processando como true e agenda o envio
    aluno->setProcessando(true);
    scheduleAt(simTime()+1, aluno);
}


void PrimeiroPeriodo::colocarFila(Aluno *aluno) {
    if (capacidadeFila > 0 && fila.getLength() == capacidadeFila) {
        EV << "Descartando \""<< aluno->getNumero() << "\". Motivo: fila cheia (#fila: " << capacidadeFila << "." << endl;
        delete aluno;
    } else {
        // aluno adicionado na fila, seta estaNaFila para true
        aluno->setEstaNaFila(true);
        fila.insert(aluno);
        EV << "Colocando \"" << aluno->getNumero() << "\" na fila (#fila: " << fila.getLength() << ")." << endl;
    }
}
