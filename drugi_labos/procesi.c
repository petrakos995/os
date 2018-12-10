#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int Id; /* identifikacijski broj segmenta */

int *ZajednickaVarijabla;

void brisi(int sig)
{
   /* oslobađanje zajedničke memorije */
   (void) shmdt((char *) ZajednickaVarijabla);
   (void) shmctl(Id, IPC_RMID, NULL);
   exit(0);
}

int main(int argc, char *argv[])
{

   int N,M;

   N = atoi(argv[1]);
   M = atoi(argv[2]);


   /* zauzimanje zajedničke memorije */
   Id = shmget(IPC_PRIVATE, sizeof(int), 0600);

   /* greška - nema zajedničke memorije */
   if (Id == -1)
      exit(1);  


   ZajednickaVarijabla = (int *) shmat(Id, NULL, 0);
   *ZajednickaVarijabla = 0;

   int i;

   /* pokretanje paralelnih procesa */
   for (i = 0; i < N; i++){
         switch (fork()) {
            case 0:
            //funkcija koja obavlja posao djeteta i
            *ZajednickaVarijabla += M;
            exit(0);
            case -1:
            //ispis poruke o nemogućnosti stvaranja procesa;
            printf("nemogućnost stvaranja novog procesa");
            default:
            //nastavak posla roditelja;
            //sleep(1);
            printf("trenutna vrijednost od a %d\n", *ZajednickaVarijabla);
         }
   }

   while (i--) wait (NULL);
   printf("_________________________________\nkonacna vrijednost varijable a =%d\n", *ZajednickaVarijabla);
   brisi(0);
   return 0;
}