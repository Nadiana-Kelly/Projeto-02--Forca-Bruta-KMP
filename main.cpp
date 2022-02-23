#include <iostream>
#include <new>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include "instancias_Reais_Trabalho_2.hpp"

using std::clock; using std::clock_t;
using namespace std;

/////////////////////////ALGORITMOS///////////////////////////////
//algoritmo de força bruta: o mesmo feito pelo prof em aula
bool foca_bruta (const char *p, const char *t, int *o){
  int *s = o;
  for(const char *i = t; *i!= '\0'; ++i){
    const char *j = i, *k = p;
    while(*k!= '\0' &&  *j == *k){
      ++j;
      ++k;
    }
    if(*k == '\0'){
      *s = i - t;
      ++s;
    }
  }
  *s = -1;
  return true;
}

//função usada pelo algoritmo KMP - função do prefixo
void prefix(const char *p, int *aux){
  int m = strlen(p);
	aux[0] = 0; //aray que vai conter as posições dos suf. que tbm é pref.
	int j = 0, i = 1;

	while(i < m){
		if(p[i] == p[j]){
			j++;
			aux[i] = j;
			i++;
		}else{
			if(j!= 0){
        j = aux[j - 1];
      }else{
				aux[i] = 0;
				i++;
			}
		}
	}
}

//algorimto KMP
bool kmp(const char *p, const char *t, int *o){
	int i_t = 0, i_p = 0, k = 0;
  int n = strlen(t); //calcula o tamanho do vetor t
  int m = strlen(p); //calcula o tamanho do vetor p

  int *aux = new(nothrow) int[m];
  //testando alocação dinâmica (1 - true, 0 - false)
  if( aux == nullptr){
    return false;
  }

  prefix(p,aux); //função do prefixo 

	while(i_t < n){
		if(p[i_p] == t[i_t]){ //se ocorrer um casamento dos caracteres de p e t então é incrementando seus índices
			i_p++;
			i_t++;
		}
    //se ocorer casamento do p e do t a posição é salva em o 
		if(i_p ==m){ 
      o[k] = i_t - i_p;
			i_p = aux[i_p - 1];
      k++;
      o[k] = -1; //(aqui eu estou sempre colocando -1 a frente depois de achar uma ocorrência)
		
    //caso os caracteres sejam diferentes
    }else if(i_t < n && p[i_p] != t[i_t]){ 
      if(i_p != 0){
        //se eu tiver algum casamento antes, então pelo padrão eu sei por onde voltar
        i_p = aux[i_p - 1]; 
      }else{
        //incremento para analisar o próximo caracter do texto
        i_t++;
      }
    }
  }
  delete[] aux;
  return true;
}


////////////////////////////INSTÂNCIAS////////////////////////////
//funcao pra gerar letras aleatorias
char char_aleatorio(char l, char *alfabeto){
  int y;
  int sorteiaAte; // valor do indice do vetor que corresp. a l
  char letraSorteada; //letra sorteada
  int valorsorteado; // recebe o indice da letra sorteada
  for(y = 0; y<26; y++){
    if(alfabeto[y] == l){
      sorteiaAte = y;
    }
  }
  //caso a letra passada seja 'a' que estará na posição 0 do vetor
  if(sorteiaAte == 0){
    valorsorteado = 0;
  }else{
    valorsorteado = rand()%sorteiaAte;
  }
  for(y=0;y<26;y++){
    if( y == valorsorteado){
      letraSorteada = alfabeto[y];
    }
  }
  return letraSorteada;
}

//funcao para a Instancia aleatoria 
void inst_aleatoria(char *t, char *p, int n, int m, char l, char *alfabeto){
  int y;
  //preenchendo o vetor de texto
  for(y = 0; y<n; y++){
    t[y] = char_aleatorio(l, alfabeto);
  }
  //preenchendo o vetor do padrao
  for(y = 0; y<m; y++){
    p[y] = char_aleatorio(l, alfabeto);
  }

}
//função para verificar se os vetores das ocorrência do KMP e do FB são iguais
bool verifica_Ocorrencias(int *oFB, int *oKMP){
  int y = 0;
  while(oFB[y] != -1 && oKMP[y]!= -1){
    if(oFB[y] != oKMP[y]){
      return false; //0
    }
    y++;
  }
  return true; //1
}


