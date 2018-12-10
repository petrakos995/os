#include <stdio.h>
#include <pthread.h>

#define TAS(ZASTAVICA) __atomic_test_and_set (&ZASTAVICA, __ATOMIC_SEQ_CST)

int ZajednickaVarijabla;
char zastavica = 0; 

void *Obrada(void *x)
{
   // radno cekanje
   while ( TAS(zastavica) != 0 )    ; 
   ZajednickaVarijabla += *((int*)x);
   printf("trenutna vrijednost varijable a =%d\n", ZajednickaVarijabla);
   zastavica=0;
}

int main(int argc, char *argv[])
{
   int i;
   int N,M;

   N = atoi(argv[1]);
   M = atoi(argv[2]);

   pthread_t thr_id[N];

   ZajednickaVarijabla = 0;

   //stvaranje dretvi
   i = N;
   while(i--){
      if (pthread_create(&thr_id[i], NULL, Obrada, &M) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
      }
   }
   i=N;
   while(i--)  pthread_join(thr_id[i], NULL);
   printf("_________________________________\nkonacna vrijednost varijable a =%d\n", ZajednickaVarijabla);


   return 0;
}