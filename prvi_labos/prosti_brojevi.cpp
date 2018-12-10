#include <ioastream>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

double 	pauza  = 0;
double	broj = 1000000000001;
double	zadnji= 1000000000001;

void ispis(int x)
{

}

void periodicki_posao(int sig)
{
	ispis(zadnji);
}

int main()
{

	return 0;
}