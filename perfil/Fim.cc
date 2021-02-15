#include <string.h>
#include <omnetpp.h>
#include <math.h>

#include "Aluno.h"


using namespace omnetpp;

class Fim : public cSimpleModule {
  private:
    std::vector<Aluno *> lista;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

Define_Module(Fim);

void Fim::initialize() {

}

void Fim::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno *>(msg);
    lista.push_back(aluno);
}

void Fim::finish(){
    EV << "\n Total de alunos formados: "<< lista.size() << endl;

}




