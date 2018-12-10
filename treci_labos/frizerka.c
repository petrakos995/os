#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <malloc.h>
#include <semaphore.h>

#define N 3

int obrt;
int slobmj = N;
int kraj;
int sleeping;
sem_t osem, bsem, ksem;

void *frizerka () {
	obrt=1;
	printf ("Frizerka: Otvaram salon\n");
	while (1) {
		if (kraj) 
			obrt = 0;
		if (slobmj!=N) {
			sem_wait (&osem);
			printf ("Frizerka: Idem raditi\n");
			sleep(3);
			printf ("Frizerka: Klijent gotov\n");
			sem_post(&bsem);
		} else if (!kraj) {
			sleeping=1;
			printf ("Frizerka: Spavam dok mogu\n");
			sem_wait (&ksem);
			sleeping=0;	
		} else {
			printf ("Frizerka: Zatvaram salon\n");
			break;
		}
	}
}

void *klijent (void *brcl) {
	int idcl = *((int*) brcl);
	printf ("	Klijent (%d): Zelim na frizuru\n", idcl);
	if (obrt && slobmj>0) {
		slobmj--;
		printf ("	Klijent (%d): Ulazim u cekaonu (%d)\n", idcl, N-slobmj);
		if (sleeping) 
			sem_post(&ksem);
		sem_post(&osem);
		sem_wait (&bsem);
		slobmj++;
		printf ("	Klijent (%d): Frizerka mi radi frizuru\n", idcl);
	} else {
		printf ("	Klijent (%d): Nista od frizure danas\n", idcl);
	}
}



int main (void) {
	int i=0, j=0, *br;
	pthread_t thr1, *thr2=NULL;
	
	kraj = 0;
	srand ((unsigned) time(NULL));
	
	sem_init (&osem,0,0);
	sem_init (&bsem,0,1);
	sem_init (&ksem,0,0);
	
	if (pthread_create(&thr1, NULL, frizerka, NULL)) {
			printf("ERROR: thread create\n");
			exit(0);
	}
	
	thr2 = (pthread_t*)malloc(100*sizeof(pthread_t));
	if(!thr2) {
		printf("ERROR\n");
		exit(0);
	}
	
	br = (int*)malloc(100*sizeof(int));
	if(!br) {
		printf("ERROR\n");
		exit(0);
	}
	
	for (i=0; i<5; ++i) {
		usleep(45000);
		br[i]=i;
		if (pthread_create(thr2+i, NULL, klijent, (void *) (br+i))) {
			printf("ERROR: thread create\n");
			exit(0);
		}
	}
	sleep(10);
	while (j<65) {
		j+=rand()%35+1;
		sleep(j%5+1);
		br[i]=i;
		pthread_create(thr2+i, NULL, klijent, (void *) (br+i));
		i++;
	}

	for(; i>=0; --i){
		pthread_join(thr2[i],NULL);
	}

	sleep(rand()%7+1);
	kraj = 1;
	sem_post(&ksem);
	pthread_join (thr1,NULL);
	
	return 0;
}
