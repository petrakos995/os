#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int pid = 0;

void prekidna_rutina(int sig){
	/*posalji sigkill procesu pid  */
	kill(pid,SIGKILL);
	exit(0);
}


int main(int argc, char *argv[]){
   pid=atoi(argv[1]);
   sigset(SIGINT, prekidna_rutina);

   int r,n;
   srand((unsigned) time(NULL));


   while(1){
      /* odspavaj 3-5 sekundi */
      /* slučajno odaberi jedan signal (od 4) */
      /* pošalji odabrani signal procesu 'pid' funkcijom kill*/
   		
   		r = (rand() % 4 +1);
   		n=rand() % 3+3;
   		sleep(n);
   		switch(r){
   			case 1:
   				kill(pid, SIGUSR1);
   				break;
   			case 2:
   				kill(pid, SIGUSR2);
   				break;
   			case 3:
   				kill(pid, SIGCONT);
   				break;
   			case 4:
   				kill(pid, SIGILL);
   				break;
   		}
   }
   return 0;
}