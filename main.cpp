
#define MODO_JOGO                   0

#define EVOLUCAO                    0
#define TESTE                       1
#define DUELO                       2

#define POPULACAO_MAX               5000

#define BIRD_BRAIN_QTD_LAYERS       1
#define BIRD_BRAIN_QTD_INPUT        4
#define BIRD_BRAIN_QTD_HIDE         6
#define BIRD_BRAIN_QTD_OUTPUT       2

#define TAXA_GRAFICO_NEURAL         1
#define LARG_GRAFICO_NEURAL        ((1110 + 75)/TAXA_GRAFICO_NEURAL)
#define ALT_GRAFICO_NEURAL         ((693 + 693)/TAXA_GRAFICO_NEURAL)

//#include "PIG.h"
#include "redeNeural.c"
#include "Sprites.h"
#include "Tipos.h"
#include "Variaveis.h"
#include "FuncoesAuxiliares.h"

//#include "Desenhar.h"

//#include "Inicializar.h"
//#include "Alocacoes.h"

//#include "Colisao.h"
//#include "Movimentar.h"
//#include "Atualizar.h"
#include "InputsRedeNeural.h"


void AplicarGravidade()
{
    for(int i=0; i<QuantidadePassaros; i++)
    {
        if(Passaro[i].Estado != 2)
        {
            Passaro[i].VelocidadeY = Passaro[i].VelocidadeY - 0.08;
        }
    }
}

void ControlarEstadoPassaros()
{
    int Pular = 0, Paraquedas = 0;
    double Saida[10];
    double Entrada[10];

    for(int i=0; i<QuantidadePassaros; i++)
    {
        if(Passaro[i].Estado != 3)
        {
            Entrada[0] = DistanciaHorizonalProximoObstaculo(Passaro[i].X);
            Entrada[1] = DistanciaVerticalProximoObstaculo(Passaro[i].X, Passaro[i].Y);
            Entrada[2] = VelocidadeProximoObstaculo(Passaro[i].X);
            Entrada[3] = AberturaProximoObstaculo(Passaro[i].X);

            RNA_CopiarParaEntrada(Passaro[i].Cerebro, Entrada);
            RNA_CalcularSaida(Passaro[i].Cerebro);
            RNA_CopiarDaSaida(Passaro[i].Cerebro, Saida);

            if(Saida[0] > 0.0)
                Pular = 1;
            else
                Pular = 0;

            if(Saida[1] > 0.0 && Passaro[i].ParaquedasCooldown >= 4000.0)
                Paraquedas = 1;
            else
                Paraquedas = 0;

            if(MODO_JOGO == EVOLUCAO)
            {
                if(Pular == 1)
                {
                    Passaro[i].VelocidadeY = 3.5;
                    Passaro[i].Angulo = 45.0;
                    Passaro[i].Estado = 1;
                }

                if(Paraquedas == 1)
                {
                    Passaro[i].ParaquedasCooldown = 0;
                    Passaro[i].VelocidadeY = -0.05;
                    Passaro[i].Angulo = 0;
                    Passaro[i].Estado = 2;
                }
            }

            if(MODO_JOGO == TESTE || MODO_JOGO == DUELO)
            {
                if(i == 0)
                {
                    if(SinalPular == 1)
                    {
                        SinalPular = 0;
                        Passaro[i].VelocidadeY = 3.5;
                        Passaro[i].Angulo = 45.0;
                        Passaro[i].Estado = 1;
                    }

                    if(SinalParaquedas == 1 && Passaro[i].ParaquedasCooldown >= 4000.0)
                    {
                        SinalParaquedas = 0;
                        Passaro[i].ParaquedasCooldown = 0;
                        Passaro[i].VelocidadeY = -0.05;
                        Passaro[i].Angulo = 0;
                        Passaro[i].Estado = 2;
                    }
                }
                if(i == 1)
                {
                    if(Pular == 1)
                    {
                        Passaro[i].VelocidadeY = 3.5;
                        Passaro[i].Angulo = 45.0;
                        Passaro[i].Estado = 1;
                    }

                    if(Paraquedas == 1)
                    {
                        Passaro[i].ParaquedasCooldown = 0;
                        Passaro[i].VelocidadeY = -0.05;
                        Passaro[i].Angulo = 0;
                        Passaro[i].Estado = 2;
                    }
                }
            }
        }
    }
}


