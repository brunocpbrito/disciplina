#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class Especifica : public cSimpleModule {
  private:
    int capacidadeFila;
    cQueue fila;
    bool controle = false;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual Aluno * processa(Aluno *msg);
};

Define_Module(Especifica);

void Especifica::initialize() {
    capacidadeFila = par("capacidadeFila");
}

void Especifica::handleMessage(cMessage *msg) {
        Aluno *postMsg = processa(dynamic_cast<Aluno *>(msg));
        int rnum = std::rand();
        if (controle) {
            cModule *modulo = postMsg->getSenderModule();
            send(postMsg, "saida");
        }
}

Aluno * Especifica::processa(Aluno *msg){
    Aluno *postMsg = new Aluno;
        if (fila.isEmpty() || fila.getLength() < capacidadeFila) {
            EV << " Nao enviar nada. Colocar na fila \n";
            EV << "Aluno de numero \""<< msg->getNome() << "\" inserida.\n";
            fila.insert(msg);
            EV << fila.getLength() << " tarefas na fila.\n";
            controle = false;
        } else {
            bubble("Tarefa trocada, Fila cheia");
            refreshDisplay();
            fila.insert(msg);
            postMsg = dynamic_cast<Aluno *>(fila.pop());
            bubble("Tarefa trocada, Fila cheia");
            refreshDisplay();
            EV << "Havera troca de alunos \""<< postMsg->getNome() << "\".\n";
            EV << "Aluno de numero \""<< postMsg->getNome() << "\" sera enviado com nota \""<< postMsg->getNota();
            controle = true;
        }
       return postMsg;
}



