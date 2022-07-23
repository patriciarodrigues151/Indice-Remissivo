#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAMTAB 431
#define MAXWORD 46


typedef struct
{
	int linha;
	int ocorrencia;
}infoOcorrencia;

typedef infoOcorrencia item2;

//célula de ocorrência
typedef struct cel2 {
  item2 info2;
  struct cel2 * prox;
} celula2; 


typedef struct
{
	char* conteudo;
	celula2* listaOcorre;
} infoConteudo;

typedef infoConteudo item1;

//célula da palavra
typedef struct cel1 {
  item1 info1;
  struct cel1 * prox;
} celula1; 

void imprimePalavras(celula1 ** inicio, int n);
celula1 * busca1 (celula1 * inicio, char* palavra);
celula1 * insereNoFim1 (celula1 * inicio, item1 x);
celula2 * busca2 (celula2 * inicio, int linha);
celula2 * insereNoFim2 (celula2 * inicio, item2 x);
int compara(char* palavra1, char* palavra2);
celula1 ** bubble(celula1 **vPonteiro, int n);

int funcHash(char* palavra)
{
		int numChar;
		int i;
		int soma = 0;
		for(i = 0;i < strlen(palavra);i++)
		{
			numChar = palavra[i];
			numChar = numChar - 64;
			
			numChar = numChar * pow(-1, i);
			soma = soma + numChar;
		}
		soma = abs(soma);
		soma = soma % TAMTAB;
		return soma;
}


int main () {
	FILE *p;
	celula1** hash = malloc(TAMTAB * sizeof(celula1 *));
	int i;
	for(i = 0; i < TAMTAB;i++)
	{
		hash[i] == NULL;
	}
	
	char letra;
	char palavra[46];
	char auxPalavra[46];
	int cont;
	char arquivo[50];
	printf("Digite o nome do arquivo: ");
	scanf("%s", arquivo);
	p = fopen(arquivo, "r");
	int qntdPalavras = 0;
	int indice = 0;
	int posicao;
	item1 contInsere;
    contInsere.conteudo = malloc(MAXWORD * sizeof(char));
	item2 ocorrInsere;
	int linhaAtual = 1;
	while(fread(&letra, sizeof(char), 1, p) == 1)
	{
		
		if(letra == ' ' || letra == '.' || letra == '\n' || letra == '!' || letra == '?' || letra == ':' || letra == ';' || letra == ',')
		{
            if(indice != 0)
			{
                auxPalavra[indice] = '\0';
                strncpy(palavra, auxPalavra, indice+1);
                
                posicao = funcHash(palavra);
                if(busca1(hash[posicao], palavra) == NULL)
                {
                    strcpy(contInsere.conteudo, palavra);
                    contInsere.listaOcorre = NULL;
                    hash[posicao] = insereNoFim1(hash[posicao], contInsere);
                    ocorrInsere.linha = linhaAtual;
                    ocorrInsere.ocorrencia = 1;
                    busca1(hash[posicao], palavra)->info1.listaOcorre = insereNoFim2(busca1(hash[posicao], palavra)->info1.listaOcorre, ocorrInsere);
                    qntdPalavras++;
                }
                else
                {
                    celula1* aux;
                    
                    aux = busca1(hash[posicao], palavra);
                    if(busca2(aux->info1.listaOcorre, linhaAtual) == NULL)
                    {
                        ocorrInsere.linha = linhaAtual;
                        ocorrInsere.ocorrencia = 1;
                        aux->info1.listaOcorre = insereNoFim2(aux->info1.listaOcorre, ocorrInsere);
                    }
                    else
                    {
                        celula2* aux2;
                        
                        aux2 = busca2(aux->info1.listaOcorre, linhaAtual);
                        aux2->info2.ocorrencia++;
                    }
                }
                indice = 0;	
            }
		}
		else
		{
			auxPalavra[indice] = letra;			
			indice++;
		}

        if(letra == '\n')
		{
			linhaAtual++;
		}				
	}
	
if(indice != 0)
{
    auxPalavra[indice] = '\0';
    strncpy(palavra, auxPalavra, indice+1);
    
    posicao = funcHash(palavra);
    if(busca1(hash[posicao], palavra) == NULL)
    {
        strcpy(contInsere.conteudo, palavra);
        contInsere.listaOcorre = NULL;
        hash[posicao] = insereNoFim1(hash[posicao], contInsere);
        ocorrInsere.linha = linhaAtual;
        ocorrInsere.ocorrencia = 1;
        busca1(hash[posicao], palavra)->info1.listaOcorre = insereNoFim2(busca1(hash[posicao], palavra)->info1.listaOcorre, ocorrInsere);
        qntdPalavras++;
    }
    else
    {
        celula1* aux;
        
        aux = busca1(hash[posicao], palavra);
        if(busca2(aux->info1.listaOcorre, linhaAtual) == NULL)
        {
            ocorrInsere.linha = linhaAtual;
            ocorrInsere.ocorrencia = 1;
            aux->info1.listaOcorre = insereNoFim2(aux->info1.listaOcorre, ocorrInsere);
        }
        else
        {
            celula2* aux2;
            
            aux2 = busca2(aux->info1.listaOcorre, linhaAtual);
            aux2->info2.ocorrencia++;
        }
    }
    indice = 0;	
}
        
        
	celula1 ** pontPalavras;
	
	pontPalavras = malloc(qntdPalavras * sizeof(celula1*));
	
	int posPontPalavras = 0;
	for(i = 0; i < TAMTAB;i++)
	{
		while(hash[i] != NULL)
		{
			
			pontPalavras[posPontPalavras]= hash[i];
			posPontPalavras++;
			hash[i] = hash[i]->prox;					
		}
	}
	
	pontPalavras = bubble(pontPalavras, qntdPalavras);

	imprimePalavras(pontPalavras, qntdPalavras);
	fclose(p);
	
    return(0);
}


