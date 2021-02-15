#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <list>
#include <iostream>
#include "Aluno.h"
#include <vector>
using namespace std;


using namespace omnetpp;

class PrimeiroPeriodo : public cSimpleModule {
  private:
    int capacidadeFila;
    int capacidade;
    bool pegarEspera;
    cQueue turma;
    cQueue filaEspera;
    cQueue filaEvadidos;

    double tempoProcessamento = 1;

    cHistogram turmaEspera;
    cHistogram mediaTurma;
    virtual void processar();
    virtual void colocarFila(Aluno *msg);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    //virtual ~PrimeiroPeriodo() override;
    virtual void finish() override;

    virtual void destinoAluno(Aluno * aluno);
    double notaAleatoria(){
       int rnum = std::rand();
       return rnum % 10;
    };
};

Define_Module(PrimeiroPeriodo);

void PrimeiroPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");;
    capacidade = capacidadeFila;
    pegarEspera = true;

}

void PrimeiroPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno*>(msg);
    if (aluno->getNome() == "turma") {
        //EV << "\n Criando turmas de "<< capacidadeFila <<" alunos no PrimeiroPeriodo. \n" << endl;

        if (pegarEspera) {
            if (turma.getLength() < capacidadeFila && !filaEspera.isEmpty()) {
                EV << "\n Turma com " << turma.getLength()  << " alunos, restando "   << (capacidadeFila - turma.getLength())  << " vagas. Pegando alunos da fila de espera ("   << filaEspera.getLength()   << ") do Primeiro Periodo, ate completar as vagas. \n" << endl;
                while (turma.getLength() < capacidadeFila) {
                    if (!filaEspera.isEmpty()) {
                        Aluno *alunoFila = check_and_cast<Aluno*>( filaEspera.pop());
                        turma.insert(alunoFila);
                    } else {
                        break;
                    }
                }
            }
        }

        EV << "\n Criando turma no Primeiro Periodo de "<< turma.getLength() <<" alunos e fila de espera "<< filaEspera.getLength() <<" \n" << endl;
        turmaEspera.collect(filaEspera.getLength());
        mediaTurma.collect(turma.getLength());
        processar();
        //delete aluno;
    } else {
        //EV << "Recebeu \"" << aluno->getNumero() << "\", status processamento: " << aluno->getProcessando() << "\" do PrimeiroPeriodo " << endl;
        colocarFila(aluno);

    }
}

void PrimeiroPeriodo::processar() {
    while (!turma.isEmpty()) {
        Aluno *aluno = check_and_cast<Aluno*>(turma.pop());
        simtime_t tempoServico = exponential(tempoProcessamento);
        //EV << "Processando \"" << aluno->getNumero() << "\" por " << tempoServico << "s." << endl;
        aluno->setProcessando(true);
        destinoAluno(aluno);
    }
    if (turma.isEmpty()) {

        Aluno *turma = new Aluno();
        turma->setNome("turma");
        EV << "\n !!Enviando alunos para o Segundo Periodo.!! \n " << endl;
        //envia mensagem para criar nova turma no prox periodo
        send(turma, "saida", 0);
    }

}


void PrimeiroPeriodo::colocarFila(Aluno *aluno) {
    //a turma so eh enchida uma vez por leva de alunos. Uma vez enchida, so sera novamente na prox leva
    //turma menor que a capacidade e nao encheu
    if (turma.getLength() < capacidadeFila) {
        //EV << "Colocando \"" << aluno->getNumero() << "\" na turma*** (#fila: "  << turma.getLength() + 1 << ")." << endl;
        turma.insert(aluno);
        if (turma.getLength() == capacidadeFila) {
            EV << "\n Turma do Primeiro Periodo com "<< turma.getLength() <<" completa, o resto vai para a fila de espera. \n"  << endl;

        }
    } else {
        EV << "Turma cheia, aluno "<<aluno->getNumero() <<" vai para a fila de espera (" << filaEspera.getLength() << ")." << endl;
        //Encheu a turma
        filaEspera.insert(aluno);
    }


}



void PrimeiroPeriodo::finish(){
    EV << "\n ## VALORES PARA O Primeiro PERIODO ##" << endl;
    EV << "Capacidade da turma de "<< capacidadeFila <<" alunos" << endl;
    EV << "Valores para a fila de espera do PrimeiroPeriodo" << endl;
    EV << "  Fila de espera, min:    " << turmaEspera.getMin() << endl;
    EV << "  Fila de espera, max:    " << turmaEspera.getMax() << endl;
    EV << "  Fila de espera, media:   " << turmaEspera.getMean() << endl;
    EV << "  Fila de espera, desvio padrao:   " << turmaEspera.getStddev() << endl;
    turmaEspera.recordAs("Espera");
    EV << "Valores para a turma do Primeiro Periodo" << endl;
    EV << "  Turma, min:    " << mediaTurma.getMin() << endl;
    EV << "  Turma, max:    " << mediaTurma.getMax() << endl;
    EV << "  Turma, media:   " << mediaTurma.getMean() << endl;
    EV << "  Turma, desvio padrao:   " << mediaTurma.getStddev() << endl;
    EV << "Total de reprovados no momento: " << filaEspera.getLength() << endl;
    EV << "Total de evadidos no momento: " << filaEvadidos.getLength() << endl;
}

void PrimeiroPeriodo::destinoAluno(Aluno *aluno) {

    int rnum = std::rand();
    int probabilidade = rnum % 10;
    //probabilidade do aluno se evadir
    if (probabilidade >= 1) {
        // se nota maior que 70, entra na porta saida que leva para o proximo periodo
        if (aluno->getNota() >= 3) {

            aluno->setProcessando(false);
            EV << "Aprovado aluno \"" << aluno->getNumero()   << "\" sendo enviado para Segundo periodo " << endl;

            //colecao.push_back(aluno);
            send(aluno, "saida", 0);

        }
        // senao, entra na porta saida que leva para o periodo atual
        else {
            EV << "Reprovado o aluno \"" << aluno->getNumero() << "\" para o mesmo periodo na fila de espera. Total espera: " << filaEspera.getLength() + 1 << " " << endl;
            //o aluno entra na fila de espera para a pro turma
            aluno->setQtdMatriculas(aluno->getQtdMatriculas() + 1);
            filaEspera.insert(aluno);

        }

    } else {
        filaEvadidos.insert(aluno);
        EV << "Aluno \"" << aluno->getNumero()   << "\" considerado como evadido. Total: "<< filaEvadidos.getLength() + 1 << " " << endl;
    }
}



