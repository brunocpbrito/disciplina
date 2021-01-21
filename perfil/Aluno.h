#include <string.h>
#include <omnetpp.h>

class Aluno : public omnetpp::cMessage{
private:
    int numero;
    std::string nome;
    int nota;
    int raca;//1 branco 2 preto 3 indio
    int evadido; //0 nao evadido 1 evadido
    int faltas;
    int qtdMatriculas; // 0 qtde alta, 1 qtde baixa;
public:
    Aluno();
    Aluno(int numero, std::string nome, double nota);
    virtual ~Aluno();

    virtual void setNome(std::string nome);
    virtual void setNumero(int numero);
    virtual void setNota(int nota);

    virtual void setRaca(int raca);
    virtual void setEvadido(int evadido);
    virtual void setFaltas(int faltas);

    virtual std::string getNome();
    virtual int getNumero();
    virtual int getNota();

    virtual int getRaca();
    virtual int getEvadido();
    virtual int getFaltas();

    virtual int getQtdMatriculas();
    virtual void setQtdMatriculas(int qtde);


};

