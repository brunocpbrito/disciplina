package perfil;


package exemplo6;

simple Inicio
{
    parameters:
        @display("i=old/folder;i2=status/red, purple");
        int quantidadeAlunos = default(40);    //quantidade de alunos numa entrada
        int entradas = default(100);            //quantidade de entradas(periodos) de uma turma. 
        									   //Ex.2021.1, 2020.2, 2021.1, 2021.2

        //Este parametro serve apenas como artificio para controle de codigo, 
        //desconsiderar como parametros para turma
        int qtdeAlunos = quantidadeAlunos + 1;

    gates:
        //input entrada;
        output saida[1];
}

simple PrimeiroPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeTurma = default(100);                    //capacidade maxima da turma
        int probReprovacaoAluno = default(0);                  //porcentagem da probablidade de um aluno reprovar o periodo
        int probCancelamentoDisciplina = default(0);           //porcentagem da probabilidade de um aluno cancelar a disciplina
    gates:
        input entrada[1];
        output saida[2];
}

simple SegundoPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red,red");
        int capacidadeTurma = default(100);      //capacidade maxima da turma
        int probReprovacaoAluno = default(0);  //porcentagem da probablidade de um aluno reprovar o periodo
        int probCancelamentoDisciplina = default(0);            //porcentagem da probabilidade de um aluno trancar.
        int probRetornoCancelamento = default(0);           //porcentagem da probabilidade de um aluno trancado retornar.
    gates:
        input entrada[1];
        output saida[1];
}
simple TerceiroPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red,red");
        int capacidadeTurma = default(100);      //capacidade maxima da turma
        int probReprovacaoAluno = default(0);  //porcentagem da probablidade de um aluno reprovar o periodo
        int probCancelamentoDisciplina = default(0);            //porcentagem da probabilidade de um aluno se evadir
        int probRetornoCancelamento = default(0);          //porcentagem da probabilidade de um aluno trancado retornar.   
    gates:
        input entrada[1];
        output saida[1];
}

simple Merge
{
    parameters:
        @display("i=old/folder;red");

    gates:
        input entrada[2];
        output saida[1];
}

simple Especifica
{
    parameters:
        @display("i=old/folder;i2=status/red,red");
        int capacidadeFila = default(2);
    gates:
        input entrada;
        output saida;
}

simple Fim
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada[1];
}

network Perfil
{
    @display("bgb=620,346");
    types:
        channel D extends ned.DatarateChannel
        {
            delay = 0.2s;
        }
        channel T extends ned.DatarateChannel
        {
            delay = 1s;
        }


    submodules:
        inicio: Inicio {
            @display("i=abstract/people,yellow1;p=29,150;i2=-");
        }
        Disciplina_A: PrimeiroPeriodo {
            @display("i=abstract/people,purple;p=119,150;t=PRIMEIRO PERIODO,,black;i2=-");
        }
        Disciplina_B: SegundoPeriodo {
            @display("i=abstract/people,#FFFF80;p=221,61;t=SEGUNDO PERIODO,,#400000;i2=-");
            
        }
        Disciplina_C: SegundoPeriodo {
            @display("i=abstract/people,grey;p=221,251;t=SEGUNDO PERIODO,,black;i2=-");
            
        }
        Disciplina_D: TerceiroPeriodo {
            @display("i=abstract/people,pink;p=397,150;t=TERCEIRO PERIODO,,black;i2=-");
        }
        merge: Merge {
            @display("i=block/join,orange,5;p=320,150;b=0,0,,11");
        }
        formados: Fim {
            @display("i=abstract/people2,#FFFF80;p=544,149;i2=-");
        }
    connections:

        inicio.saida[0] --> D --> Disciplina_A.entrada[0];

        Disciplina_A.saida[0] --> D --> Disciplina_B.entrada[0];
        Disciplina_A.saida[1] --> D --> Disciplina_C.entrada[0];

        Disciplina_B.saida[0] --> D --> merge.entrada[0];
        Disciplina_C.saida[0] --> D --> merge.entrada[1];

        merge.saida[0] --> D --> Disciplina_D.entrada[0];

        Disciplina_D.saida[0] --> D --> formados.entrada[0];

}
