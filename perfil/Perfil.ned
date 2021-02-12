package perfil;


package exemplo6;

simple Inicio
{
    parameters:
        @display("i=old/folder;i2=status/red, purple");
        int capacidadeFila = default(10);
    gates:
        //input entrada;
        output saida[10];
}

simple PrimeiroPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(10);
    gates:
        input entrada[20];
        output saida[20];
}

simple SegundoPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(10);
    gates:
        input entrada[20];
        output saida[20];
}
simple QuintoPeriodo
{
    parameters:
        @display("i=old/folder;i2=status/red, red");
        int capacidadeFila = default(2);
    gates:
        input entrada[20];
        output saida[20];
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
        input entrada[10];
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
            @display("i=,purple;p=172,93");
        }
        segundoPeriodo: SegundoPeriodo {
                    @display("i=,grey;p=273,93");
        }
        //        quintoPeriodo: QuintoPeriodo {
        //            @display("i=,pink;p=405,93");
        //        }
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
            @display("i=,red;p=364,93");
        }
    connections:

        for i=0..9 {
            inicio.saida[i] --> D --> primeiroPeriodo.entrada[i];
            primeiroPeriodo.saida[i] --> D --> segundoPeriodo.entrada[i];
            primeiroPeriodo.saida[i + 10] --> D --> primeiroPeriodo.entrada[i + 10];
				
           segundoPeriodo.saida[i] --> D --> fim.entrada[i];
           segundoPeriodo.saida[i + 10] --> D --> segundoPeriodo.entrada[i + 10];

            //quintoPeriodo.saida[i] --> D --> sextoPeriodo.entrada[i];
            //quintoPeriodo.saida[i + 10] --> D --> quintoPeriodo.entrada[i + 10];

            //sextoPeriodo.saida[i] --> D --> fim.entrada[i];            
            //sextoPeriodo.saida[i + 10] --> D --> sextoPeriodo.entrada[i + 10];


        }
	
//        comum.saida++ --> D --> especifica1.entrada;
//        comum.saida++ --> D --> especifica2.entrada;
//        especifica1.saida --> D --> fim.entrada++;
//        especifica2.saida --> D --> fim.entrada++;

}
