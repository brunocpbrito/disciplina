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

    Aluno *msgFimProcesso;
    Aluno *processando;
    double tempoProcessamento = 0.5;
    bool analisando;

    virtual void processar(Aluno *msg);
    virtual void colocarFila(Aluno *msg);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    virtual ~PrimeiroPeriodo() override;
};

Define_Module(PrimeiroPeriodo);

/**
 * O destrutor abaixo remove a mensagem de fim do processo.
 */
PrimeiroPeriodo::~PrimeiroPeriodo() {
    if (processando) {
        cancelAndDelete(processando);
        delete processando;
    }
    cancelAndDelete(msgFimProcesso);
}

void PrimeiroPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");
    analisando = false;
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
    EV << "Recebeu aluno \"" << aluno1->getNumero() << " sendo processado: "  << aluno1->getEvadido()<< "." << endl;

    //Se o aluno foi processado sera enviado.
    if (aluno1->getEvadido() != 0) {
            EV << "Fim do processamento de \"" << processando->getNumero() << "\"." << endl;

            //copia os dados do "processando" para poder enviar ao proximo periodo
            //Cabe aqui um trecho de codigo que vai definir se o aluno ira para o prox periodo,
            //ou se ira evadir de fato ou voltar ao mesmo periodo.
            //Ate entao todos os alunos processados estao indo para o proximo periodo.
            Aluno *alunoAprovado = new Aluno(processando->getNumero(), processando->getNome(), 10);
            alunoAprovado->setFaltas(0);
            alunoAprovado->setRaca(2);
            send(alunoAprovado, "saida", portaSaida);

            delete processando;
            if (fila.isEmpty()) {
                //caso a fila(turma) esteja vazia
                processando = nullptr;
            } else {
                //senao ja pega o proximo da fila(turma) para processar.
                processando = check_and_cast<Aluno *>(fila.pop());
                processar(processando);
        }
        //senao se aluno que chegou nao foi processado, mas a variavel processando está nula
        //significa que o aluno esta apto para processsar.
        } else if (!processando) {
            processando = aluno1;
            processar(processando);
          //senao se o aluno que chegou e a variavel processando nao esta nula, o aluno ira para a fila,
          //pois ainda esta no processando
        } else {
            colocarFila(aluno1);
        }
}

//O processar se dá apenas em converter o aluno 0 ou 1 no evadido e agendar seu envio ao handle message
// num tempo maior.
void PrimeiroPeriodo::processar(Aluno *msg) {
    simtime_t tempoServico = exponential(tempoProcessamento);
    EV << "Processando aluno \"" << msg->getNumero() << "\" por " << tempoServico << "s." << endl;
    //O processar significa colocar o aluno como evadido, e assim marca-lo como ja processado
    msg->setEvadido(1);
    //agenda o envio do aluno ao handleMessage com tempo de demora de 1 segundo
    scheduleAt(simTime()+1.0, msg);

}

void PrimeiroPeriodo::colocarFila(Aluno *msg) {
    if (capacidadeFila > 0 && fila.getLength() == capacidadeFila) {
        EV << "Descartando \""<< msg->getNumero() << "\". Motivo: fila cheia (#fila: " << capacidadeFila << "." << endl;
        delete msg;
    } else {
        //msg->setEvadido(1);
        fila.insert(msg);
        EV << "Colocando \"" << msg->getNumero() << "\" na fila (#fila: " << fila.getLength() << ")." << endl;
    }
}

