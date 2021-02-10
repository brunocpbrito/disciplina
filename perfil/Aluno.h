#include <string.h>
#include <omnetpp.h>

class Aluno : public omnetpp::cMessage{
private:
    int numero;
    std::string nome;
    double nota;
    int raca;//1 branco 2 preto 3 indio
    int evadido; //0 nao evadido 1 evadido
    int faltas;
    int qtdMatriculas;
    bool processando;
    bool ingressante;
public:
    Aluno();
    Aluno(int numero, std::string nome, double nota);
    virtual ~Aluno();

    virtual void setNome(std::string nome);
    virtual void setNumero(int numero);
    virtual void setNota(double nota);
    virtual void setProcessando(bool processando);
    virtual void setIngressante(bool ingressante);

    virtual void setRaca(int raca);
    virtual void setEvadido(int evadido);
    virtual void setFaltas(int faltas);

    virtual std::string getNome();
    virtual int getNumero();
    virtual double getNota();
    virtual bool getProcessando();
    virtual bool getIngressante();

    virtual int getRaca();
    virtual int getEvadido();
    virtual int getFaltas();

    virtual int getQtdMatriculas();
    virtual void setQtdMatriculas(int qtde);


};

