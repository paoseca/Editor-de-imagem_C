#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct pixel
{
    int R, G, B;
} PIXEL;

PIXEL **inverteCores(PIXEL **original, int linhas, int colunas, int maxValor);
PIXEL **escalaCinza(PIXEL **original, int linhas, int colunas);
int **borrar(int **original, int linhas, int colunas, int tB);
void escreverImagem(char *nomeArquivo, PIXEL **mat, int linhas, int colunas, int maxValor);
PIXEL **lerImagem(char *nomeArquivo, int *pLinhas, int *pColunas, int *pMaxValor);
PIXEL **diminuirContraste(PIXEL **original, int linhas, int colunas, int maxValor);
PIXEL **aumentarContraste(PIXEL **original, int linhas, int colunas, int maxValor);
int **adicionaMoldura(PIXEL **original, int linhas, int colunas, int color);
int **diminuirBrilho(int **original, int linhas, int colunas);
int **aumentarBrilho(int **original, int linhas, int colunas, int maxValor);

int main(int argc, char *argv[])
{
    char opcao[10] = "0";
    int linhas = 0, colunas = 0, maxValor = 0;
    PIXEL **mat = NULL;
    int tamanhoBorrao = 8;
    char nomeArquivo[100] = "";
    char nomeArquivoLeitura[100] = "";
    char nomeArquivoEscrita[100] = "";
    while (opcao[0] != '11')
    {
        printf("\n\nMini-photoshop\n\n");
        printf("1) Ler imagem\n");
        printf("2) Gravar imagem\n");
        printf("3) Aumentar o brilho\n");
        printf("4) Diminuir o brilho\n");
        printf("5) Aumentar contraste\n");
        printf("6) Diminuir contraste\n");
        printf("7) Desfocar\n");
        printf("8) Fazer moldura\n");
        printf("9) Escala em tom de cinza\n");
        printf("10) Inverter cores\n");
        printf("11) Sair\n");
        printf("\nEntre a opcao desejada: ");
        fgets(opcao, 10, stdin);
        switch (opcao[0])
        {
        case '1':
            printf("\n\nEntre com o nome da imagem (sem extensao): ");
            fgets(nomeArquivo, 100, stdin);
            nomeArquivo[strlen(nomeArquivo) - 1] = '\0';
            strcpy(nomeArquivoLeitura, nomeArquivo);
            strcat(nomeArquivoLeitura, ".ppm");
            mat = lerImagem(nomeArquivoLeitura, &linhas, &colunas, &maxValor);
            break;
        case '2':
            strcpy(nomeArquivoEscrita, nomeArquivo);
            strcat(nomeArquivoEscrita, "_editada.ppm");
            printf("\n\nA imagem sera salva como %s\n", nomeArquivoEscrita);
            escreverImagem(nomeArquivoEscrita, mat, linhas, colunas, maxValor);
            break;
        case '3':
            mat = aumentarBrilho(mat, linhas, colunas, maxValor);
            break;
        case '4':
            mat = diminuirBrilho(mat, linhas, colunas);
            break;
        case '5':
            mat = aumentarContraste(mat, linhas, colunas, maxValor);
            break;
        case '6':
            mat = diminuirContraste(mat, linhas, colunas, maxValor);
            break;
        case '7':
            mat = borrar(mat, linhas, colunas, tamanhoBorrao);
            break;
        case '8':
            int color;
            printf("\nDigite a cor da moldura: ");
            printf("\n1 - Preto");
            printf("\n2 - Branco\n");
            scanf("%d", &color);
            mat = moldura(mat, linhas, colunas, maxValor, color);
            break;
        case '9':
            mat = escalaCinza(mat, linhas, colunas);
            break;
        case '10':
            mat = inverteCores(mat, linhas, colunas, maxValor);
            break;
        }
    }
    return 0;
}

int **aumentarBrilho(int **original, int linhas, int colunas, int maxValor)
{
    int l, c;
    // criando a matriz nova dinamicamente
    int **nova, i;
    nova = (int **)malloc(linhas * sizeof(int *));
    for (i = 0; i < linhas; i++)
        nova[i] = (int *)malloc(colunas * sizeof(int));

    // aumentando o brilho de cada pixel e armazenando na nova matriz
    for (l = 0; l < linhas; l++)
    {
        for (c = 0; c < colunas; c++)
        {
            nova[l][c] = (int)(original[l][c] * 1.2);
            if (nova[l][c] > maxValor)
                nova[l][c] = maxValor;
        }
    }

    return nova;
}

