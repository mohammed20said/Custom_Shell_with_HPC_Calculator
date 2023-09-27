#include<stdio.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
void main()
{
int p;
if((p=open("tubenom",O_RDONLY))==-1)
{perror("ouverture impossible du tube");exit(2);}
dup2(p,0);
if(fork()==0){
	execlp("cat","cat",NULL);
	perror("Pb execlp");
	exit(0);
}else{
	wait(0);
	printf("\033[1;31m"); // séquence d'échappement pour la couleur rouge
	printf("boukhryss shell> ");
	fflush(stdout);
	printf("\033[0m");// séquence d'échappement pour la couleur original
}
}
