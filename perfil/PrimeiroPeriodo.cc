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

    Aluno *processando;

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

        //criado uma nova variavel para poder enviar.
        Aluno *alunoParaEnvio = new Aluno(processando->getNumero(), processando->getNome(), processando->getNota());

        //seta a nota aleatoria
        alunoParaEnvio->setNota(notaAleatoria());
        alunoParaEnvio->setRaca(2);

        // se nota maior que 70, entra na porta saida que leva para o proximo periodo
        if (alunoParaEnvio->getNota() >= 70.0) {
            EV << "Recebeu \"" << alunoParaEnvio->getNumero() << "\", status processamento: " << aluno->getProcessando() << " sendo enviado "<< endl;
            send(alunoParaEnvio, "saida", portaSaida);
        }
        // sen�o, entra na porta saida que leva para o periodo atual
        else {
            EV << "Recebeu \"" << alunoParaEnvio->getNumero() << "\", status processamento: " << aluno->getProcessando() << " sendo enviado "<< endl;
            //nesse trecho ele vai para porta 85 e dá erro, depois verificar, comentei por isso.
            //send(alunoParaEnvio, "saida", portaSaida + 40);
        }

        delete processando;
        if (fila.isEmpty()) {
             processando = nullptr;
        } else {
             processando = check_and_cast<Aluno *>(fila.pop());
              processar(processando);
            }
        } else if (!processando) {
               processando = aluno;
               processar(processando);
             //senao se estiver em analise, coloca o aluno que chegou na fila.
           } else {
               colocarFila(aluno);
           }
}



void PrimeiroPeriodo::processar(Aluno *aluno) {
    simtime_t tempoServico = exponential(tempoProcessamento);
    EV << "Processando \"" << aluno->getNumero() << "\" por " << tempoServico << "s." << endl;
    // aluno est� processsanod, seta processando como true e agenda o envio
    aluno->setProcessando(true);
    scheduleAt(simTime()+0.01, aluno);
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
