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

static int prazan = 1;
static int s;
pthread_mutex_t monitor;
pthread_cond_t trgovac1;
pthread_cond_t pusaci;

void *pusac(void *broj) {
	long brojd = (long)broj;
		if(brojd==1) {
			printf("Pusac %ld:Ja imam papir\n",brojd);
		} else if(brojd == 2) {	
			printf("Pusac %ld:Ja imam sibicu\n",brojd);
		}else if(brojd == 3) {	
			printf("Pusac %ld:Ja imam duhan\n",brojd);
		}	
	do {
		pthread_mutex_lock(&monitor);
		while(brojd != s) {
			pthread_cond_wait(&pusaci,&monitor);
		}
		if(s==1) {
			printf("Pusac %ld:uzimam duhan i sibice\n\n",brojd);
		} else if(s == 2) {	
			printf("Pusac %ld:uzimam duhan i papir\n\n",brojd);
		}else if(s == 3) {	
			printf("Pusac %ld:uzimam papir i sibice\n\n",brojd);
		}
		prazan = 1;
		s=0;
		sleep(3);
		pthread_cond_signal(&trgovac1);
		printf("Rolam i pusim\n\n");
		pthread_mutex_unlock(&monitor);
	}while(1);

}

void *trgovac() {
	sleep(1);	
	do {
		pthread_mutex_lock(&monitor);
		while(prazan == 0) {
			 pthread_cond_wait(&trgovac1,&monitor);
		}
		prazan = 0;
		s = rand()%(3-1+1)+1;
			if(s==1) {
				printf("Trgovac stavlja duhan i sibice\n");
			} else if(s == 2) {	
				printf("Trgovac stavlja duhan i papir\n");
			}else if(s == 3) {	
				printf("Tgrovac stavlja papir i sibice\n");
			}
			sleep(3);
		 pthread_cond_broadcast(&pusaci);
		 pthread_mutex_unlock(&monitor);

	}while(1);

}
void brisi() {
	pthread_mutex_destroy(&monitor);
	pthread_cond_destroy(&trgovac1);
	 pthread_cond_destroy(&pusaci);
}
int init() {
	if (pthread_mutex_init(&monitor, NULL) != 0)
    {
        printf("\n Monitor se nije uspio inicijalizirati\n");
        return 1;
    }
    if(pthread_cond_init(&trgovac1,NULL)!=0) {
    	printf("iniciajliziranje reda uvjeta trgova nije uspjelo\n");
    	return 1;
    }
    if(pthread_cond_init(&pusaci,NULL)!=0) {
    	printf("iniciajliziranje reda uvjeta pusaca nije uspjelo\n");
    	return 1;
    }
    return 0;

}

int main(void) {
	pthread_t thr_id[4];
	long t1=1, t2=2, t3=3;
	sigset(SIGSTOP, brisi);
	srand((unsigned) time(NULL));
	if(init()==1) {
		printf("Inicijaliziranje monitora i uvjetnih redova nije uspjelo\n");
		exit(1);
	}
   	if(pthread_create(&thr_id[0],NULL,(void *)trgovac,NULL) != 0) {
				printf("Stvaranje dretve trgovac nije uspjelo\n");
				exit(0);
	} 		
   	if(pthread_create(&thr_id[1],NULL,(void *)pusac,(void *)t1)!= 0) {
				printf("Stvaranje %ld. pusaca nije uspjelo\n",t1);
				exit(1);
	} 
	if(pthread_create(&thr_id[2],NULL,(void *)pusac,(void *)t2)!= 0) {
				printf("Stvaranje %ld. pusaca nije uspjelo\n",t2);
				exit(1);
	} 	
	if(pthread_create(&thr_id[3],NULL,(void *)pusac,(void *)t3)!= 0) {
				printf("stvaranje %ld. pusaca nije uspjelo\n",t3);
				exit(1);
			 
	}


	for(int i = 0; i< 4; i++) {
		pthread_join(thr_id[i], NULL);
	}

	return 0;
}




