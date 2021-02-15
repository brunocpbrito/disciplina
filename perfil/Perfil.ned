package perfil;


package exemplo6;

simple Inicio
{
    parameters:
        @display("i=old/folder;i2=status/red, purple");
        int capacidadeFila = default(10);
    gates:
        //input entrada;
        output saida[1];
}

simple PrimeiroPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(10);
    gates:
        input entrada[1];
        output saida[1];
}

simple SegundoPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(10);
    gates:
        input entrada[1];
        output saida[1];
}
simple TerceiroPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada[1];
        output saida[1];
}

simple SextoPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada[20];
        output saida[20];
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
    submodules:
        inicio: Inicio {
            @display("i=,yellow1;p=29,93");
        }
        primeiroPeriodo: PrimeiroPeriodo {
            @display("i=,purple;p=133,93");
        }
        segundoPeriodo: SegundoPeriodo {
            @display("i=,grey;p=239,93");
        }
        terceiroPeriodo: TerceiroPeriodo {
            @display("i=,pink;p=342,93");
        }
        //        sextoPeriodo: SextoPeriodo {
        //            @display("i=,white;p=537,93");
        //        }
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
            @display("i=,red;p=444,93");
        }
    connections:

        inicio.saida[0] --> D --> primeiroPeriodo.entrada[0];
        primeiroPeriodo.saida[0] --> D --> segundoPeriodo.entrada[0];
            //primeiroPeriodo.saida[0] --> D --> primeiroPeriodo.entrada[i + 10];

        segundoPeriodo.saida[0] --> D --> terceiroPeriodo.entrada[0];
            //segundoPeriodo.saida[i + 10] --> D --> segundoPeriodo.entrada[i + 10];

        terceiroPeriodo.saida[0] --> D --> fim.entrada[0];
            //quintoPeriodo.saida[i + 10] --> D --> quintoPeriodo.entrada[i + 10];

            //sextoPeriodo.saida[i] --> D --> fim.entrada[i];            
            //sextoPeriodo.saida[i + 10] --> D --> sextoPeriodo.entrada[i + 10];




}
