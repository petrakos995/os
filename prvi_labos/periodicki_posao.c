#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void periodicki_posao ( int sig )
{
	printf ( "Radim periodicki posao\n" );
}

int main ()
{
	struct itimerval t;

	/* povezivanje obrade signala SIGALRM sa funkcijom "periodicki_posao" */
	sigset ( SIGALRM, periodicki_posao );

	/* definiranje periodičkog slanja signala */
	/* prvi puta nakon: */
	t.it_value.tv_sec = 500;
	t.it_value.tv_usec = 500000;
	/* nakon prvog puta, periodicki sa periodom: */
	t.it_interval.tv_sec = 500;
	t.it_interval.tv_usec = 500000;

	/* pokretanje sata s pridruženim slanjem signala prema "t" */
	setitimer ( ITIMER_REAL, &t, NULL );

	while (1)
		pause (); /* pauzira do primitka bilo kojeg signala */

	return 0;
}
