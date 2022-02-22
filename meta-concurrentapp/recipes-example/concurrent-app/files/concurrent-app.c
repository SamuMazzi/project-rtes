#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

struct bandierine_t {
	sem_t mutex;

	sem_t pGiudice, viaGioc, fGiudice;
	int preso, ritBase, bandiera, g_vinc;
    int bGioc, bGiud, fGioc, fGiud;
} bandierine;

void init_bandierine(struct bandierine_t* b) {
	sem_init(&b->mutex, 0, 1);

	sem_init(&b->pGiudice, 0, 0);
	sem_init(&b->viaGioc, 0, 0);
	sem_init(&b->fGiudice, 0, 0);

	b->preso = b->ritBase = b->bandiera = b->g_vinc = 0;
    b->bGioc = b->bGiud = b->fGioc = b->fGiud = 0;
}

void attendi_giocatori(struct bandierine_t* b) {
	sem_wait(&b->mutex);
	if(b->bGioc < 2) {
        b->bGiud = 1;
        sem_post(&b->mutex);
        sem_wait(&b->pGiudice);
        b->bGiud = 0;
    }
	b->preso = b->ritBase = b->bandiera = b->g_vinc = b->bGioc = 0;

    sem_post(&b->mutex);
}

void via(struct bandierine_t* b) {
	sem_post(&b->viaGioc);
    sem_post(&b->viaGioc);
}

void attendi_il_via(struct bandierine_t* b, int n) {
	sem_wait(&b->mutex);
    b->bGioc++;
    if(b->bGioc == 2 && b->bGiud == 1) {
        sem_post(&b->pGiudice);
    } else {
        sem_post(&b->mutex);
    }
    sem_wait(&b->viaGioc);
}

int bandierina_presa(struct bandierine_t* b, int n) {
	sem_wait(&b->mutex);
    int out = 0;
    if(b->bandiera == 0) {
        b->bandiera = 1;
        out = 1;
    }
    sem_post(&b->mutex);
    return out;
}

int sono_salvo(struct bandierine_t* b, int n) {
	sem_wait(&b->mutex);
    int out = 0;
    if(b->preso == 0) {
        //printf("sono salvo\n");
        b->ritBase = 1;
        out = 1;
        b->g_vinc = n;
    }
    b->fGioc++;
    if(b->fGioc == 2 && b->fGiud == 1) {
        sem_post(&b->fGiudice);
    } else {
        sem_post(&b->mutex);
    }
    return out;
}

int ti_ho_preso(struct bandierine_t* b, int n) {
	sem_wait(&b->mutex);
    int out = 0;
    if(b->ritBase == 0) {
        b->preso = 1;
        out = 1;
        b->g_vinc = n;
    }
    b->fGioc++;
    if(b->fGioc == 2 && b->fGiud == 1) {
        sem_post(&b->fGiudice);
    } else {
        sem_post(&b->mutex);
    }
    return out;
}

int risultato_gioco(struct bandierine_t* b) {
	sem_wait(&b->mutex);
    int out = 0;
    if(b->fGioc != 2) {
        //printf("Mi metto in attesa\n");
        b->fGiud = 1;
        sem_post(&b->mutex);
        sem_wait(&b->fGiudice);
        b->fGiud = 0;
    }
    b->fGioc = 0;
    out = b->g_vinc;
    sem_post(&b->mutex);
    return out;
}

void pausetta(void)
{
	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = (rand() % 10 + 1) * 1000000;
	nanosleep(&t, NULL);
}

void *giocatore(void *arg) {
    int numerogiocatore = (int) arg;
    pausetta();
    attendi_il_via(&bandierine, numerogiocatore);
    pausetta();
    if(bandierina_presa(&bandierine, numerogiocatore)) {
        //Corri alla base
        pausetta();
        if(sono_salvo(&bandierine, numerogiocatore)) printf("%d: Salvo!\n", numerogiocatore);
    } else {
        //Cerca di acchiappare l'altro
        pausetta();
        if(ti_ho_preso(&bandierine, numerogiocatore)) printf("%d: Ti ho preso!\n", numerogiocatore);
    }
    return 0;
}

void *giudice(void *arg) {
    attendi_giocatori(&bandierine);
    //Pronti, attenti...
    pausetta();
    via(&bandierine);
    pausetta();
    printf("Il vincitore è %d\n", risultato_gioco(&bandierine));
    return 0;
}

int main()
{
	pthread_attr_t a;
	pthread_t p[3];
    while(1) {
        init_bandierine(&bandierine);

        srand(555);

        pthread_attr_init(&a);

        //pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);
        for(int i = 0; i < 2; i++){
            pthread_create(&p[i], &a, giocatore, (void*) i);
        }
        pthread_create(&p[2], &a, giudice, (void*) NULL);

        for(int i = 0; i < 3; i++){
            pthread_join(p[i], NULL);
        }

        printf("----------------------------------\n");
        //sleep(1);
    }
    pthread_attr_destroy(&a);

	return 0;
}


