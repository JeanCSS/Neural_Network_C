
#define OBSTACULOS_SPRITE_QUANTIDADE    8
#define QTD_COR_BIRDS                   6
#define QTD_SPRITES_BIRDS               3

typedef struct sprite
{
    int Largura, Altura;
    int Objeto;

}   Sprite;

    Sprite SpriteCano;
    Sprite SpriteChao;
    Sprite SpriteBirds[6][3];
    Sprite SpriteNuvem, SpriteArvore, SpritePredio;
    Sprite SpriteParaquedas;

    int SpriteNeuronDesativado;
    int SpriteNeuronAtivado;
    int SpriteLuz;
    int SpriteSeta;


void InicializarSpriteCano()
{
}

void InicializarSpritesBird()
{

}

void InicializarSpritesNeuronios()
{

}

void InicializarSpriteChao()
{
}

void InicializarSpriteNuvem()
{

}

void InicializarSpritePredio()
{
}

void InicializarSpriteArvore()
{
}

void InicializarSpriteParaquedas()
{

}

void InicializarSprites()
{
    InicializarSpriteNuvem();
    InicializarSpritePredio();
    InicializarSpriteArvore();

    InicializarSpriteChao();

    InicializarSpriteCano();
    InicializarSpritesBird();
    InicializarSpritesNeuronios();

    InicializarSpriteParaquedas();

}


Sprite* getPassaroSprite(int Indice, int Cor)
{
    return &SpriteBirds[Cor][Indice];
}



