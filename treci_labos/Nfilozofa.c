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

	printf("prije otvaranja dat\n");
	FILE *text;
    text = fopen("/home/petra/Desktop/os/treci_labos/dat.txt", "r");

    printf("poslije otvaranja dat\n");
    fscanf(text, "%lf %lf", &broj_stapica, &broj_filozofa);

    int ruke; 
    pthread_t filozofi[broj_filozofa];

    printf("prije for 1 %d %d \n", broj_stapica, broj_filozofa);

    for(int i = 0; i < broj_filozofa; ++i) {
    	fscanf(text, "%lf",&ruke);
    	printf("%d\n", ruke);
       	if (pthread_create(filozofi+i, NULL, proces_dretva, &ruke)) {
			printf("Greska pri stvaranju dretve\n");
			exit(1);
		} 
    }
    
    for(int i = 0; i < broj_filozofa; i++)
		pthread_join(filozofi[i], NULL);

    fclose(text);

	return 0;
}