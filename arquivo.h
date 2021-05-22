#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int LEITORES;
int ESCRITORES;


typedef struct{
	int temperatura;
	int idSensor;
	int idLeitura;
}medicao;


//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;


void * atuadores (void * arg);
void * sensores (void * arg);
int lerTemperatura(int id);
void alarme();
int medirTemperatura();