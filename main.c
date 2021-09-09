#include <stdio.h>
#include <stdlib.h>


typedef struct neuronio{
    float *Peso;
    float Erro;
    float Saida;

    int QtdLigacoes;
}Neuronio;

typedef struct camada{
    Neuronio *Neuronios;
    struct camada *CamadaAdjacente;
    int QtdNeuronios
}Camada;

typedef struct redeNeural{
    Camada CamadaEntrada;
    Camada *CamadaOculta;
    int QtdCamadasOculta;
    Camada CamadaSaida;

}RedeNeural;



void g(Camada camada){
    float sum;

    for (int i = 0; i < camada.QtdNeuronios; i++){
        sum = 0;
        for(int j = 0; j < camada.CamadaAdjacente[0].QtdNeuronios; j++){
            float peso = camada.CamadaAdjacente[0].Neuronios[j].Peso[i];
            float saida = camada.CamadaAdjacente[0].Neuronios[j].Saida;
            sum+= peso * saida;
            printf("\nPeso %f | Saida %f = %f\n", peso, saida, (peso * saida));
        }
        sum+= 0;
        sum = sum >= 0 ? 1 : 0;

        camada.Neuronios[i].Saida = sum;
    }

}

int step(float sum){
   return sum >= 0 ? 1 : 0;
}

Camada * criaCamada(){

}

RedeNeural* criarRedeNeural(int QtdNeuEntrada,int QtdNeuOculta, int QtdNeuSaida,int QtdCamadaOculta){

    RedeNeural *nn = (RedeNeural*)malloc(sizeof(RedeNeural));
   /// CamadaEntrada ////
    nn->CamadaEntrada.Neuronios = (Neuronio*)malloc(QtdNeuEntrada * sizeof(Neuronio)); // aloca vetor de Neuronio no tamanho QtdNeuEntrada
    nn->CamadaEntrada.QtdNeuronios = QtdNeuEntrada;

    //nn->CamadaEntrada.CamadaAdjacente = (Camada*)malloc(sizeof(Camada));
    for (int i = 0; i < QtdNeuEntrada; i++){
        nn->CamadaEntrada.Neuronios[i].Peso = (float*)malloc(QtdNeuOculta * sizeof(float));
    }

   /// ////////////////

   ///  CamadaOculta ///
    nn->CamadaOculta = (Camada*)malloc(QtdCamadaOculta * sizeof(Camada));// aloca vetor de camadas no tamanho QtdCamadaOculta
    nn->QtdCamadasOculta = QtdCamadaOculta;
    for (int i = 0; i < QtdCamadaOculta; i++){
        nn->CamadaOculta[i].Neuronios = (Neuronio*)malloc(QtdNeuOculta * sizeof(Neuronio)); // aloca vetor de Neuronio no tamanho QtdNeuOculta
        nn->CamadaOculta[i].QtdNeuronios = QtdNeuOculta;
        nn->CamadaOculta[i].CamadaAdjacente = (Camada*)malloc(sizeof(Camada));

        for (int j = 0; j < QtdNeuOculta; j++){
            nn->CamadaOculta[i].Neuronios[j].Peso = (float*)malloc(QtdNeuSaida * sizeof(float));
        }

        if(i == 0){
            nn->CamadaOculta[i].CamadaAdjacente[0] = nn->CamadaEntrada;
        }
        else{
            nn->CamadaOculta[i].CamadaAdjacente[0] = nn->CamadaOculta[i - 1];
        }

        //obs caso tenha mais de uma camada escondida todaasd tera a mesma quantodade de neronio

    }
   /// //////////////////

   ///  CamadaSaida ///

    nn->CamadaSaida.QtdNeuronios = QtdNeuSaida;
    nn->CamadaSaida.Neuronios = (Neuronio*)malloc(QtdNeuSaida * sizeof(Neuronio));
    nn->CamadaSaida.CamadaAdjacente = (Camada*)malloc(sizeof(Camada));
    nn->CamadaSaida.CamadaAdjacente[0] = nn->CamadaOculta[QtdCamadaOculta - 1];
   /// //////////////////
    return nn;
}
void main(){


    RedeNeural *NN = criarRedeNeural(3,3,1,1); /// QtdNeuEntrada, QtdNeuOculta, QtdNeuSaida, QtdCamadaOculta)

    Neuronio *x1 = &NN[0].CamadaEntrada.Neuronios[0];
    Neuronio *x2 = &NN[0].CamadaEntrada.Neuronios[1];
    Neuronio *bias = &NN[0].CamadaEntrada.Neuronios[2];

    Neuronio *Or = &NN[0].CamadaOculta[0].Neuronios[0];
    Neuronio *And = &NN[0].CamadaOculta[0].Neuronios[1];
    Neuronio *bias2 = &NN[0].CamadaOculta[0].Neuronios[2];

    x1->Saida = 1;
    x2->Saida = 0;
    bias->Saida = 1;

    x1->Peso[0] = 2;
    x2->Peso[0] = 2;
    bias->Peso[0] = -1;

    x1->Peso[1] = -1;
    x2->Peso[1] = -1;
    bias->Peso[1] = 1.5;

    Or->Peso[0] = 1;
    And->Peso[0] = 1;
    bias2->Peso[0] = -1.5;

    ///NN[0].CamadaOculta[0].Neuronios[0].Saida = g(NN[0].CamadaEntrada, 0);
    g(NN[0].CamadaOculta[0]);
    g(NN[0].CamadaSaida);
    ;printf("\n Ok ");
   // g(NN[0].CamadaOculta[1], 1);

    printf("\n| OR %.0f   |",  NN[0].CamadaOculta[0].Neuronios[0].Saida);
    printf("\n| NAND %.0f |",  NN[0].CamadaOculta[0].Neuronios[1].Saida);
    printf("\n| XOR %.0f  |",  NN[0].CamadaSaida.Neuronios[0].Saida);

}
