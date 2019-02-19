#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/stat.h> 
#include <time.h>
#include <fcntl.h>

#define FIFO MyFIFO 

//struktura wyniku
struct
{
int poczatek_zakresu;
int koniec_zakresu ;
int ilosc_liczb;
}wynik_t;

//main
void main(int argc, char *argv[]){ 

int pid,status,krok,procesy,rd;
int wynik=0;
int fifo,odczyt;

int tmp_poczatek,tmp_koniec;
char arg1[20], arg2[20];

int poczatek 	=atoi(argv[1]);
int koniec	=atoi(argv[2]);
int ilosc	=atoi(argv[3]);



//utworzenie fifo
fifo = mkfifo("MyFIFO",0666);
if(fifo < 0) { perror("mkfifo"); } 

odczyt = open("MyFIFO",O_RDWR);
if(odczyt < 0) { perror("Open");}

//ustalenie przedzialow
krok=( koniec - poczatek ) / ilosc;
wynik_t.ilosc_liczb=0;

//pętla startująca thready
for(int i=0;i<ilosc;i++)
{  

//obliczenie przedzialu
tmp_poczatek =  poczatek + krok * i;
if(i<ilosc-1)
{tmp_koniec = poczatek + krok * (i+1)-1;}
else
{tmp_koniec = koniec;}

//nowy process
printf("Tworze proces:%i Przedział:%i,%i\n", i,tmp_poczatek,tmp_koniec);

sprintf(arg1,"%d" ,tmp_poczatek);
sprintf(arg2,"%d" ,tmp_koniec);

if((pid=fork())==0)
{
execl("./worker","worker",arg1,arg2,NULL);
}
else if(pid == -1)
{
printf("Proces nie zostal utworzony\n");
break;
}


}

//petla czekająca na zakonczenie threadow  
for(int i=0;i<atoi(argv[3]);i++){
	pid = wait(&status);  
}

//petla odczytujaca z pliku


for(int i=0; i<ilosc ; i++){
 		read(odczyt, &wynik_t, sizeof(wynik_t) );
 		wynik = wynik + wynik_t.ilosc_liczb;
		printf("Otrzymano Wynik\n");
}
  close(odczyt);
  unlink("MyFIFO");
  printf("Proces zakonczony %i\n",wynik);
} 



	
	
	
	
	
	
	
	
	
	
	