int **diminuirBrilho(int **original, int linhas, int colunas)
{
    int l, c;
    // criando a matriz nova dinamicamente
    int **nova, i;
    nova = (int **)malloc(linhas * sizeof(int *));
    for (i = 0; i < linhas; i++)
        nova[i] = (int *)malloc(colunas * sizeof(int));

    // diminuindo o brilho de cada pixel e armazenando na nova matriz
    for (l = 0; l < linhas; l++)
    {
        for (c = 0; c < colunas; c++)
        {
            nova[l][c] = (int)(original[l][c] / 1.2);
        }
    }

    return nova;
}

int **adicionaMoldura(PIXEL **original, int linhas, int colunas, int color)
{
    int l, c;
    int largura;

    printf("Digite a largura da moldura: ");
    scanf("%d", &largura);

    PIXEL **nova = (int **)malloc((linhas + 2 * largura) * sizeof(PIXEL *));
    for (l = 0; l < linhas + 2 * largura; l++)
    {
        nova[l] = (PIXEL *)malloc((colunas + 2 * largura) * sizeof(PIXEL));
    }

    switch (color)
    {
    case 1:
        for (l = 0; l < linhas + 2 * largura; l++)
        {
            for (c = 0; c < colunas + 2 * largura; c++)
            {
                if (l < largura || l >= linhas + largura || c < largura || c >= colunas + largura)
                {
                    nova[l][c].R = 0;
                    nova[l][c].G = 0;
                    nova[l][c].B = 0;
                }
                else
                {
                    nova[l][c] = original[l - largura][c - largura];
                }
            }
        }
        break;
    case 2:
        for (l = 0; l < linhas + 2 * largura; l++)
        {
            for (c = 0; c < colunas + 2 * largura; c++)
            {
                if (l < largura || l >= linhas + largura || c < largura || c >= colunas + largura)
                {
                    nova[l][c].R = 255;
                    nova[l][c].G = 255;
                    nova[l][c].B = 255;
                }
                else
                {
                    nova[l][c] = original[l - largura][c - largura];
                }
            }
        }
        break;
    default:
        printf("\nOpcao Invalida!");
        break;
    }

    return nova;
}

PIXEL **aumentarContraste(PIXEL **original, int linhas, int colunas, int maxValor)
{
    int l, c;
    double r, g, b;
    double novo_r, novo_g, novo_b;

    PIXEL **nova;
    int i;
    nova = (PIXEL **)malloc(linhas * sizeof(PIXEL *));
    for (i = 0; i < linhas; i++)
        nova[i] = (PIXEL *)malloc(colunas * sizeof(PIXEL));

    // Transforma cada canal de cor em um valor de ponto flutuante de 0 a 1
    for (l = 0; l < linhas; l++)
    {
        for (c = 0; c < colunas; c++)
        {
            r = (double)original[l][c].R / maxValor;
            g = (double)original[l][c].G / maxValor;
            b = (double)original[l][c].B / maxValor;

            // Aplica a fórmula para aumentar o contraste
            novo_r = 1.0 / (1.0 + exp(-(10.0 * r - 5.0)));
            novo_g = 1.0 / (1.0 + exp(-(10.0 * g - 5.0)));
            novo_b = 1.0 / (1.0 + exp(-(10.0 * b - 5.0)));

            // Transforma o resultado de volta para 0..255
            nova[l][c].R = (int)(novo_r * maxValor);
            nova[l][c].G = (int)(novo_g * maxValor);
            nova[l][c].B = (int)(novo_b * maxValor);
        }
    }
    return nova;
}

PIXEL **diminuirContraste(PIXEL **original, int linhas, int colunas, int maxValor)
{
    int l, c;
    double r, g, b;
    double novo_r, novo_g, novo_b;

    PIXEL **nova;
    int i;
    nova = (PIXEL **)malloc(linhas * sizeof(PIXEL *));
    for (i = 0; i < linhas; i++)
        nova[i] = (PIXEL *)malloc(colunas * sizeof(PIXEL));

    // Transforma cada canal de cor em um valor de ponto flutuante de 0 a 1
    for (l = 0; l < linhas; l++)
    {
        for (c = 0; c < colunas; c++)
        {
            r = (double)original[l][c].R / maxValor;
            g = (double)original[l][c].G / maxValor;
            b = (double)original[l][c].B / maxValor;

            // Aplica a fórmula para diminuir o contraste
            novo_r = 1.0 / (1.0 + exp(2.0 - (10.0 * r - 5.0)));
            novo_g = 1.0 / (1.0 + exp(2.0 - (10.0 * g - 5.0)));
            novo_b = 1.0 / (1.0 + exp(2.0 - (10.0 * b - 5.0)));

            // Transforma o resultado de volta para 0..255
            nova[l][c].R = (int)(novo_r * maxValor);
            nova[l][c].G = (int)(novo_g * maxValor);
            nova[l][c].B = (int)(novo_b * maxValor);
        }
    }
    return nova;
}