void EncerrarPartida()
{
    if(DistanciaAtual > DistanciaRecorde)
    {
        DistanciaRecorde = DistanciaAtual;
        //SalvarRedeArquivo();
    }
}

void CarregarRede()
{
    FILE* f = fopen("rede","rb");
    fread(&Passaro[1].TamanhoDNA, 1, sizeof(int), f);
    fread(DNADaVez[1], Passaro[1].TamanhoDNA, sizeof(double), f);
    fclose(f);
}

void RandomMutations()
{
    static double RangeRandom = Passaro[0].TamanhoDNA;

    Passaros* Vetor[POPULACAO_TAMANHO];
    Passaros* Temp;

    if(Geracao < LARG_GRAFICO)
    {
        GeracaoCompleta = Geracao+1;
        BestFitnessPopulacao[Geracao] = BestFitnessGeracao();
        MediaFitnessPopulacao[Geracao] = MediaFitnessGeracao();
    }
    else
    {
        for(int i=0; i<LARG_GRAFICO-1; i++)
        {
            BestFitnessPopulacao[i] = BestFitnessPopulacao[i+1];
            MediaFitnessPopulacao[i] = MediaFitnessPopulacao[i+1];
        }
        BestFitnessPopulacao[GeracaoCompleta] = BestFitnessGeracao();
        MediaFitnessPopulacao[GeracaoCompleta] = MediaFitnessGeracao();
    }

    for(int i=0; i<POPULACAO_TAMANHO; i++)
    {
        Vetor[i] = &Passaro[i];
    }

    for(int i=0; i<POPULACAO_TAMANHO; i++)
    {
        for(int j=0; j<POPULACAO_TAMANHO-1; j++)
        {
            if(Vetor[j]->Fitness < Vetor[j+1]->Fitness)
            {
                Temp = Vetor[j];
                Vetor[j] = Vetor[j+1];
                Vetor[j+1] = Temp;
            }
        }
    }

    int Step = 5;
    for(int i=0; i<Step; i++)  /// Clonando individuos
    {
        for(int j=Step+i; j<POPULACAO_TAMANHO; j=j+Step)
        {
            for(int k=0; k<Vetor[j]->TamanhoDNA; k++)
            {
                Vetor[j]->DNA[k] = Vetor[i]->DNA[k];        /// individuo 'j' recebe dna do individuo 'i'
            }
        }
    }

    for(int j=Step; j<POPULACAO_TAMANHO; j++)
    {
        int tipo;
        int mutations = (rand()%(int)RangeRandom)+1;

        for(int k=0; k<mutations; k++)
        {
            tipo = rand()%3;

            int indice = rand()%Vetor[j]->TamanhoDNA;
            switch(tipo)
            {
                case 0:
                {
                    Vetor[j]->DNA[indice] = getRandomValue();       /// Valor Aleatorio

                }   break;
                case 1:
                {
                    double number = (rand()%10001)/10000.0 + 0.5;
                    Vetor[j]->DNA[indice] = Vetor[j]->DNA[indice]*number;   /// Multiplica��o aleatoria

                }   break;
                case 2:
                {
                    double number = getRandomValue()/100.0;
                    Vetor[j]->DNA[indice] = Vetor[j]->DNA[indice] + number; /// Soma aleatoria

                }   break;
            }
        }
    }

    for(int j=0; j<POPULACAO_TAMANHO; j++)  /// Copiando novos DNAs para DNAsDaVez
    {
        for(int k=0; k<Passaro[j].TamanhoDNA; k++)
        {
            DNADaVez[j][k] = Passaro[j].DNA[k];
        }
    }

    for(int i=0; i<POPULACAO_TAMANHO; i++)
    {
        Vetor[i]->ResetarFitness = 1;
    }

    printf("Range Random: %f\n", RangeRandom);
    RangeRandom = RangeRandom*0.99;
    if(RangeRandom < 5)
    {
        RangeRandom = 5;
    }

    Geracao++;
}
int main(int argc, char* args[])
{

    while(1 == 1)
    {
        ControlarEstadoPassaros();
    }

    return 0;
}
