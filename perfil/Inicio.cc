#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"
#include <random>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>


using namespace omnetpp;

class Inicio : public cSimpleModule {
  private:
    int capacidadeFila;
    int countRaca;
    int indio;
    cQueue fila;
    int count = 0;
    cHistogram racaStats;
    int qtdeAlunos;
    int entradas;
    double t = 0.0;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarNovaTurma();
    virtual void finish() override;
    virtual double tempo();
};

Define_Module(Inicio);

void Inicio::initialize() {
    entradas = par("entradas");
    qtdeAlunos = par("qtdeAlunos");
    for (int var = 0; var < entradas; ++var) {
        enviarNovaTurma();
    }
}

void Inicio::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno*>(msg);
    send(aluno, "saida", 0);

}

/*
 * Metodo que envia uma turma com entradas e quantidade definidas.
 * Ao finao da ultima iteracao eh enviado uma mensagem de Turma, significando que o periodo
 * seguinte deve processar a turma enviada.
 */
void Inicio::enviarNovaTurma() {

    int cont = 0;

    for (int i = 0; i < qtdeAlunos; ++i) {
        int rnum = std::rand();

        int numero = ++count;

        Aluno *aluno = new Aluno(numero, "aluno "+std::to_string(numero), 0.0);
        aluno->setIngressante(true);
        //EV << "\n Enviando aluno \"" << aluno->getNumero()   << "\" como Ingressante. \n" << endl;

        ++cont;

        //caso a quantidade seja igual a 41, 51, ou 61 ou qualquer outro semelhante a depender do parametro.
        if(cont == qtdeAlunos){
            //envia mensagem de Turma
            aluno->setNome("turma");
            scheduleAt(simTime()+tempo(), aluno);
            --count;
        }else{
            scheduleAt(simTime()+tempo(), aluno);
        }

    }

}

/*
 * Metodo que verifica se uma turma foi enviado por completo para a escolha do tempo.
 * Em caso positivo incrementa o tempo da prox turma para enviar num momento posterior.
 */
double Inicio::tempo(){
    double contagem = 0.0;
    int t = qtdeAlunos;
    if(count > t){
        contagem = contagem + 6.0;
        t = t + qtdeAlunos;
        while(count > t){
            contagem = contagem + 6.0;
            t = t + qtdeAlunos;
        }
    }
    return contagem;
}

void Inicio::finish(){
    EV << "Enviado "<< entradas<< " entradas com "<< qtdeAlunos-1 << " alunos cada totalizando "<< entradas*(qtdeAlunos-1) <<" alunos."<< endl;
}


