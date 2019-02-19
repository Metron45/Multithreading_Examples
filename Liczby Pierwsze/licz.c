#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>  

int pierwsza(int n) 
// Funkcja zwraca 1 gdy n jest liczba pierwsza 0 gdy nie                                   
{ int i,j=0; 
  for(i=2;i*i<=n;i++) { 
    if(n%i == 0) return(0) ; 
 } 	
 return(1); 
} 

void main(int argc,char *argv[]) { 
  int i ,poczatek,koniec; 
  int numer_znalezionych;
  
  numer_znalezionych = 0;
  poczatek=atoi(argv[1]);
  koniec=atoi(argv[2]);
  
  for(i=poczatek;i <= koniec;i++) { 
        if(pierwsza(i)){
	numer_znalezionych++;		
	}	
  } 
  //printf("Proces %s do %s zakonczony znaleziono: %d\n",argv[1],argv[2],numer_znalezionych);
  exit(numer_znalezionych); 
} 
