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
    int probReprovacao;
    int probCancelamentoDisciplina;
    bool pegarEspera;
    cQueue turma;
    cQueue filaEspera;

    //alunos que cancelaram a disciplina ao iniciar
    cQueue filaCancelados;

    double tempoProcessamento = 1;

    cHistogram turmaEspera;
    cHistogram mediaTurma;
    virtual void processar();
    virtual void colocarFila(Aluno *msg);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    virtual void finish() override;

    virtual void destinoAluno(Aluno * aluno);
    double notaAleatoria(){
       int rnum = std::rand();
       return rnum % 100;
    };
};

Define_Module(PrimeiroPeriodo);

void PrimeiroPeriodo::initialize() {
    capacidadeFila = par("capacidadeTurma");
    probReprovacao = par("probReprovacaoAluno");
    probCancelamentoDisciplina = par("probCancelamentoDisciplina");

    pegarEspera = true;
}

/*
 * Metodo que recebe uma mensagem do periodo anterior. Se a mensagem for um Aluno, então este eh colocado na
 * fila(turma). Se a mensagem for Turma, então processa a turma existente enviando para o prox periodo
 */
void PrimeiroPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno*>(msg);
    if (aluno->getNome() == "turma") {
        //EV << "\n Criando turmas de "<< capacidadeFila <<" alunos no PrimeiroPeriodo. \n" << endl;

        //caso a turma seja menor que a capacidade, então pega da turma de espera ate completar as vagas restantes.
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

        //processa a turma existente capturando os valores para medir
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

/*
 * Metodo que processa os alunos de uma turma, atribuindo notas aleatorias e colocando como processados e
 * em seguida enviando ao prox periodo.
 * Envia ao final uma mensagem de Turma que significa que o prox periodo tera que processar esta turma.
 */
void PrimeiroPeriodo::processar() {
    while (!turma.isEmpty()) {
        Aluno *aluno = check_and_cast<Aluno*>(turma.pop());
        simtime_t tempoServico = exponential(tempoProcessamento);
        //EV << "Processando \"" << aluno->getNumero() << "\" por " << tempoServico << "s." << endl;
        aluno->setProcessando(true);
        aluno->setNota(notaAleatoria());
        destinoAluno(aluno);
    }
    if (turma.isEmpty()) {
        Aluno *turma = new Aluno();
        turma->setNome("turma");
        EV << "\n !!Enviando alunos para Disciplina B do Segundo Periodo.!! \n " << endl;
        //envia mensagem para criar nova turma no prox periodo
        send(turma, "saida", 0);

        Aluno *turma2 = new Aluno();
        turma2->setNome("turma");
        EV << "\n !!Enviando alunos para Disciplina C do Segundo Periodo.!! \n " << endl;
        //envia mensagem para criar nova turma no prox periodo
        send(turma2, "saida", 1);

    }

}

/*
 * Metodo que coloca um aluno na turma enquanto houver vagas. Uma vez a turma cheia, os alunos vao para a fila de
 * espera.
 */
void PrimeiroPeriodo::colocarFila(Aluno *aluno) {

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
    EV << "Total de alunos que cancelaram a disciplina: " << filaCancelados.getLength() << endl;
}

/*
 * Metodo que verifica o destino do aluno na turma, se aprovado e segue para o prox periodo,
 * se reprovado e volta ao mesmo periodo ou se evadido e saira do sistema.
 * Os valores sao baseados em parametros do arquivo .ned.
 */
void PrimeiroPeriodo::destinoAluno(Aluno *aluno) {

    int rnum = std::rand();
    int valor = rnum % 100;
    //probabilidade do aluno se evadir
    if (valor >= probCancelamentoDisciplina) {
        // se nota maior que 70, entra na porta saida que leva para o proximo periodo
        if (aluno->getNota() >= probReprovacao) {

            aluno->setProcessando(false);
            EV << "Aprovado aluno \"" << aluno->getNumero()   << "\" sendo enviado para Segundo periodo " << endl;

            Aluno *copiaAluno = new Aluno();
            copiaAluno->setNumero(aluno->getNumero());
            copiaAluno->setNome(aluno->getNome());
            copiaAluno->setNota(aluno->getNota());

            send(aluno, "saida", 0);
            send(copiaAluno, "saida", 1);

        }
        // senao, entra na porta saida que leva para o periodo atual
        else {
            EV << "Reprovado o aluno \"" << aluno->getNumero() << "\" para o mesmo periodo na fila de espera. Total espera: " << filaEspera.getLength() << " " << endl;
            //o aluno entra na fila de espera para a pro turma
            aluno->setQtdMatriculas(aluno->getQtdMatriculas() + 1);
            filaEspera.insert(aluno);

        }

    } else {
        filaCancelados.insert(aluno);
        EV << "Aluno \"" << aluno->getNumero()   << "\" cancelou a disciplina. Total: "<< filaCancelados.getLength() << " " << endl;
    }
}



