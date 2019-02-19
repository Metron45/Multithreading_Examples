#include <sys/mman.h> 
#include <sys/wait.h>
#include <fcntl.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h> 
#define BSIZE     4   // Rozmiar bufora 
#define LSIZE    80   //  Dlugosc linii 
typedef struct { 
    char buf[BSIZE][LSIZE]; 
    int head; 
    int tail; 
    int cnt; 
    sem_t mutex; 
    sem_t empty; 
    sem_t full; 
} bufor_t; 

void main(int argc,char *argv[]) { 
 int i,stat,k, pid, size, fd,res; 
 bufor_t  *wbuf ; 
 char c; 
 // Utworzenie segmentu --------------------------- 
 fd = shm_open("bufor", O_RDWR , 0660); 
 if(fd == -1) { perror("open"); _exit(-1); } 
 printf("Konsument: fd: %d\n",fd);     
 // Odwzorowanie segmentu fd w obszar pamieci procesow 
 wbuf = (bufor_t *)mmap(0, sizeof(bufor_t) ,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0); 
 if(wbuf == NULL) { perror("map");  _exit(-1); } 

 for(i=0;i<10;i++) { 
     sem_wait(&(wbuf->full)); 
     sem_wait(&(wbuf->mutex)); 
     printf("Konsument - iter: %d cnt: %d odebrano %s\n"
     ,i ,wbuf->cnt ,wbuf->buf[wbuf->tail]);  
     wbuf-> cnt --; 
     wbuf->tail = (wbuf->tail +1) % BSIZE; 
     sem_post(&(wbuf->mutex)); 
     sem_post(&(wbuf->empty)); 
     sleep(1); 
 }     

}
