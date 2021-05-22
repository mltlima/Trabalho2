/**
 * Miguel Lima Tavares
 * DRE 119161571
 * Trabalho 2 Computacao Concorrente
 */

#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"arquivo.h"


//variaveis do problema
int leitores_count = 0; //contador de threads lendo
int escritores_count = 0; //contador de threads escrevendo
int leituras = 0; //total de leituras
int escritas = 0; //total de escritas
int somatorio = 0; //somatorio total temperaturas lidas
int media = 0; //media das temeperaturas lidas

medicao temperaturas[60]; //guarda as ultimas 60 leituras de temperatura
medicao ultimasTemperaturas[15]; //guarda as ultimas 15 leituras de temperatura



/*---------------------------------------------------------------------------------------*/
//gera um numero aleatorio para temperatura entre 25 e 40
int medirTemperatura(int id){
    // numero aleatorio (max_number + 1 - minimum_number) + minimum_number
    srand(escritas);//Usa a quantidade de leituras como seed
	int temperatura = rand() % (40 + 1 - 25) + 25;

    medicao novaTemperatura;
    novaTemperatura.idLeitura = escritas;
    novaTemperatura.temperatura = temperatura;
    novaTemperatura.idSensor = id;

    temperaturas[escritas % 60] = novaTemperatura;
    ultimasTemperaturas[escritas % 15] = novaTemperatura;

    

	return temperatura;
}


/*---------------------------------------------------------------------------------------*/
//le a temperatura somante do sensor com mesmo id do leitor 
int lerTemperatura(int id){

    int temperaturaLida = 0;

    //procura entrea as ultimas (quantidade de escritores) leituras o id 
    for (int i = ((escritas-LEITORES) % 60) ; i < leituras+LEITORES; i++){
        if (temperaturas[i].idSensor == id){
            //leitor com mesmo sensor do escritor achado
            temperaturaLida = temperaturas[i].temperatura;
            //passa o id da leitura
            temperaturas[i].idLeitura = leituras;
            break;
        }
        
    }

    return temperaturaLida;
    
}


/*---------------------------------------------------------------------------------------*/
//alerta se a temperatura condizer com os requisitos:
//ultimas 15 leituras 5 sao acima de 35 ALERTA AMARELO
//ultimas 5 leituras sao acima de 35 ALERTA VERMELHO
void alarme(){

    int id = (((escritas-15-LEITORES) % 15) + 15) % 15;

    int amarelo = 0;
    int vermelho = 0;

    for (int i = 1 ; i <  16; i++){

        id = (((escritas-15-LEITORES+i) % 15)+15)%15 ;  
        int temperatura = ultimasTemperaturas[id].temperatura; 

        if (temperatura > 35){
            amarelo++;
            vermelho++;
        }else {
            vermelho = 0;
        }
    }

    if (vermelho >= 5){
        printf("ALERTA VERMELHO\n");
    } else if (amarelo > 5){
        printf("ALERTA AMARELO\n");
    } else {
        printf("ALERTA NORMAL\n");
    }
    
    
}


/*---------------------------------------------------------------------------------------*/
//thread leitora
void * atuadores (void * arg) {
    
    int id = *((int*)arg);
    int temperatura;

    for(int i = 0; i >= 0; i++){//loop infinito
        
        //entra secao critica
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

            somatorio += temperatura;
            media = somatorio/leituras;
            
            printf("LEITOR %i | Temperatura lida : %i | Media: %i ||| Ciclo: %i\n",
                    id, temperatura, media, i);
        }
        pthread_mutex_unlock(&mutex);

        if (leitores_count == ESCRITORES){
        pthread_mutex_lock(&mutex);
            alarme();
            //libera os sensores
            pthread_cond_broadcast(&cond_escr);
            leitores_count = 0;
        pthread_mutex_unlock(&mutex);
        }
        
    }
    free(arg);
    pthread_exit(0);

  
}


/*---------------------------------------------------------------------------------------*/
//thread escritora
void * sensores (void * arg) {

    int id = *((int*)arg);
    int temperatura = 0;
    
    for(int i = 0; i >= 0; i++){

        
       
        //entra na secao critica
        pthread_mutex_lock(&mutex);
        {
            sleep(1);
            temperatura = medirTemperatura(id);
            escritas++;
            escritores_count++;

            printf("ESCRITOR %i | Temperatura: %i | Media: %i ||| Ciclo: %i\n",
                id, temperatura, media, i);   
            //lock esperando a ultima thread
            if(escritores_count != ESCRITORES)
                pthread_cond_wait(&cond_escr, &mutex);
            
        }
        pthread_mutex_unlock(&mutex);

        //espera chegar na ultima thread para liberar os atuadores
        if(escritores_count == ESCRITORES)
        {   
            pthread_mutex_lock(&mutex);
            printf("condicao alcancada\n");
            pthread_cond_broadcast(&cond_leit);
            escritores_count = 0;
            pthread_cond_wait(&cond_escr, &mutex);
            pthread_mutex_unlock(&mutex);
        }
        
    }
    free(arg);
    pthread_exit(0);
}