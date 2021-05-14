#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"arquivo.h"


//variaveis do problema
int leitores_count = 0; //contador de threads lendo
int escritores_count = 0; //contador de threads escrevendo
unsigned int shared_var = 0;

//Gera um delay de tempo aleatorio
void delay(){
    int delay;
    srand(time(NULL));
    delay = rand() % 4;
    sleep(delay);
    printf("delay de %d segundos\n", delay);
}

//thread leitora
void * leitor (void * arg) {
    //int *id = (int *) arg;
    int id = *((int*)arg);

    for(int i = 1; i <= LEITORES; i++)
    {
        delay();
        
        /* Enter critical section: */
        pthread_mutex_lock(&mutex);
        {
            while(escritores_count != 0)
                pthread_cond_wait(&cond_leit, &mutex);
            escritores_count = -1;
        }
        pthread_mutex_unlock(&mutex);

        /* Critical section, WRITE: */
        printf("LEITOR %i | New value: %i | Readers count: %i ||| Cycle: %i of %i\n",
               id, shared_var, leitores_count, i, ESCRITORES);

        /* Exit critical section: */
        pthread_mutex_lock(&mutex);
        {
            escritores_count = 0;
            pthread_cond_broadcast(&cond_escr);
            pthread_cond_signal(&cond_leit);
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);

  
}

//thread escritora
void * escritor (void * arg) {

    int id = *((int*)arg);
    //int *id = (int *) arg;
    for(int i = 1; i <= ESCRITORES; i++){

        delay();

        /* Enter critical section: */
        pthread_mutex_lock(&mutex);
        {
            if(escritores_count == -1)
                pthread_cond_wait(&cond_escr, &mutex);
            escritores_count++;
        }
        pthread_mutex_unlock(&mutex);

        /* Critical section, Escrita: */
        shared_var++;
        printf("ESCRITOR %i | Value: %i | Writes count: %i ||| Cycle: %i of %i\n",
               id, shared_var, escritores_count, i, ESCRITORES);

        /* Exit critical section: */
        pthread_mutex_lock(&mutex);
        {
            escritores_count--;
            if(escritores_count == 0)
            {
                pthread_cond_signal(&cond_leit);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
}
