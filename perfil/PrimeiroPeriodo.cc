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
    bool encheuTurma;
    cHistogram turmaEspera;
    virtual void processar(Aluno *msg);
    virtual void colocarFila(Aluno *msg);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    virtual void finish() override;
    virtual Aluno * alunoPrioridade(Aluno * aluno);
    double notaAleatoria(){
       int rnum = std::rand();
       return rnum % 10;
    };
};

Define_Module(PrimeiroPeriodo);

void PrimeiroPeriodo::initialize() {
    capacidadeFila = 10;
    capacidade = capacidadeFila;
    encheuTurma = false;
}


void PrimeiroPeriodo::handleMessage(cMessage *msg) {
    Aluno *aluno = dynamic_cast<Aluno *>(msg);
   if(aluno->getNome() == "turma"){
       EV << " Criando turmas de 10 alunos "  << endl;
       encheuTurma = false;
        //delete aluno;
    }else{
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
            EV << "Enviando \"" << alunoParaEnvio->getNumero() << "\" para o mesmo periodo na fila de espera " << filaEspera.getLength() << " "<< endl;
            //o aluno entra na fila de espera para a pro turma
            alunoParaEnvio->setQtdMatriculas(alunoParaEnvio->getQtdMatriculas()+1);
            filaEspera.insert(alunoParaEnvio);
            controle = false;
        }
        turmaEspera.collect(filaEspera.getLength());
        delete processando;
        if (turma.isEmpty()) {
             processando = nullptr;
        } else {
             processando = alunoPrioridade(aluno);
             processar(processando);
            }
        } else if (!processando) {
               processando = alunoPrioridade(aluno);
               if(controle){
                   processar(processando);
               }
             //senao se estiver em analise, coloca o aluno que chegou na fila.
           } else {
               if(controle)
                   colocarFila(aluno);
           }

    }
}


void PrimeiroPeriodo::processar(Aluno *aluno) {
    simtime_t tempoServico = exponential(tempoProcessamento);
    EV << "Processando \"" << aluno->getNumero() << "\" por " << tempoServico << "s." << endl;
    // aluno est� processsanod, seta processando como true e agenda o envio
    aluno->setProcessando(true);
    scheduleAt(simTime()+1.2, aluno);
}


void PrimeiroPeriodo::colocarFila(Aluno *aluno) {
    //a turma so eh enchida uma vez por leva de alunos. Uma vez enchida, so sera novamente na prox leva
    //turma menor que a capacidade e nao encheu
    if (turma.getLength() < capacidadeFila && encheuTurma == false) {
        EV << "Colocando \"" << aluno->getNumero() << "\" na turma*** (#fila: " << turma.getLength() << ")." << endl;
        turma.insert(aluno);
    }else
    //turma igual a capacidade
    if(encheuTurma == false){
         EV << "Turma cheia, vai para a fila de espera " << filaEspera.getLength() << "." << endl;
         //Encheu a turma
         encheuTurma = true;
         filaEspera.insert(aluno);
    }else if(encheuTurma){
        EV << "Turma cheia, vai para a fila de espera " << filaEspera.getLength() << "." << endl;
        filaEspera.insert(aluno);
    }
//    if(turma.getLength() == 0 && encheuTurma == true){
//       encheuTurma = false;
//    }

}

Aluno * PrimeiroPeriodo::alunoPrioridade(Aluno *aluno){
    Aluno * aluno1 = new Aluno();
    Aluno * aluno2 = new Aluno();
    Aluno * retorno = new Aluno();

    //se houver fila de espera e turma
    //entao compara qual sera o prox a processar baseado na quantidade de matriculas
    //precisa ser ajustado para pegar a devida preferencia
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
        } else if(turma.getLength() == 0){
            retorno = aluno;
        }else{
            retorno = check_and_cast<Aluno *>(turma.pop());
        }

    return retorno;
}

void PrimeiroPeriodo::finish(){
    EV << "\n Fila de espera" << endl;
    EV << "Valores para a fila de espera" << endl;
    EV << " Fila de espera, min:    " << turmaEspera.getMin() << endl;
    EV << " Fila de espera, max:    " << turmaEspera.getMax() << endl;
    EV << " Fila de espera, mean:   " << turmaEspera.getMean() << endl;
    EV << " Fila de espera, stddev: " << turmaEspera.getStddev() << endl;
    EV << " Total da fila de espera no momentov: " << filaEspera.getLength() << endl;
    turmaEspera.recordAs("Espera");
}
