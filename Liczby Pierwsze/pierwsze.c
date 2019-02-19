#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <time.h>

void main(int argc, char *argv[]){ 
int pid,status, i,krok,procesy;
char tmp_poczatek[20], tmp_koniec[20];
int czas_poczatek,czas_koniec;

  krok=( atoi(argv[2]) - atoi(argv[1]) ) / atoi(argv[3]);
  czas_poczatek = time(NULL); 
  

for(i=0;i<atoi(argv[3]);i++){  

	sprintf(tmp_poczatek,"%d",(atoi(argv[1]) + krok * i));
	sprintf(tmp_koniec,"%d"  ,(atoi(argv[1]) + krok * (i+1)));

	if((pid=fork())==0){
		execl("./licz","licz",tmp_poczatek,tmp_koniec,NULL);
	}else if(pid == -1){
		printf("Proces nie zostal utworzony\n");
		break;
	}
}
  
for(i=0;i<atoi(argv[3]);i++){
	pid = wait(&status); 
	 
}

  czas_koniec=time(NULL);
  printf("Proces zakonczony sekundy: %d\n",czas_koniec-czas_poczatek);



} 



	
	
	
	
	
	
	
	
	
	
	

