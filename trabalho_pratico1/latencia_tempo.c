/* Autor: Elias P. Duarte Jr.
   Data da Ãšltima ModificaÃ§Ã£o: 11/abril/2024
   DescriÃ§Ã£o: Nosso primeiro programa de simulaÃ§Ã£o da disciplina Sistemas DistribuÃ­dos.
     Vamos simular N processos, cada um conta o â€œtempoâ€ independentemente
    Um exemplo simples e significativo para captar o â€œespÃ­ritoâ€ da simulaÃ§Ã£o */

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"
#include "cisj.h"

// Vamos definir os EVENTOS
#define test 1
#define fault 2
#define recovery 3

// Vamos definir o descritor do processo

typedef struct
{
   int id; // identificador de facility do SMPL
   // outras variÃ¡veis locais dos processos sÃ£o declaradas aqui!
   int *STATE;
} TipoProcesso;

TipoProcesso *processo;

void imprimeState(int processo, int *state, int tamanho)
{
   printf("Vetor STATE do processo %d: ", processo);
   for (int i = 0; i < tamanho; i++)
   {
      printf("%d ", state[i]);
   }
   printf("\n");
}

void obtemDiagnostico(int token, int tokenTeste, TipoProcesso *processo, node_set *nodes)
{
   for (int nd = 0; nd < nodes->size; nd++)
   {
      if (nodes->nodes[nd] != tokenTeste)
      {
         int tokenDiag = nodes->nodes[nd];
         processo[token].STATE[tokenDiag] = processo[tokenTeste].STATE[tokenDiag];
      }
   }
}

int main(int argc, char *argv[])
{

   static int N, // nÃºmero de processos
       token,    // indica o processo que estÃ¡ executando
       event, r, i,
       MaxTempoSimulac = 150;

   static char fa_name[5];

   if (argc != 2)
   {
      puts("Uso correto: tempo <nÃºmero de processos>");
      exit(1);
   }

   N = atoi(argv[1]);

   smpl(0, "Um Exemplo de SimulaÃ§Ã£o");
   reset();
   stream(1);

   // inicializar processos

   processo = (TipoProcesso *)malloc(sizeof(TipoProcesso) * N);

   for (i = 0; i < N; i++)
   {
      memset(fa_name, '\0', 5);
      sprintf(fa_name, "%d", i);
      processo[i].id = facility(fa_name, 1);
   }
   // vamos fazer o escalonamento inicial de eventos

   // nossos processos vÃ£o executar testes em intervalos de testes
   // o intervalo de testes vai ser de 30 unidades de tempo
   // a simulaÃ§Ã£o comeÃ§a no tempo 0 (zero) e vamos escalonar o primeiro teste de todos os
   //       processos para o tempo 30.0

   for (i = 0; i < N; i++)
   {
      schedule(test, 30.0, i);
   }
   schedule(fault, 0.0, 0);
   // schedule(recovery, 61.0, 0);

   // agora vem o loop principal do simulador

   puts("===============================================================");
   puts("           Sistemas DistribuÃ­dos Prof. Elias");
   puts("          LOG do Trabalho PrÃ¡tico 0, Tarefa 0");
   puts("      Digitar, compilar e executar o programa tempo.c");
   printf("   Este programa foi executado para: N=%d processos.\n", N);
   printf("           Tempo Total de SimulaÃ§Ã£o = %d\n", MaxTempoSimulac);
   puts("===============================================================");

   // Inicializa vetor STATE de cada processo
   for (i = 0; i < N; i++)
   {
      // Aloca N posições pra cada STATE
      processo[i].STATE = (int *)malloc(sizeof(int) * N);

      // Seta o valor de todas as posições como -1
      for (int j = 0; j < N; j++)
      {
         if (i == j)
            processo[i].STATE[j] = 0;
         else
            processo[i].STATE[j] = -1;
      }
   }

   node_set *nodes;
   int finalizaLoop = 0;
   int latencia[N];
   for (int i = 0; i < N; i++)
   {
      latencia[i] = 1;
   }

   while (1)
   {

      cause(&event, &token);
      switch (event)
      {
      case test:
         // Se o processo está falho, não testa!
         if (status(processo[token].id) != 0)
            break;

         // Round
         for (int s = 1; s <= log2(N); s++)
         {
            nodes = cis(token, s);

            // Todos os testados
            for (int j = 0; j < nodes->size; j++)
            {
               int tokenTeste = nodes->nodes[j];

               // Se correto
               if (status(processo[tokenTeste].id) == 0)
               {

                  // Se tava marcado como errado/desconhecido, agora ta como certo
                  if (abs(processo[token].STATE[tokenTeste] % 2) == 1)
                  {
                     processo[token].STATE[tokenTeste]++;
                  }
                  // Obtem o diagostico do cluster?
                  obtemDiagnostico(token, tokenTeste, processo, nodes);


                  printf("O processo [%d] testou o processo [%d] correto no tempo %4.1f.\n", token, tokenTeste, time());
                  imprimeState(token, processo[token].STATE, N);

                  // Soma 1 rodada de teste na latência do processo
                  if ((processo[token].STATE[0] == -1) || (processo[token].STATE[0] == 0))
                     latencia[token]++;
                  break;
               }
               else
               {
                  // Se tava marcado como certo, agora ta como errado
                  if (abs(processo[token].STATE[tokenTeste] % 2) == 0)
                     processo[token].STATE[tokenTeste]++;
                  if (processo[token].STATE[tokenTeste] == -1)
                     processo[token].STATE[tokenTeste] = 1;

                  printf("O processo [%d] testou o processo [%d] falho no tempo %4.1f.\n", token, tokenTeste, time());
                  imprimeState(token, processo[token].STATE, N);

                  break;
               }
            }
         }

         // Encerra quando todos os processos tiverem detectado a falha
         finalizaLoop = 1;
         for (int i = 1; i < N; i++)
         {
            if ((processo[i].STATE[0] == -1) || (processo[i].STATE[0] == 0))
            {
               finalizaLoop = 0;
            }
         }

         schedule(test, 30.0, token);
         break;
      case fault:
         r = request(processo[token].id, token, 0);
         printf("Socooorro!!! Sou o processo %d  e estou falhando no tempo %4.1f\n", token, time());
         break;
      case recovery:
         release(processo[token].id, token);
         printf("Viva!!! Sou o processo %d e acabo de recuperar no tempo %4.1f\n", token, time());
         schedule(test, 1.0, token);
         break;
      } // switch

      if (finalizaLoop)
         break;
   } // while

   // Imprime vetor de latência
   for (int i = 1; i < N; i++)
   {
      printf("Latencia do processo %d: %d\n", i, latencia[i]);
   }
} // tempo.c