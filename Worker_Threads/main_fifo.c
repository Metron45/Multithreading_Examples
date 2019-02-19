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
int fifo,odczyt,zapis;

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

fifo = mkfifo("MyFIFO_przedziały",0666);
if(fifo < 0) { perror("mkfifo"); }

zapis= open("MyFIFO_przedziały",O_RDWR);
if(odczyt < 0) { perror("Open");}

printf("utworzono FIFO\n");

//ustalenie przedzialow
krok=( koniec - poczatek ) / ilosc;
wynik_t.ilosc_liczb=0;

//petla zapisujaca przedziały dla worker threads
for(int i=0;i<ilosc;i++)
{ 
//obliczenie przedzialu
tmp_poczatek =  poczatek + krok * i;
if(i<ilosc-1){
tmp_koniec = poczatek + krok * (i+1)-1;
}
else{
tmp_koniec = koniec;
}
//zapis wyniku
wynik_t.poczatek_zakresu	= tmp_poczatek;
wynik_t.koniec_zakresu 		= tmp_koniec;

//zapis do fifo
write(zapis, &wynik_t, sizeof(wynik_t));

}
printf("Zapisano Przedziały\n");

for(int i=0;i<4;i++)
{ 
wynik_t.poczatek_zakresu	= 0;
wynik_t.koniec_zakresu 		= 0;

write(zapis, &wynik_t, sizeof(wynik_t));
}

//pętla startująca thready
for(int i=0;i<4;i++){  
if((pid=fork())==0){
execl("./worker","worker",NULL);
}
else if(pid == -1){
printf("Proces nie zostal utworzony\n");
break;
}
}
printf("Utworzono Thready\n");

//petla czekająca na zakonczenie threadow  
for(int i=0;i<ilosc;i++){
pid = wait(&status);  
}

//petla odczytujaca z pliku
for(int i=0; i<4 ; i++){
read(odczyt, &wynik_t, sizeof(wynik_t) );
wynik = wynik + wynik_t.ilosc_liczb;
}

  close(odczyt);
  close(zapis);
  unlink("MyFIFO");
  unlink("MyFIFO_przedziały");
  printf("Proces zakonczony %i\n",wynik);
} 



	
	
	
	
	
	
	
	
	
	
	

