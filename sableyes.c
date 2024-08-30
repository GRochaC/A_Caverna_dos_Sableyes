#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

#define TAMSAB 12
#define NUMCAM 5
#define true 1

pthread_t sableyes[TAMSAB];

sem_t camaras[NUMCAM], caverna;
int qntJoias[NUMCAM];

void * sableye(void *arg);

void * terremoto(void *arg);

int main(int arg, char **argv) {
    srand48(time(NULL));

    // cria as threads sableyes
    for(int i = 0; i < TAMSAB; i++) {
        int *id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&sableyes[i], NULL, sableye, (void*) id);
    }

    // inicia o semaforo da caverna
    sem_init(&caverna, 0, 1);

    // inicia os semaforos das camaras
    for(int i = 0; i < NUMCAM; i++) sem_init(&camaras[i],0,1);

    for(int i = 0; i < TAMSAB; i++) pthread_join(sableyes[i],NULL);

    return 0;
}

void * terremoto(void *arg) {
    while(true) {
        // terremoto dormente
        sleep((int) drand48() % 20 + 10);

        // terremoto
        sem_wait(&caverna);

        sem_post(&caverna);

    }

    pthread_exit(0);
}