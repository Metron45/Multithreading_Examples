// Producent / konsument  proces odbierajacy komunikaty z kolejki      
// Kompilacja gcc mq_rcv.c -o mq_rcv -lrt       
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#define  SIZE     80
#define  MQ_NAME "/Kolejka"
 struct {
        int  typ;        // Typ komunikatu
        char text[SIZE];  // Tekst komunikatu
 } msg;

void main(int argc, char *argv[]) {
 int i, res, num=0;
 unsigned int prior;
 mqd_t  mq;
 struct mq_attr attr;
 prior = 10;
 if(argc < 2) {
    printf("Uzycie: mq_rcv numer\n"); 
    exit(0);
 }
 num = atoi(argv[1]);
 // Utworzenie kolejki komunikatow ----------------
 attr.mq_msgsize = sizeof(msg);
 attr.mq_maxmsg  = 1;
 attr.mq_flags   = 0;
 mq=mq_open(MQ_NAME , O_RDWR | O_CREAT , 0660, &attr );
 if( mq == -1 ) {   
 	perror("Kolejka "); 
 	exit(0); 
 }
 printf("Kolejka utworzona: %d \n",mq);

 for(i=0; i < num ;i++) {
    	mq_getattr(mq,&attr); //  Status kolejki
    	res = mq_receive(mq,(char *)&msg,sizeof(msg),&prior);
    	if (res == -1 ) { perror("Blad odczytu z mq"); }
    	else 		{ printf("Konsument odebral: nr:%d msg:%s\n",msg.typ , msg.text); }
    	usleep(1);
  }
  mq_close(mq);
  mq_unlink(MQ_NAME);
}  
