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

medicao temperaturas[60];
int leituras=0;
int escritoras = 0;
int in=0;
int out=0;
medicao ultimasTemperaturas[15];
int ultimaTemperatura=0;

int medirTemperatura(){
    // numero aleatorio (max_number + 1 - minimum_number) + minimum_number
    srand(escritoras);//Usa id como 
	int temperatura = rand() % (40 + 1 - 25) + 25;
	return temperatura;
}


//thread leitora
void * leitor (void * arg) {
    //int *id = (int *) arg;
    int id = *((int*)arg);

    for(int i = 1; i <= LEITORES; i++)
    {
        
        /* Enter critical section: */
        pthread_mutex_lock(&mutex);
        {
            //while(escritores_count != 0)
                pthread_cond_wait(&cond_leit, &mutex);
            //escritores_count = -1;
        }
        pthread_mutex_unlock(&mutex);


        leitores_count ++;
        /* Critical section, WRITE: */
        printf("LEITOR %i | New value: %i | Readers count: %i ||| Cycle: %i of %i\n",
               id, shared_var, leitores_count, i, ESCRITORES);

        if (leitores_count == ESCRITORES){
        pthread_mutex_lock(&mutex);
        sleep(2);
            pthread_cond_broadcast(&cond_escr);
            leitores_count = 0;
        pthread_mutex_unlock(&mutex);
        }
        

        /* Exit critical section: */
        /*
        pthread_mutex_lock(&mutex);
        {
            escritores_count = 0;
            pthread_cond_broadcast(&cond_escr);
            pthread_cond_signal(&cond_leit);
        }
        pthread_mutex_unlock(&mutex);
        */
    }

    pthread_exit(0);

  
}

//thread escritora
void * escritor (void * arg) {

    int id = *((int*)arg);
    int temperatura = 0;
    //int *id = (int *) arg;
    for(int i = 1; i <= ESCRITORES; i++){

        sleep(1);
        temperatura = medirTemperatura();
        escritoras++;
       
        /* Enter critical section: */
        pthread_mutex_lock(&mutex);
        {
            escritores_count++;
            shared_var++;
            printf("ESCRITOR %i | Temperatura: %i | Writes count: %i ||| Cycle: %i of %i\n",
                id, temperatura, escritores_count, i, ESCRITORES);   
            //sleep(1);
            if(escritores_count != ESCRITORES)
                pthread_cond_wait(&cond_escr, &mutex);
            
        }
        pthread_mutex_unlock(&mutex);

        /* Critical section, Escrita: */
        
            //printf("ESCRITOR %i | Value: %i | Writes count: %i ||| Cycle: %i of %i\n",
            //    id, shared_var, escritores_count, i, ESCRITORES);
        //shared_var++;
        //printf("ESCRITOR %i | Value: %i | Writes count: %i ||| Cycle: %i of %i\n",
        //       id, shared_var, escritores_count, i, ESCRITORES);

        /* Exit critical section: */
        {
            //escritores_count--;
            if(escritores_count == 3)
            {   
                pthread_mutex_lock(&mutex);
                printf("condicao alcancada\n");
                //pthread_cond_signal(&cond_leit);
                //pthread_cond_broadcast(&cond_escr);
                pthread_cond_broadcast(&cond_leit);
                escritores_count = 0;
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    pthread_exit(0);
}
