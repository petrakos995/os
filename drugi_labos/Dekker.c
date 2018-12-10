#include <stdio.h>
#include <pthread.h>

int ZajednickaVarijabla;
int ZASTAVICA[2] ;
int Pravo = 0;
int N;

void enterCriticalSection(int i){
   int j = 1-i;
   ZASTAVICA[i]= 1;
   while(ZASTAVICA[j] != 0){
      if(Pravo == j){
         ZASTAVICA[i]=0;
         while(Pravo == j){
            continue;
         }
         ZASTAVICA[i] = 1;
      }
   }
}

void exitCriticalSection(int i) {
   Pravo = 1-i;
   ZASTAVICA[i]=0;
}

void *Obrada(void *x)
{
   enterCriticalSection(*((int *)x));
   ZajednickaVarijabla += N;
   printf("trenutna vrijednost varijable a =%d\n", ZajednickaVarijabla);
   exitCriticalSection(*((int *)x));
}

int main(int argc, char *argv[])
{
   int i1=0;
   int i2=1;
   
   N = atoi(argv[1]);

   pthread_t thr_id[2];

   ZajednickaVarijabla = 0;

   //stvaranje dretvi 
   if (pthread_create(&thr_id[0], NULL, Obrada, &i1) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
   }

      
   if (pthread_create(&thr_id[1], NULL, Obrada, &i2) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
      }
   
   pthread_join(thr_id[0], NULL);
   pthread_join(thr_id[1], NULL);
   printf("_________________________________\nkonacna vrijednost varijable a =%d\n", ZajednickaVarijabla);


   return 0;
}