#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "Aluno.h"


using namespace omnetpp;

class PrimeiroPeriodo : public cSimpleModule {
  private:
    int capacidadeFila;
    int capacidade;
    cQueue turma;
    cQueue filaEspera;

    int portaSaida = 0;
    int portaEntrada = 0;

    Aluno *processando;

    double tempoProcessamento = 1;
    bool controle;
    virtual void processar(Aluno *msg);
    virtual void colocarFila(Aluno *msg);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    virtual Aluno * alunoPrioridade();
    double notaAleatoria(){
       int rnum = std::rand();
       return rnum % 10;
    };
};

Define_Module(PrimeiroPeriodo);

void PrimeiroPeriodo::initialize() {
    capacidadeFila = par("capacidadeFila");
    capacidade = capacidadeFila;
}


void PrimeiroPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno *>(msg);
    EV << "Recebeu \"" << aluno->getNumero() << "\", status processamento: " << aluno->getProcessando()  << endl;


    controle = true;
    // criei a variavel processando para n�o trabalhar com evadido
    if (aluno->getProcessando()) {

        //criado uma nova variavel para poder enviar.
        Aluno *alunoParaEnvio = new Aluno(processando->getNumero(), processando->getNome(), processando->getNota());

        //seta a nota aleatoria
        alunoParaEnvio->setNota(notaAleatoria());
        alunoParaEnvio->setRaca(2);

        // se nota maior que 70, entra na porta saida que leva para o proximo periodo
        if (alunoParaEnvio->getNota() >= 7) {
            EV << "Enviando \"" << alunoParaEnvio->getNumero() << "\" sendo enviado para outro periodo "<< endl;
            send(alunoParaEnvio, "saida", 0);
            controle = false;
        }
        // sen�o, entra na porta saida que leva para o periodo atual
        else {
            EV << "Enviando \"" << alunoParaEnvio->getNumero() << "\" para o mesmo periodo "<< endl;
            //nesse trecho ele vai para porta 85 e dá erro, depois verificar, comentei por isso.
            alunoParaEnvio->setQtdMatriculas(alunoParaEnvio->getQtdMatriculas()+1);
            send(alunoParaEnvio, "saida", 14);
            controle = false;
        }

        delete processando;
        if (turma.isEmpty()) {
             processando = nullptr;
        } else {
             processando = alunoPrioridade();
             processar(processando);
            }
        } else if (!processando) {
               processando = aluno;
               if(controle){
                   processar(processando);
               }
             //senao se estiver em analise, coloca o aluno que chegou na fila.
           } else {
               if(controle)
                   colocarFila(aluno);
           }
}



void PrimeiroPeriodo::processar(Aluno *aluno) {
    simtime_t tempoServico = exponential(tempoProcessamento);
    EV << "Processando \"" << aluno->getNumero() << "\" por " << tempoServico << "s." << endl;
    // aluno est� processsanod, seta processando como true e agenda o envio
    aluno->setProcessando(true);
    scheduleAt(simTime()+1.1, aluno);
}


void PrimeiroPeriodo::colocarFila(Aluno *aluno) {

    if (turma.getLength() == capacidadeFila) {
        EV << "Colocando \""<< aluno->getNumero() << "\" Na fila de espera. Motivo: turma cheia (#fila: " << capacidadeFila << "." << endl;
        filaEspera.insert(aluno);

    } else {
        if(filaEspera.getLength() == 0){
            // aluno adicionado na fila, seta estaNaFila para true
            aluno->setEstaNaFila(true);
            turma.insert(aluno);
            EV << "Colocando \"" << aluno->getNumero() << "\" na turma (#fila: " << turma.getLength() << ")." << endl;
        }else{
            EV << "Colocando na fila de espera. Motivo: turma cheia " << endl;
            filaEspera.insert(aluno);
        }

    }
}

Aluno * PrimeiroPeriodo::alunoPrioridade(){
    Aluno * aluno1 = new Aluno();
    Aluno * aluno2 = new Aluno();
    Aluno * retorno = new Aluno();

    //se houver fila de espera e turma
    //entao compara qual sera o prox a processar baseado na quantidade de reprovacaoes
    if(filaEspera.getLength() > 0 && turma.getLength() > 0){

        aluno1 = check_and_cast<Aluno *>(turma.front());
        aluno2 = check_and_cast<Aluno *>(filaEspera.front());
        if(aluno1->getQtdMatriculas() <= aluno2->getQtdMatriculas()){
            EV << "Mantem a prioridade do aluno." << endl;
            retorno = check_and_cast<Aluno *>(turma.pop());
        }else{
            EV << "troca de alunos pela prioridade da fila de espera." << endl;
            retorno = check_and_cast<Aluno *>(filaEspera.pop());
        }
        } else{
            retorno = check_and_cast<Aluno *>(turma.pop());
        }

    //Se a turma acabar pega da lista de espera.
    if(filaEspera.getLength() > 0 && turma.getLength() == 0){
        EV << "Pegando alunos da fila de espera." << endl;
        retorno = check_and_cast<Aluno *>(filaEspera.pop());
    }

    return retorno;
}
