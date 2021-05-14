#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define LEITORES 1
#define ESCRITORES 3


//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;


void delay();
void * leitor (void * arg);
void * escritor (void * arg);