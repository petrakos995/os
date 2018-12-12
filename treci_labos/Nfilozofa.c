#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>

pthread_mutex_t M;
pthread_cond_t CONDITIONS;
int broj_filozofa;
int broj_stapica;
int polje[100];

void zauzmi(int id){
	broj_stapica -= id;
}

void mon_uzmi_stapice(int id){
	pthread_mutex_lock(&M);
	while(broj_stapica < id)
		pthread_cond_wait(&CONDITIONS, &M);
	zauzmi(id);
	pthread_mutex_unlock(&M);
}

void oslobodi(int id){
	broj_stapica += id;
}

void mon_vrati_stapice(int id){
	pthread_mutex_lock(&M);
	oslobodi(id);
	pthread_cond_broadcast(&CONDITIONS);
	pthread_mutex_unlock(&M);
}



void jedi(int id){
	printf("filozof sa %d ruku je poceo jest.\n", id);
}

void misli(int id){
	printf("filozof sa %d ruku je vratio  stapice i prestao jest.\n", id);
}



void *proces_dretva(void *x){
	int id = *((int *)x);
	int i = 0;
	while(i < 5){
		i++;
		mon_uzmi_stapice(id);
		jedi(id);
		sleep(2);
		mon_vrati_stapice(id);
		misli(id);
		sleep(2);
	}
}

int main(void){

	pthread_t filozofi[100];

	printf("prije otvaranja dat\n");
	FILE *text;
    text = fopen("dat.txt", "r+");

    printf("poslije otvaranja dat\n");
    int brojac = 0;

    //fscanf(text, "%d %d", &broj_stapica, &broj_filozofa);
    while(fscanf(text, "%d", &polje[brojac]) !=EOF){
    	brojac++;
    }

    broj_stapica = polje[0];
    broj_filozofa = polje[1];
	
    if (pthread_mutex_init(&M, NULL) != 0)
    {
        printf("\n Monitor se nije uspio inicijalizirati\n");
        return 1;
    }
    if(pthread_cond_init(&CONDITIONS,NULL)!=0) {
    	printf("iniciajliziranje reda uvjeta trgova nije uspjelo\n");
    	return 1;
    }

    printf("prije for 1 %d %d \n", broj_stapica, broj_filozofa);

    for(int i = 0; i < broj_filozofa; ++i) {
       	if (pthread_create(filozofi+i, NULL, proces_dretva, &polje[i+2])) {
			printf("Greska pri stvaranju dretve\n");
			exit(1);
		} 
    }
    
    for(int i = 0; i < broj_filozofa; i++)
		pthread_join(filozofi[i], NULL);

    fclose(text);

	return 0;
}