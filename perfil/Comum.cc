#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class Comum : public cSimpleModule {
  private:
    int capacidadeFila;
    cQueue fila;
    bool controle = false;
    int countEvasao;
    int countTotal;

    cHistogram filaStats;
    cOutVector filaVector;
    cHistogram tempoServicoStats;
    cOutVector tempoServicoVector;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual Aluno * processa(Aluno *msg);
    virtual void calculaEvasao(Aluno *aluno);
};

Define_Module(Comum);

void Comum::initialize() {
    capacidadeFila = par("capacidadeFila");
    countEvasao = 0;
    countTotal = 0;
}

void Comum::handleMessage(cMessage *msg) {
        Aluno *postMsg = processa(dynamic_cast<Aluno *>(msg));
        int rnum = std::rand();
        if(controle){
            EV << "Enviando aluno \""<< postMsg->getNome() << "\".\n";
            //veririca se o aluno ficará em evasão
            this->calculaEvasao(postMsg);
            //filaVector.record(1.0 * postMsg->getNota());
            filaStats.collect(1.0 * postMsg->getEvadido());
            send(postMsg, "saida", rnum % gateSize("saida"));
        }
}

Aluno * Comum::processa(Aluno *msg){

    Aluno *postMsg = new Aluno;
    if (fila.isEmpty() || fila.getLength() < capacidadeFila) {
        EV << " Nao enviar nada. Colocar na fila \n";
        EV << "Aluno de numero \""<< msg->getNome() << "\" inserida.\n";
        fila.insert(msg);
        EV << fila.getLength() << " tarefas na fila.\n";
        controle = false;
    } else {
        fila.insert(msg);
        postMsg = dynamic_cast<Aluno *>(fila.pop());
        EV << "Havera troca de alunos \""<< postMsg->getNome() << "\".\n";
        controle = true; //fila cheia, trocara o aluno da fila, e mandara o primeiro.

    }
    int tamanhoFila = fila.getLength();
    return postMsg;
}

void Comum::calculaEvasao(Aluno *aluno){
    countTotal++;
    if(aluno->getEvadido() >= 10){
        countEvasao++;
    }
}

void Comum::finish(){
    EV << "\n Quantidade de aluno evadidos" << endl;
    EV << "Total:    " << countEvasao << "\n" <<endl;
    EV << "Quantidade total" << endl;
    EV << "Total:    " << countTotal << "\n" <<endl;

}



