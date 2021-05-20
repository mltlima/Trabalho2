#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"arquivo.h"


//variaveis do problema
int leitores_count = 0; //contador de threads lendo
int escritores_count = 0; //contador de threads escrevendo
int leituras = 0;
int escritas = 0;
unsigned int shared_var = 0;

medicao temperaturas[60];
medicao ultimasTemperaturas[15];
int in=0;
int out=0;
int ultimaTemperatura=0;

int medirTemperatura(int id){
    // numero aleatorio (max_number + 1 - minimum_number) + minimum_number
    srand(escritas);//Usa a quantidade de leituras como seed
	int temperatura = rand() % (40 + 1 - 25) + 25;

    medicao novaTemperatura;
    novaTemperatura.idLeitura = escritas;
    novaTemperatura.temperatura = temperatura;
    novaTemperatura.idSensor = id;

    temperaturas[escritas % 61] = novaTemperatura;
    ultimasTemperaturas[escritas % 16] = novaTemperatura;

    

	return temperatura;
}


int lerTemperatura(int id){

    int temperaturaLida = 0;

    //procura entrea as ultimas (quantidade de escritores) leituras o id 
    for (int i = (escritas % 16) ; i >= 0; i--){
        
        if (ultimasTemperaturas[i].idSensor == id){
            //leitor com mesmo sensor do escritor achado
            //printf("teste ---------> %d \n", ultimasTemperaturas[i].temperatura);
            temperaturaLida = ultimasTemperaturas[i].temperatura;
            //passa o id da leitura
            ultimasTemperaturas[i].idLeitura = leituras;
            break;
        }
        
    }

    return temperaturaLida;
    
}


void alarme(){

    int amarelo = 0;
    int vermelho = 0;
    
    for (int i = 16 ; i >= 0; i--){
        
    }
}


//thread leitora
void * leitor (void * arg) {
    //int *id = (int *) arg;
    int id = *((int*)arg);
    int temperatura;

    for(int i = 1; i >= 0; i++){
        
        /* Enter critical section: */
        pthread_mutex_lock(&mutex);
        {
            pthread_cond_wait(&cond_leit, &mutex);
            sleep(2);
            leitores_count ++;
            temperatura = lerTemperatura(id);
            leituras++;

            //checa leitura valida
            if (!temperatura){
                printf("erro leitura de temperatura\n");
            }
            
            printf("LEITOR %i | Temperatura lida : %i | Readers count: %i ||| Ciclo: %i\n",
                    id, temperatura, leitores_count, i);
        }
        pthread_mutex_unlock(&mutex);

        if (leitores_count == ESCRITORES){
        pthread_mutex_lock(&mutex);
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
    free(arg);
    pthread_exit(0);

  
}

//thread escritora
void * escritor (void * arg) {

    int id = *((int*)arg);
    int temperatura = 0;
    //int *id = (int *) arg;
    for(int i = 1; i >= 0; i++){

        
       
        /* Enter critical section: */
        pthread_mutex_lock(&mutex);
        {
            sleep(1);
            escritas++;
            temperatura = medirTemperatura(id);
            //count++;
            escritores_count++;
            shared_var++;
            printf("ESCRITOR %i | Temperatura: %i | Writes count: %i ||| Ciclo: %i\n",
                id, temperatura, escritores_count, i);   
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
                pthread_cond_wait(&cond_escr, &mutex);
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    free(arg);
    pthread_exit(0);
}
