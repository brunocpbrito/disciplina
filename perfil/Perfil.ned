package perfil;


package exemplo6;

simple Inicio
{
    parameters:
        @display("i=old/folder;i2=status/red, purple");
        int capacidadeFila = default(2);
    gates:
        //input entrada;
        output saida;
}

simple PrimeiroPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada;
        output saida;
}

simple TerceiroPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada;
        output saida;
}
simple QuintoPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada;
        output saida;
}

simple SextoPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada;
        output saida;
}

simple Comum
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(0);
    gates:
        input entrada;
        output saida[1];
}

simple Especifica
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
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
        input entrada;
}

network Perfil
{
    @display("bgb=381,245");
    types:
        channel D extends ned.DatarateChannel
        {
            delay = 0.2s;
        }
    submodules:
        inicio: Inicio {
            @display("i=,yellow1;p=345,99");
        }
        primeiroPeriodo: PrimeiroPeriodo {
            @display("i=,purple;p=280,99");
        }
        terceiroPeriodo: TerceiroPeriodo {
            @display("i=,grey;p=280,30");
        }
        quintoPeriodo: QuintoPeriodo {
            @display("i=,pink;p=192,30");
        }
        sextoPeriodo: SextoPeriodo {
            @display("i=,white;p=192,99");
        }
//        especifica1: Especifica {
//            @display("i=,darkblue;p=29,117");
//        }
//        especifica2: Especifica {
//            @display("i=,darkblue;p=99,117");
//        }
//        comum: Comum {
//            @display("i=,red;p=43,45");
//        }
        fim: Fim {
            @display("i=,red;p=192,192");
        }
    connections:
        inicio.saida --> D --> primeiroPeriodo.entrada;
        primeiroPeriodo.saida --> D --> terceiroPeriodo.entrada;
        terceiroPeriodo.saida --> D --> quintoPeriodo.entrada;
        quintoPeriodo.saida --> D --> sextoPeriodo.entrada;
        sextoPeriodo.saida --> D --> fim.entrada;
//        comum.saida++ --> D --> especifica1.entrada;
//        comum.saida++ --> D --> especifica2.entrada;
//        especifica1.saida --> D --> fim.entrada++;
//        especifica2.saida --> D --> fim.entrada++;

}
