/**
 * Miguel Lima Tavares
 * DRE 119161571
 * Trabalho 2 Computacao Concorrente
 */

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include"arquivo.h"



/*---------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]){

    
  //recebe e valida os parametros de entrada (numero de sensores)
  if(argc < 2) {
      fprintf(stderr, "Digite: %s <numero de sensores>\n", argv[1]);
      return 1; 
  }
  LEITORES = atoi(argv[1]);
  ESCRITORES = LEITORES; // Quantidade de atuadores = sensores
  

  pthread_t tid[LEITORES + ESCRITORES];
  int id[LEITORES + ESCRITORES];
    

  //inicializa as variaveis de sincronizacao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_leit, NULL);
  pthread_cond_init(&cond_escr, NULL);

  //cria as threads leitoras
  for(int i=0; i<LEITORES; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, atuadores, (void *) &id[i])) exit(-1);
  } 

  //cria as threads escritoras
  for(int i=0; i<ESCRITORES; i++) {
    id[i+LEITORES] = i+1;
    if(pthread_create(&tid[i+LEITORES], NULL, sensores, (void *) &id[i+LEITORES])) exit(-1);
  } 

    
  //espera o fim das threads
  for(int i = 0; i < LEITORES + ESCRITORES; i++){
      pthread_join(tid[i], NULL);
  }

  exit(EXIT_SUCCESS);

  pthread_exit(NULL);
  return 0;

}