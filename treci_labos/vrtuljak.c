#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PROLAZAN 1
#define NEPROLAZAN 0

int slobodno, broj_ljudi;
sem_t vrtuljak, sjeo, kraj, sisao;


void sjedni(int broj){
	printf("posjetitelj je sjeo na %d mjesto od max %d na vrtuljku\n", broj, slobodno );
}

void ustani(int broj){
	printf("posjetitelj se ustao sa %d mjesta na vrtuljku\n", broj);
}

void sidi(int broj){
	printf("posjetitelj je sisao sa vrtuljka, mjesto %d je slobodno.\n", broj);
}


void pokreni(){
	printf("Vrtuljak je napunjen, pokreni vrtuljak.\n");
}


void zaustavi(){
	printf("Vrtuljak je zavrsio sa voznjom, zaustavljamo ga.\n");
}



void *posjetitelj_dretva(void *x){
	int id = *((int *)x) % 10 + 1;
	sem_wait (&vrtuljak);
	sleep(1);
	sjedni(id);
	sleep(1);
	sem_post(&sjeo);
	sem_wait(&kraj);
	sleep(1);
	ustani(id);
	sleep(1);
	sidi(id);
	sleep(1);
	sem_post(&sisao);

}

void *vrtuljak_dretva(){
	int i = 0;
	while(1){
		for(i = 0; i < slobodno; ++i){
			sleep(1);
			sem_post(&vrtuljak);
		}
		for(i = 0; i < slobodno; ++i){
			sem_wait(&sjeo);
		}
		pokreni();
		sleep(2);
		zaustavi();
		for (i = 0; i < slobodno; ++i){
			sleep(1);
			sem_post(&kraj);
		}
		for (i = 0; i < slobodno; ++i){
			sem_wait(&sisao);
		}
	}
}

int main(int argc, char** argv){
	slobodno = atoi(argv[1]);
	pthread_t vrtuljak_id;
	pthread_t posjetitelji_id[100];


	sem_init(&vrtuljak, 0, NEPROLAZAN);
	sem_init(&sjeo, 0, NEPROLAZAN);
	sem_init(&kraj, 0, NEPROLAZAN);
	sem_init(&sisao, 0, NEPROLAZAN);

	if (pthread_create(&vrtuljak_id, NULL, vrtuljak_dretva, NULL)) {
			printf("Greska pri stvaranju dretve\n");
			exit(1);
	}

	srand ((unsigned) time(NULL));
	int vrijeme;

	for(int i = 0; i < 100; i++){
		pthread_create(posjetitelji_id+i, NULL, posjetitelj_dretva, &i);
		vrijeme = rand() % (45000+1-1000)+1000; 
		usleep(vrijeme);
	}

	for (int i = 0; i < 100; ++i){
		pthread_join(posjetitelji_id[i], NULL);
	}

	return 0;
}
