#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <list>
#include <iostream>
#include "Aluno.h"
#include <vector>
using namespace std;


using namespace omnetpp;

class SegundoPeriodo : public cSimpleModule {
  private:
    int capacidadeFila;
    int capacidade;
    cQueue turma;
    cQueue filaEspera;
    cQueue filaEvadidos;
    vector<Aluno *> colecao;

    int portaSaida = 0;
    int portaEntrada = 0;

    Aluno *processando;

    double tempoProcessamento = 1;
    bool encheuTurma;
    cHistogram turmaEspera;
    virtual void processar();
    virtual void colocarFila(Aluno *msg);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    //virtual ~PrimeiroPeriodo() override;
    virtual void finish() override;
    virtual Aluno * alunoPrioridade(Aluno * aluno);
    virtual void destinoAluno(Aluno * aluno);
    double notaAleatoria(){
       int rnum = std::rand();
       return rnum % 10;
    };
};

Define_Module(SegundoPeriodo);

void SegundoPeriodo::initialize() {
    capacidadeFila = 10;
    capacidade = capacidadeFila;
    encheuTurma = false;
    processando = nullptr;

}

void SegundoPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno*>(msg);
    if (aluno->getNome() == "turma") {
        EV << "\n Criando turmas de 10 alunos no Segundo Periodo \n" << endl;
        encheuTurma = false;

        if(turma.getLength() < capacidadeFila && !filaEspera.isEmpty()){
            EV << "\n Turma com vagas, pegando alunos da fila de espera do Segundo Periodo \n" << endl;
            while(turma.getLength() < capacidade){
                while(!filaEspera.isEmpty()){
                    Aluno *alunoFila = check_and_cast<Aluno*>(filaEspera.pop());
                    turma.insert(alunoFila);
                }
            }
        }
        processar();
        //delete aluno;
    } else {
        EV << "Recebeu \"" << aluno->getNumero() << "\", status processamento: " << aluno->getProcessando() << "\" do Segundo Periodo " << endl;

        // criei a variavel processando para no trabalhar com evadido
        if (aluno->getProcessando()) {

            //criado uma nova variavel para poder enviar.
            Aluno *alunoParaEnvio = new Aluno(aluno->getNumero(), aluno->getNome(), aluno->getNota());

            //seta a nota aleatoria
            alunoParaEnvio->setNota(notaAleatoria());
            alunoParaEnvio->setRaca(2);

            destinoAluno(alunoParaEnvio);

        } else {
            //senao se estiver em analise, coloca o aluno que chegou na filaelse {
            colocarFila(aluno);
        }

    }
}

void SegundoPeriodo::processar() {
    while (!turma.isEmpty()) {
        Aluno *aluno = check_and_cast<Aluno*>(turma.pop());
        simtime_t tempoServico = exponential(tempoProcessamento);
        EV << "Processando \"" << aluno->getNumero() << "\" por " << tempoServico << "s." << endl;
        aluno->setProcessando(true);
        destinoAluno(aluno);
    }
    if (turma.isEmpty()) {

        Aluno *turma = new Aluno();
        turma->setNome("turma");
        EV << "\n !!Enviando alunos para o Terceiro Periodo.!! \n " << endl;
        //envia mensagem para criar nova turma no prox periodo
        send(turma, "saida", 0);
        encheuTurma = false;
    }

}


void SegundoPeriodo::colocarFila(Aluno *aluno) {
    //a turma so eh enchida uma vez por leva de alunos. Uma vez enchida, so sera novamente na prox leva
    //turma menor que a capacidade e nao encheu
    if (turma.getLength() < capacidadeFila && encheuTurma == false) {
        EV << "Colocando \"" << aluno->getNumero() << "\" na turma*** (#fila: "  << turma.getLength() + 1 << ")." << endl;
        turma.insert(aluno);
        if (turma.getLength() == capacidadeFila) {
            encheuTurma = true;
            processar();
        }
    } else
    //turma igual a capacidade
    if (encheuTurma == false) {
        EV << "Turma cheia, vai para a fila de espera " << filaEspera.getLength() + 1 << "." << endl;
        //Encheu a turma
        encheuTurma = true;
        filaEspera.insert(aluno);
    } else if (encheuTurma) {
        EV << "Turma cheia, vai para a fila de espera " << filaEspera.getLength() + 1 << "." << endl;
        filaEspera.insert(aluno);
    }
//    if(turma.getLength() capacidade && encheuTurma == false){
//       encheuTurma = false;
//    }

}

