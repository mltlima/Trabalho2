#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include"arquivo.h"


/*---------------------------------------------------------------------------------------*/
int main(void){

    pthread_t tid[LEITORES + ESCRITORES];
    int id[LEITORES + ESCRITORES];

    //seed aleatoria
    //srand(time(NULL));
    

    //inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);

    //cria as threads leitoras
    for(int i=0; i<LEITORES; i++) {
      id[i] = i+1;
      if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
    } 

    //cria as threads escritoras
    for(int i=0; i<ESCRITORES; i++) {
      id[i+LEITORES] = i+1;
      if(pthread_create(&tid[i+LEITORES], NULL, escritor, (void *) &id[i+LEITORES])) exit(-1);
    } 

    

   

    /* 4. Wait for readers' & writers' termination: */
    for(int i = 0; i < LEITORES + ESCRITORES; i++)
    {
        pthread_join(tid[i], NULL);
    }

    exit(EXIT_SUCCESS);

    pthread_exit(NULL);
    return 0;

}