/**
InCogNiTo
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <unistd.h>
#define CARS_ALLOWED 3

typedef struct Car {
	int direction;
	int number;
} Car;

pthread_mutex_t MOST;
pthread_cond_t CONDITIONS[2];	/* 0 is LEFT, 1 is RIGHT */
int ON_BRIDGE = 0;
int DIRECTION = 0;
Car *CARS;

void error(char *error_string) {
	printf("%s\n", error_string);
	exit(1);
}

void *car_thread(void *c) {
	Car *car = c;
	printf("Created car no. %d, direction = %s\n", car->number, car->direction == 0 ? "LEFT" : "RIGHT");
	pthread_mutex_lock(&MOST);
	while (ON_BRIDGE >= CARS_ALLOWED || DIRECTION != -1 && DIRECTION != car->direction) {
		pthread_cond_wait(CONDITIONS + car->direction, &MOST);
	}
	
	ON_BRIDGE += 1;
	printf("Car %d crossing %s...\n", car->number, car->direction == 0 ? "LEFT" : "RIGHT");
	DIRECTION = car->direction;
	pthread_mutex_unlock(&MOST);
	sleep(2);
	pthread_mutex_lock(&MOST);
	ON_BRIDGE -= 1;
	if (ON_BRIDGE > 0) {
		pthread_cond_signal(CONDITIONS + car->direction);	
	} else {
		DIRECTION = -1;
		pthread_cond_broadcast(CONDITIONS + 1 - car->direction);	
	}
	pthread_mutex_unlock(&MOST);
	printf("Car %d crossed the bridge.\n", car->number);
	free(car);
	return NULL;
}
int main(int argc, char *argv[]) {
	int i;
	pthread_t *thread_ids;
	if (argc != 2) {
		error("Usage: ./stari_most N, where N is the number of cars to simulate");
	} else {
		int N = atoi(argv[1]);
		for (i = 0; i < 2; i++) {
			pthread_cond_init(CONDITIONS + i, NULL);
		}
		thread_ids = (pthread_t *)malloc(N*sizeof(pthread_t));
		srand(time(NULL));
		for (i = 0; i < N; i++) {
			Car *car = malloc(sizeof(Car));
			car->direction = rand() % 2;
			car->number = i;
			pthread_create(thread_ids+i, NULL, car_thread, (void *)car);
		}
		
		for (i = 0; i < N; i++) {
			pthread_join(thread_ids[i], NULL);		
		}
		return 0;
	}
}	