Aluno* SegundoPeriodo::alunoPrioridade(Aluno *aluno) {
    Aluno *aluno1 = new Aluno();
    Aluno *aluno2 = new Aluno();
    Aluno *retorno = new Aluno();

    //se houver fila de espera e turma
    //entao compara qual sera o prox a processar baseado na quantidade de matriculas
    //precisa ser ajustado para pegar a devida preferencia
    if (filaEspera.getLength() > 0 && turma.getLength() > 0) {

        aluno1 = check_and_cast<Aluno*>(turma.front());
        aluno2 = check_and_cast<Aluno*>(filaEspera.front());
        if (aluno1->getQtdMatriculas() <= aluno2->getQtdMatriculas()) {
            EV << "Mantem a prioridade do aluno." << endl;
            retorno = check_and_cast<Aluno*>(turma.pop());
        } else {
            EV << "troca de alunos pela prioridade da fila de espera." << endl;
            retorno = check_and_cast<Aluno*>(filaEspera.pop());
        }
    } else if (turma.getLength() <= 0) {
        EV << "Colocando \"" << aluno->getNumero() << "\" na turma e processando (#fila: " << turma.getLength() + 1 << ")." << endl;
        //turma.insert(aluno);
        retorno = aluno;
    } else {
        retorno = check_and_cast<Aluno*>(turma.pop());
    }

    return retorno;
}

void SegundoPeriodo::finish(){
    EV << "\n Fila de espera" << endl;
    EV << "Valores para a fila de espera" << endl;
    EV << " Fila de espera, min:    " << turmaEspera.getMin() << endl;
    EV << " Fila de espera, max:    " << turmaEspera.getMax() << endl;
    EV << " Fila de espera, mean:   " << turmaEspera.getMean() << endl;
    EV << " Fila de espera, stddev: " << turmaEspera.getStddev() << endl;
    EV << " Total da fila de espera no momento: " << filaEspera.getLength()+1 << endl;
    EV << " Total de evadidos no momento: " << filaEvadidos.getLength()+1 << endl;
    turmaEspera.recordAs("Espera");
}

void SegundoPeriodo::destinoAluno(Aluno *aluno) {

    int rnum = std::rand();
    int probabilidade = rnum % 10;
    //probabilidade do aluno se evadir
    if (probabilidade >= 1) {
        // se nota maior que 70, entra na porta saida que leva para o proximo periodo
        if (aluno->getNota() >= 4) {

            aluno->setProcessando(false);
            EV << "Aprovado e \"" << aluno->getNumero()   << "\" sendo enviado para Terceiro periodo " << endl;

            //colecao.push_back(aluno);
            send(aluno, "saida", 0);

        }
        // senao, entra na porta saida que leva para o periodo atual
        else {
            EV << "Reprovado \"" << aluno->getNumero() << "\" para o mesmo periodo na fila de espera " << filaEspera.getLength() + 1 << " " << endl;
            //o aluno entra na fila de espera para a pro turma
            aluno->setQtdMatriculas(aluno->getQtdMatriculas() + 1);
            filaEspera.insert(aluno);

        }
        turmaEspera.collect(filaEspera.getLength());
    } else {
        filaEvadidos.insert(aluno);
        EV << "Enviando aluno como evadido. Total: "<< filaEvadidos.getLength() + 1 << " " << endl;
    }
}



