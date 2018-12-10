#include <stdio.h>
#include <pthread.h>

int ZajednickaVarijabla;

void *Obrada(void *x)
{
      ZajednickaVarijabla += *((int*)x);
      printf("trenutna vrijednost varijable a =%d\n", ZajednickaVarijabla);
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
   for(i =0; i<N; i++){
      if (pthread_create(&thr_id[i], NULL, Obrada, &M) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
      }
   }
   
   while(i--)  pthread_join(thr_id[i], NULL);
   printf("_________________________________\nkonacna vrijednost varijable a =%d\n", ZajednickaVarijabla);


   return 0;
}