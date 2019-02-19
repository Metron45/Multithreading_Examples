#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

struct
{
int poczatek_zakresu;
int koniec_zakresu ;
int ilosc_liczb;
}wynik_t;

int pierwsza(int n) 
// Funkcja zwraca 1 gdy n jest liczba pierwsza 0 gdy nie                                   
{ int i,j=0; 
  for(i=2;i*i<=n;i++) { 
    if(n%i == 0) return(0) ; 
 } 	
 return(1); 
} 


void main(int argc, char *argv[]){ 
int pid,status,krok,procesy,rd;
int tmp_poczatek,tmp_koniec,poczatek;
int wynik=0;
int file;

  file = creat("wynik",0666);
  //file = open("wynik",O_RDWR | O_CREAT | O_TRUNC,0666) ;

  krok=( atoi(argv[2]) - atoi(argv[1]) ) / atoi(argv[3]);
  poczatek= atoi(argv[1]);
  wynik_t.ilosc_liczb=0;
  

//pętla startująca thready
for(int i=0;i<atoi(argv[3]);i++){  

	tmp_poczatek =  poczatek + krok * i;
	if(i<atoi(argv[3])-1){
	tmp_koniec = 	poczatek + krok * (i+1)-1;
	}else{	
	tmp_koniec = atoi(argv[2]);
	}
	//nowy process
	if((pid=fork())==0){

		int numer_znalezionych = 0;  

			
  
  		for(i=tmp_poczatek;i <= tmp_koniec;i++) { 
        		if(pierwsza(i)){
				numer_znalezionych++;		
			}	
		} 
  
  		wynik_t.poczatek_zakresu= tmp_poczatek;
		wynik_t.koniec_zakresu = tmp_koniec;
		wynik_t.ilosc_liczb = numer_znalezionych;
		
		do{
		file = open("wynik",O_RDWR | O_APPEND);
		}while(file<0);		
		lockf(file,F_LOCK,0);
		write( file, &wynik_t, sizeof(wynik_t) );
		lockf(file,F_ULOCK,0);		
		close(file);
		

		exit(0);
	}
}

//petla czekająca na zakonczenie threadow  
for(int i=0;i<atoi(argv[3]);i++){
	pid = wait(&status);  
}

//petla odczytujaca z pliku
file = open("wynik",O_RDWR);
for(int i=0; i<atoi(argv[3]) ; i++){
 		rd = read(file, &wynik_t, sizeof(wynik_t) );
 		wynik = wynik + wynik_t.ilosc_liczb;
}
  close(file);

  printf("Proces zakonczony %i\n",wynik);



} 



	
	
	
	
	
	
	
	
	
	
	