celula1 ** bubble(celula1 **vPonteiro, int n)
{
	celula1* aux;
	
	int i, j;	
	for(i = 0; i < n - 1; i++)
	{
		for(j = 0; j < n-i-1; j++)
		{
			if(compara(vPonteiro[j]->info1.conteudo, vPonteiro[j+1]->info1.conteudo) > 0)
			{
				aux = vPonteiro[j];
				vPonteiro[j] = vPonteiro[j+1];
				vPonteiro[j+1] = aux;
			}
		}
	}
	
	return vPonteiro;
}

void imprimePalavras(celula1 ** inicio, int n) 
{
    celula2 * celulaOcorre;
	int i;
	for(i = 0; i < n;i++)
	{
		printf("%s: ",  inicio[i]->info1.conteudo);
        celulaOcorre = inicio[i]->info1.listaOcorre;
        while(celulaOcorre != NULL){
            if(celulaOcorre->info2.ocorrencia == 1){
                printf("%d ", celulaOcorre->info2.linha);
            } else {
                printf("%d(%d) ", celulaOcorre->info2.linha, celulaOcorre->info2.ocorrencia);
            }
            celulaOcorre = celulaOcorre->prox;
        }
        printf("\n");
	}
}

int compara(char* palavra1, char* palavra2)
{
  	int i;
	int diferenca;
	int tamPalavra1;
	int tamPalavra2;
	int tamanho;
	int numeroChar, numeroChar2;
	int escolhe1, escolhe2;
	tamPalavra1 = strlen(palavra1);
	tamPalavra2 = strlen(palavra2);
	
	diferenca = tamPalavra1 - tamPalavra2;
	
	if(diferenca < 0)
	{
		tamanho = tamPalavra1;
	}
	else
	{
		tamanho = tamPalavra2;
	}
	
	
	for(i = 0; i < tamanho; i++)
	{
		numeroChar2 = palavra2[i];
		numeroChar = palavra1[i];
		if(palavra1[i] > 64 && palavra1[i] < 91)
		{
			numeroChar = numeroChar + 32;
		}
		
		if(palavra2[i] > 64 && palavra2[i] < 91)
		{
			numeroChar2 = numeroChar2 + 32;
		}
				
		if(numeroChar > numeroChar2)
		{
			return 1;
		}
		else if(numeroChar < numeroChar2)
		{
			return -1;
		}
    }
    if(tamPalavra1 > tamPalavra2)
    {
        return 1;
    } 
    else 
    {
        return -1;
    }
}

celula1 * busca1 (celula1 * inicio, char* palavra) {
  if (inicio == NULL || strcmp(inicio->info1.conteudo, palavra) == 0)
    return inicio;
  return (busca1 (inicio->prox, palavra));
}


celula1 * insereNoFim1 (celula1 * inicio, item1 x) {
  celula1 *novo = malloc(sizeof (celula1));
  novo->info1.conteudo = malloc(MAXWORD * sizeof(char));
  celula1 *atual, *ant; 
  strcpy(novo->info1.conteudo, x.conteudo);
  novo->info1.listaOcorre = x.listaOcorre;
  novo->prox = NULL; 
  for (atual = inicio, ant = NULL; atual != NULL;
       ant = atual, atual = atual->prox);
  if (ant != NULL)
    ant -> prox = novo;
  else
    inicio = novo;
  return (inicio);
}


celula2 * busca2 (celula2 * inicio, int linha) {
  if (inicio == NULL || inicio->info2.linha == linha)
    return inicio;
  return (busca2 (inicio->prox, linha));
}


celula2 * insereNoFim2 (celula2 * inicio, item2 x) {
  celula2 *novo = malloc(sizeof (celula2));
  celula2 *atual, *ant; 
  novo->info2.linha = x.linha;
  novo->info2.ocorrencia = x.ocorrencia;
  novo->prox = NULL; 
  for (atual = inicio, ant = NULL; atual != NULL;
       ant = atual, atual = atual->prox);
  if (ant != NULL)
    ant -> prox = novo;
  else
    inicio = novo;
  return (inicio);
}

