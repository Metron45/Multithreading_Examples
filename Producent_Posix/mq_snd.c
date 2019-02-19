//---------------------------------------------------------------------------------
// Producent / konsument  proces wysylajacy komunikaty do kolejki     
// Odbiera program mq_rcv
// Kompilacja:   gcc mq_snd.c -o mq_snd -lrt   
// Uruchomienie: ./mq_snd numer
// Gdy zamontujemy koleki komunikatow w kat /dev/mqueue to mozna nimi manipulować
// uzywając poleceń ls,...
// mkdir /dev/mqueue
// mount -t mqueue none /dev/mqueue
// ----------------------------------------------------------------------------------
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define  SIZE     80
#define  MQ_NAME "/Kolejka"
 struct {
      int  typ;        // Typ komunikatu
      char text[SIZE];  // Tekst komunikatu
  } msg;

 void main(int argc, char *argv[]) {
   int i, res, num=0, nr = 0;
   unsigned int prior;
   mqd_t  mq;
   struct mq_attr attr;
   char kname[40];
   prior = 10;
   num 	  = atoi(argv[2]);
   nr     = atoi(argv[1]);
   strcpy(kname,MQ_NAME);
   // Utworzenie kolejki komunikatow ----------------
   attr.mq_msgsize = sizeof(msg);
   attr.mq_maxmsg  = 1;
   attr.mq_flags   = 0;
   mq=mq_open(kname , O_RDWR | O_CREAT , 0660, &attr );
   if( mq == -1 ) {
   	perror("Kolejka "); exit(0);  
   }
   printf("Kolejka: %s otwarta, mq: %d\n", kname,mq);
   
   for(i=0; i < num ;i++) {
     mq_getattr(mq,&attr);   
     sprintf(msg.text,"Producent %d krok %d", nr,i);
     msg.typ = nr;
     res = mq_send(mq,(char *)&msg,sizeof(msg),prior);
     if (res == -1 ) { perror("Blad zapisu do mq"); continue; }
     else 	     { printf("Producent nr:%d Komunikat:%d wyslany\n",nr,i); }
     usleep(1);
  }
  mq_close(mq);
  mq_unlink(MQ_NAME);
}  
 
