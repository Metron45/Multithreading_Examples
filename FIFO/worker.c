#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

#define FIFO MyFIFO 

// Funkcja zwraca 1 gdy n jest liczba pierwsza 0 gdy nie   
int pierwsza(int n)                                 
{ int i,j=0; 
  for(i=2;i*i<=n;i++) { 
    if(n%i == 0) return(0) ; 
 } 	
 return(1); 
} 

//struktura wyniku
struct
{
int poczatek_zakresu;
int koniec_zakresu ;
int ilosc_liczb;
}wynik_t;

//main
void main(int argc,char *argv[]) { 

int numer_znalezionych 	= 0;  
int tmp_poczatek 	= atoi(argv[1]);
int tmp_koniec 		= atoi(argv[2]);
int zapis;

//liczenie liczb pierwszych
for(int i=tmp_poczatek;i <= tmp_koniec;i++) { 
if(pierwsza(i)){ numer_znalezionych++; }	
} 

//utworzenie wyniku 
wynik_t.poczatek_zakresu	= tmp_poczatek;
wynik_t.koniec_zakresu 		= tmp_koniec;
wynik_t.ilosc_liczb 		= numer_znalezionych;
	
//zapis do pliku
zapis = open("MyFIFO",O_RDWR);	
write(zapis, &wynik_t, sizeof(wynik_t) );
close(zapis);

printf("Worker Thread się zakończył %i\n",numer_znalezionych);
exit(0);
} 