PIXEL **lerImagem(char *nomeArquivo, int *pLinhas, int *pColunas, int *pMaxValor)
{
    FILE *fp;
    fp = fopen(nomeArquivo, "r");
    /* Arquivo ASCII, para leitura */
    if (!fp)
    {
        printf("\nErro na abertura do arquivo\n\n");
        exit(-1);
    }
    // leia tipo do arquivo
    char buffer[1001];
    fgets(buffer, 1000, fp);          // Primeira linha
    if (strstr(buffer, "P3") == NULL) // o tipo de arquivo eh diferente de P3?
    {
        printf("\nErro no tipo do arquivo\n\n");
        exit(-2);
    }
    // leia comentario
    fgets(buffer, 1000, fp);

    // leia dados da imagem
    fscanf(fp, "%d%d%d", pColunas, pLinhas, pMaxValor);
    // criando a matriz
    PIXEL **mat;
    int i;
    mat = (PIXEL **)malloc(*pLinhas * sizeof(PIXEL *));
    for (i = 0; i < *pLinhas; i++)
        mat[i] = (PIXEL *)malloc(*pColunas * sizeof(PIXEL));
    int l, c;
    for (l = 0; l < *pLinhas; l++)
    {
        for (c = 0; c < *pColunas; c++)
        {
            fscanf(fp, "%d", &mat[l][c].R);
            fscanf(fp, "%d", &mat[l][c].G);
            fscanf(fp, "%d", &mat[l][c].B);
        }
    }
    fclose(fp);
    return mat;
}
void escreverImagem(char *nomeArquivo, PIXEL **mat, int linhas, int colunas, int maxValor)
{
    FILE *fp;
    fp = fopen(nomeArquivo, "w");
    // Arquivo ASCII, para leitura
    if (!fp)
    {
        printf("\nErro na abertura do arquivo\n\n");
        exit(-1);
    }
    // escreva tipo do arquivo
    fprintf(fp, "P3\n");
    // escreva comentario
    fprintf(fp, "#Figura modificada...\n");
    // colunas, linhas
    fprintf(fp, "%d %d\n", colunas, linhas);
    // maxValor
    fprintf(fp, "%d\n", maxValor);
    int l, c;
    for (l = 0; l < linhas; l++)
    {
        for (c = 0; c < colunas; c++)
            fprintf(fp, "%d\n%d\n%d\n", mat[l][c].R, mat[l][c].G, mat[l][c].B);
    }
    fclose(fp);
}

int **borrar(int **original, int linhas, int colunas, int tB)
{
    int l, c;
    // criando a matriz nova dinamicamente
    int **nova, i;
    nova = (int **)malloc(linhas * sizeof(int *));
    for (i = 0; i < linhas; i++)
        nova[i] = (int *)malloc(colunas * sizeof(int));

    for (l = tB; l < linhas - tB; l++)
        for (c = tB; c < colunas - tB; c++)
            nova[l][c] = (original[l][c] + original[l + tB][c] + original[l - tB][c] + original[l][c + tB] + original[l][c - tB]) / 5;
    return nova;
}

PIXEL **escalaCinza(PIXEL **original, int linhas, int colunas)
{
    // Alocar nova matriz dinamicamente
    PIXEL **nova = (PIXEL **)malloc(linhas * sizeof(PIXEL *));
    for (int i = 0; i < linhas; i++)
    {
        nova[i] = (PIXEL *)malloc(colunas * sizeof(PIXEL));
    }

    // Converter cada pixel para escala de cinza
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            int R = original[i][j].R;
            int G = original[i][j].G;
            int B = original[i][j].B;
            int gray = (int)(0.3 * R + 0.59 * G + 0.11 * B);
            nova[i][j].R = gray;
            nova[i][j].G = gray;
            nova[i][j].B = gray;
        }
    }
    return nova;
}

PIXEL **inverteCores(PIXEL **original, int linhas, int colunas, int maxValor)
{
    PIXEL **nova;
    int i, j;
    nova = (PIXEL **)malloc(linhas * sizeof(PIXEL *));
    for (i = 0; i < linhas; i++)
    {
        nova[i] = (PIXEL *)malloc(colunas * sizeof(PIXEL));
        for (j = 0; j < colunas; j++)
        {
            nova[i][j].R = maxValor - original[i][j].R;
            nova[i][j].G = maxValor - original[i][j].G;
            nova[i][j].B = maxValor - original[i][j].B;
        }
    }
    return nova;
}