/////////////////////CALCULAR O TEMPO/////////////////////////////
//funcao para calcular o tempo do Força bruta
double tempoFB(const char *p, const char *t, int *o){
  clock_t tempo_inicio_FB, tempo_final_FB;
  double tempo_total_FB;

  tempo_inicio_FB = clock();
  foca_bruta(p, t, o);
  tempo_final_FB = clock();

  tempo_total_FB = (tempo_final_FB - tempo_inicio_FB) /  (double) CLOCKS_PER_SEC;
  return  tempo_total_FB ;
}

//funcao para calcular o tempo do KMP
double tempoKMP (const char *p, const char *t, int *o){
  clock_t tempo_inicio_KMP, tempo_final_KMP;
  double tempo_total_KMP;

  tempo_inicio_KMP = clock();
  kmp(p, t, o);
  tempo_final_KMP = clock();

  tempo_total_KMP = (tempo_final_KMP - tempo_inicio_KMP) /  (double) CLOCKS_PER_SEC;
  return  tempo_total_KMP;
}

///////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
  srand(time(NULL)); //pra tentar gerar valores aleat. diferentes

  //variaveis para armazenar o tempo dos algoritmos
  double tempo_total_FB = 0; //tempo final do FB
	double tempo_total_KMP = 0; //tempo final do KMP

  //CASO PARA INSTÂNCIAS ALEATÓRIAS 
  if(argv[1][0] != 'R'){

    int i = atoi(argv[5]); //número de instâncias
    char l = argv[2][0]; // letra de "a" a "z"

    //vetor com o alfabeto (usado pra sortear os 'char' para t e p)
    char alfabeto[26]= {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    int n = atoi(argv[4]); // tamanho do texto
    int m = atoi(argv[3]); //tamanho do padrão
    int *oFB, *oKMP; // vetores de ocorrencia
    char *t, *p; // vetor de texto e padrão;

    int j; //indice para uso dos for
    oFB = new int[n+1]; //vetor de ocorrências do FB
    oKMP= new int[n+1]; //vetor de ocorrências do KMP

    for(j = 0; j < n+1; j++){
      oFB[i] = 0;
    }
    for(j = 0; j < n+1; j++){
      oKMP[i] = 0;
    }

    t = new char[n];
    p = new char[m];

    for(j= 0; j<i; j++){
      //gerando instacia aleatoria para p e t
      inst_aleatoria(t, p, n, m, l, alfabeto);
      //calculando o tempo do FB
      tempo_total_FB += tempoFB(p, t, oFB);
      //calcuando o tempo do KMP
      tempo_total_KMP += tempoKMP(p, t, oKMP);

      //verifica se os vetor de ocorrências é igual 
      //(1 - true e 0 - false)
      //cout << verifica_Ocorrencias(oFB, oKMP);
    } 

    cout << "\nForca Bruta: "<< tempo_total_FB;
	  cout << "\nKMP: "<< tempo_total_KMP;

    //desalocando os vetores usados
    delete[] t;
    delete[] p;
    delete[] oFB;
    delete[] oKMP;

  //CASO PARA INSTÂNCIA REAL
  }else{

    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    int i;
    int *oFB = new int[35129 + 1];
    int *oKMP = new int[35129 + 1];

    int j; //indice para uso dos for
    //preenchendo os vetores de ocorrência com 0
    for(j = 0; j < 35129+1; j++){
      oFB[j] = 0;
    }
    for(j = 0; j < 35129+1; j++){
      oKMP[j] = 0;
    }

    for(i = x;i <=y; i++){
      //calculando o tempo do força bruta
      tempo_total_FB += tempoFB(Padroes_Palavras[i], Texto_Livros, oFB);
      //calculando o tempo do KMP
      tempo_total_KMP += tempoKMP(Padroes_Palavras[i], 
      Texto_Livros, oKMP);
      
      //verifica se os vetor de ocorrências é igual
      //(1 - true e 0 - false)
      //cout << verifica_Ocorrencias(oFB, oKMP);

    }

    cout << "\nForca Bruta: "<< tempo_total_FB;
	  cout << "\nKMP: "<< tempo_total_KMP;

    //desalocando os vetores usados
    delete[] oFB;
    delete[] oKMP;

  }

}
