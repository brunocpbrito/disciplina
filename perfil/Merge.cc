#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"
#include <iostream>
#include <list>
#include <algorithm>
#include <vector>


using namespace omnetpp;

class Merge : public cSimpleModule {
  private:
    int count;
    std::vector<Aluno *> lista;
    std::vector<Aluno *> colecao;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void verificarRepeticao(cMessage *msg);
  public:
    virtual void enviarAlunos(std::vector<Aluno *> colecao);
};

Define_Module(Merge);

void Merge::initialize() {
    count = 0;
}

void Merge::handleMessage(cMessage *msg) {

    verificarRepeticao(msg);

}

void Merge::verificarRepeticao(cMessage *msg){
    Aluno *aluno = check_and_cast<Aluno*>(msg);

    if(aluno->getNome() == "turma"){
        count++;
    }else{
        lista.push_back(aluno);
    }
    if(count == 2){
        EV << "Alunos aptos a cursar o Terceiro Perido por ter sido aprovado nas disciplinas A e B. " << endl;
        //verifica se ha alunos repetidos. Isso é importante para evitar alunos duplicados no envio.
        //para poder enviar um aluno, ele deve estar aprovado das duas disciplinas do Segundo Periodo
        for (int i = 0; i < lista.size(); ++i){
            Aluno * procurado = lista[i];

            for (int j = i+1; j < lista.size(); ++j) {

                Aluno * encontrado = lista[j];
                if(procurado->getNumero() == encontrado->getNumero()){
                    EV << "Aluno "<< encontrado->getNumero()<< endl;
                    colecao.push_back(encontrado);
                    lista.erase(lista.begin()+j);
                    break;
                }
            }

        }
        //envia a colecao de alunos
        if(!colecao.empty()){
            enviarAlunos(colecao);
        }else{
            EV << "Não ha alunos para enviar ao Terceiro Periodo " << endl;
        }
        count = 0;
        lista.clear();
        colecao.clear();
    }
}

void Merge::enviarAlunos(std::vector<Aluno *> colecao){
    for (int var = 0; var < colecao.size(); ++var) {
        Aluno * aluno = colecao[var];
        send(aluno, "saida", 0);
    }
    Aluno *turma = new Aluno();
    turma->setNome("turma");
    EV << "\n !!Enviando alunos para o Terceiro Periodo.!! \n " << endl;
    //envia mensagem para criar nova turma no prox periodo
    send(turma, "saida", 0);
}



