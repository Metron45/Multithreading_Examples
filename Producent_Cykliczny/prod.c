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
 int i,stat,k, size, fd,res; 
 bufor_t  *wbuf ; 
 char c; 
 // Utworzenie segmentu --------------------------- 
 fd = shm_open("bufor", O_RDWR|O_CREAT , 0660); 
 if(fd == -1) { perror("open"); _exit(-1); } 
 printf("Producent: fd: %d\n", fd); 
 size = ftruncate(fd, sizeof(bufor_t));   
 if(size < 0) { perror("trunc"); _exit(-1); } 
 // Odwzorowanie segmentu fd w obszar pamieci procesow 
 wbuf = (bufor_t *)mmap(0, sizeof(bufor_t) ,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0); 
 if(wbuf == NULL) {perror("map");  _exit(-1); } 

//producent
 for(i=0;i<10;i++) { 
       // printf("Producent: %i\n",i); 
        
       sem_wait(&(wbuf->empty)); 
       sem_wait(&(wbuf->mutex)); 
       printf("Producent - iter: %d cnt: %d head: %d tail: %d\n"
       ,i ,wbuf->cnt ,wbuf->head ,wbuf->tail);
       sprintf(wbuf->buf[wbuf->head],"Komunikat %d",i);  
       wbuf->cnt ++; 
       wbuf->head = (wbuf->head +1) % BSIZE; 
       sem_post(&(wbuf->mutex)); 
       sem_post(&(wbuf->full)); 
       sleep(1); 
 }

}
