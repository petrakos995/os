#include <stdio.h>
#include <signal.h>
#define N 6    /* broj razina proriteta */

 

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET = 0;



int sig[]={SIGUSR1,SIGUSR2,SIGCONT,SIGILL,SIGINT};
//NAREDBOM KILL SALJES OVE SIGNALE: kill -SIGTERM 12345

//gotovo
void zabrani_prekidanje(){
   for(int i=0; i<5; i++)
      sighold(sig[i]);
}
//gotovo
void dozvoli_prekidanje(){
   for(int i=0; i<5; i++)
      sigrelse(sig[i]);
}

//gotov ispis
void print(int n, char pon){

	for(int i =0; i < N ; i++){
		printf("%c ", ((i == n) ? pon : '-'));
	}
	printf("\n");
}
 
//gotovo
void obrada_prekida(int n){
	print(n, 'P');
	for(int i=0; i < 5; i++){
		sleep(1);
		print(n,'1' + i);
	}
	print(n, 'K');
}

void prekidna_rutina(int sig){
   int n=-1;
   int x;
   zabrani_prekidanje();
   switch(sig){
   		case SIGUSR1: 
        	n=1; 
        	printf("- X - - - -\n");
        	break;
      case SIGUSR2: 
        	n=2; 
        	printf("- - X - - -\n");
        	break;
      case SIGCONT:
      		n=3;
      		printf("- - - X - -\n");
      		break;
      case SIGILL:
      		n=4;
      		printf("- - - - X -\n");
      case SIGINT:
      		n=5;
      		printf("- - - - - X\n");
      		break;
   }

   OZNAKA_CEKANJA[n]=1;
    

    do{
   		x = 0;
   		for(int j= TEKUCI_PRIORITET +1; j < N; j++){
   			if(OZNAKA_CEKANJA[j] != 0)	
   				x = j;
   		}

   		if (x > 0){
   			OZNAKA_CEKANJA[x] =0;
   			PRIORITET[x]=TEKUCI_PRIORITET;
   			TEKUCI_PRIORITET=x;

   		  	dozvoli_prekidanje();
   		  	obrada_prekida(x);
   		  	zabrani_prekidanje(); 	

   		  	TEKUCI_PRIORITET = PRIORITET[x];
   		}	
   }while(x>0);
   dozvoli_prekidanje();
} 

 

int main ( void )
{
   sigset (SIGUSR1, prekidna_rutina);
   sigset (SIGUSR2, prekidna_rutina);
   sigset (SIGCONT, prekidna_rutina);
   sigset (SIGILL, prekidna_rutina);
   sigset (SIGINT, prekidna_rutina);

   printf("Proces obrade prekida, PID=%d\n", getpid());
   printf("G 1 2 3 4 5 \n");

   /* troši vrijeme da se ima šta prekinuti - 10 s */
   for(int i=0; i < 2000; i++){
   	sleep(1);
   }

   printf ("Zavrsio osnovni program\n");
 
   return 0;

}