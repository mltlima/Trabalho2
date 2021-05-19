#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define LEITORES 3
#define ESCRITORES 3


typedef struct{
	int temperatura;
	int idSensor;
	int idLeitura;
}medicao;


//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;


void delay();
void * leitor (void * arg);
void * escritor (void * arg);
int medirTemperatura();