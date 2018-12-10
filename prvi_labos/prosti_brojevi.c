#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

int pauza = 0;
unsigned long broj = 1000000001;
unsigned long zadnji = 1000000001;

int prost ( unsigned long n ) {
	int i ;

	if ( ( n & 1  ) == 0 ) 
		return 0;

	
	for ( i = 3; i*i <= n; i++ )
		if ( ( n % i ) == 0 )
			return 0;

	return 1;
}

void ispis(unsigned long x){
	printf("zadnji prosti broj: %lu\n", x);
}

void periodicki_ispis(int sig){
	ispis(zadnji);
}

void postavi_pauzu(int sig){
	pauza = 1 - pauza;
	signal(SIGINT, postavi_pauzu );

}

void prekini(int sig){
	signal(SIGTERM, prekini);
	ispis(zadnji);
	exit(0);	
}

int main ()
{
	struct itimerval t;

	
	sigset ( SIGALRM, periodicki_ispis );

	t.it_value.tv_sec = 0;
	t.it_value.tv_usec = 500000;

	t.it_interval.tv_sec = 5;
	t.it_interval.tv_usec = 0;

	setitimer ( ITIMER_REAL, &t, NULL );


	signal(SIGINT, postavi_pauzu );
	signal(SIGTERM, prekini);




	while(1){
		if(prost(broj))	{
			zadnji=broj;
		}	
		broj++;
		if(pauza){
			pause();
		}			
	}

	

	return 0;
}
