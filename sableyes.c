#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

#define TAMSAB 17
#define NUMCAM 5
#define CAP 7
#define true 1
#define false 0

pthread_t sableyes[TAMSAB], caverna;
pthread_cond_t sableye_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t camaras[NUMCAM],capCamaras[NUMCAM];
int qntJoias[NUMCAM], capacidade[NUMCAM];

int comecoTerremoto = false;

void * sableye(void *arg);

void * terremoto(void *arg);

int main(int arg, char **argv) {
    // inicia a thread do terremoto
    int *id = 0;
    pthread_create(&caverna,NULL,terremoto,(void *) id);

    // inicia os semaforos das camaras
    for(int i = 0; i < NUMCAM; i++) {
        sem_init(&camaras[i],0, 1);
        int cap = ((int) rand() % CAP)+1;
        capacidade[i] = cap;
        sem_init(&capCamaras[i],0,cap);
    }

    // cria as threads sableyes
    for(int i = 0; i < TAMSAB; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&sableyes[i], NULL, sableye, (void*) id);
    }

    pthread_join(caverna,NULL);
    for(int i = 0; i < TAMSAB; i++) pthread_join(sableyes[i],NULL);

    return 0;
}

void * sableye(void *arg) {
    int id = *((int *) arg);
    while(true) {
        // aguarda o terremoto terminar
        while(comecoTerremoto) pthread_cond_wait(&sableye_cond, &mutex);

        // procurando
        sleep(((int) rand() % 5) * 2);

        // sorteia uma camara
        int idx = ((int) rand()) % NUMCAM;
        printf("O sableye %d está procurando alimento na câmara %d com capacidade %d.\n",id,idx,capacidade[idx]);

        // regiao critica
        // verifica se ha espaco na camara
        if(sem_trywait(&capCamaras[idx]) == 0) {
            sem_wait(&camaras[idx]);
                if(qntJoias[idx]) {
                    int joiasComidas = ((int) rand() % qntJoias[idx]) +1;

                    // comendo
                    sleep(((int) rand() % 3)+1);

                    printf("O sableye %d comeu %d joias. Há %d joias restantes.\n",id,joiasComidas,qntJoias[idx]-joiasComidas);
                    qntJoias[idx] -= joiasComidas;

                } else {
                    printf("O sableye %d não encontrou nada...\n",id);
                }    
            sem_post(&camaras[idx]);
            sem_post(&capCamaras[idx]);
        } else {
            printf("A câmara %d está lotada...\n",idx);
        }
    }
}

void * terremoto(void *arg) {
    while(true) {
        // terremoto dormente
        sleep((int) rand() % 10 + 10);

        // terremoto
        comecoTerremoto = true;
        pthread_mutex_lock(&mutex);
            printf("\n");
            printf("Um terremoto está fazendo a terra tremer! Novas joias foram reveladas.\n");

            // terremoto revelando joias  
            for(int i = 0; i < NUMCAM; i++) {
                int joiasReveladas = ((int) rand() % 10) +1;
                qntJoias[i] += joiasReveladas;
                printf("%d joias reveladas em %d!\n", joiasReveladas, i);
            }

            // fim do terremoto
            comecoTerremoto = false;
            
            // liberando os sableyes 
            pthread_cond_broadcast(&sableye_cond);
            printf("\n");
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
}